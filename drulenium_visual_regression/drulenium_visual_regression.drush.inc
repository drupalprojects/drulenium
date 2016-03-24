<?php

/**
 * @file
 * Generate & compare snapshots via drush framework.
 */

/**
 * Implements hook_drush_command().
 */
function drulenium_visual_regression_drush_command() {
  $items['vr-create'] = array(
    'callback' => 'drush_drulenium_visual_regression_create',
    'description' => 'Create release and generate snapshots.',
    'drupal dependencies' => array('drulenium_visual_regression'),
    'arguments' => array(
      'base_url' => 'Base URL of the site to test.',
      'release_name' => 'Name of the release to create.',
    ),
    'options' => array(
      'preset_cid' => 'Test Preset cid to base the test on.', // drush vrc --preset_cid=anonymous_permissions_test_case
      'preset_category' => 'Test Preset category of all tests to run.',
      'pages' => 'List of pages to use on the base_url',
    ),
    'aliases' => array('vrc'),
  );
  $items['vr-update'] = array(
    'callback' => 'drush_drulenium_visual_regression_update',
    'description' => 'Generate new snapshots for a release.',
    'drupal dependencies' => array('drulenium_visual_regression'),
    'arguments' => array(
      'release_name' => 'Name of the release to update.',
    ),
    'aliases' => array('vru'),
  );
  $items['vr-diff'] = array(
    'callback' => 'drush_drulenium_visual_regression_diff',
    'description' => 'Generate comparision report for 2 releases.',
    'drupal dependencies' => array('drulenium_visual_regression'),
    'arguments' => array(
      'from' => 'Release ID of the first set of screenshots.',
      'to' => 'Release ID of the second set of screenshots.',
    ),
    'options' => array(
      'preset_category' => 'Test Preset category of all tests to diff.',
    ),
    'aliases' => array('vrd'),
  );
  return $items;
}

/**
 * Command callback. Create a release & generate snapshots.
 */
function drush_drulenium_visual_regression_create($vr_base_url = "", $release_name = "") {
  global $base_url;

  // Option
  $preset_cid = drush_get_option('preset_cid', ''); // Default to empty string
  $preset_category = drush_get_option('preset_category', '');

  if(empty($preset_cid)) {
    $drulenium_vr_path_custom = variable_get("drulenium_vr_path_custom");
    $pages_array = drush_get_option_list('pages');
    if (empty($pages_array)) {
      $pages_array = array('<front>');
    }
  } else {
    $config_array = drulenium_visual_regression_get_preset_config($preset_cid);
    $drulenium_vr_path_custom = $config_array['specific_pages'];
  }


  $drulenium_vr_user_pass = variable_get("drulenium_vr_release_user_pass", '');
  $drulenium_vr_user_name = variable_get("drulenium_vr_release_user_name", '');
  $secureid = drupal_hash_base64(drupal_random_bytes(55));

  if (empty($release_name)) {
    $release_name = format_date(REQUEST_TIME);
  }

  // Default to the current site if no $vr_base_url is provided.
  if (empty($vr_base_url)) {
    $vr_base_url = $base_url . "/?q=";
  }

  drush_log(t('Starting snapshot process of @url', array('@url' => $vr_base_url)), 'ok');

  if (!empty($preset_category)) {
    $presets = drulenium_preset_load();
    foreach ($presets as $preset_cid => $value) {
      $category = $value->category;
      if(empty($category))  $category = 'Miscellaneous';
      if($category == $preset_category){
        $config_array = drulenium_visual_regression_get_preset_config($preset_cid);
        $drulenium_vr_path_custom = $config_array['specific_pages'];
        $pages_array = array();
        $re_id = drulenium_visual_regression_create_release_in_db($secureid, $release_name, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass, $specific_node_types = array(), $specific_menu_types = array(), $specific_term_types = array(), $specific_pages = $drulenium_vr_path_custom, $preset_cid);
        drulenium_visual_regression_release_setup_batch($re_id);
      }
    }
  }
  else {
    $re_id = drulenium_visual_regression_create_release_in_db($secureid, $release_name, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass, $specific_node_types = array(), $specific_menu_types = array(), $specific_term_types = array(), $specific_pages = $drulenium_vr_path_custom);

    drulenium_visual_regression_release_setup_batch($re_id);
  }

  drush_backend_batch_process();
  $url = $base_url . '/admin/structure/drulenium/vr/release/' . $re_id;
  drush_log(t('Took snapshots of @vrpages pages, view on @url', array('@vrpages' => count($drulenium_vr_path_custom), '@url' => $url)), 'success');
}

/**
 * Command callback. Update a release & generate snapshots.
 */
function drush_drulenium_visual_regression_update($release_name = "") {

  drush_log("Updating Drulenium release: $release_name", 'ok');
  $release_id = db_select('drulenium_vr_releases', 're')
    ->fields('re')
    ->condition('name', $release_name, '=')
    ->execute()
    ->fetchField();

  // Wipe the current snapshot's to trigger a refresh.
  db_update('drulenium_vr_runs')
    ->fields(array('snapshot' => ''))
    ->condition('re_id', $release_id, '=')
    ->execute();

  drulenium_visual_regression_release_setup_batch($release_id);

  drush_backend_batch_process();

  $num_pages = db_select('drulenium_vr_runs', 'r')
    ->condition('re_id', $release_id, '=')
    ->countQuery()->execute()->fetchField();

  drush_log(t('Took snapshots of @vrpages pages.', array('@vrpages' => $num_pages)), 'success');

}

/**
 * Command callback. Generate comparision report of releases..
 *
 * Optionally one per category if the preset_category is used.
 */
function drush_drulenium_visual_regression_diff($old_re_id = null, $new_re_id = null) {
  $preset_category = drush_get_option('preset_category', '');
  if (!empty($preset_category)) {
    return _drush_drulenium_visual_regression_loop_categories($preset_category);
  }

  if ($old_re_id == null || $new_re_id == null) {
    // Get the last two releases.
    $result = db_select('drulenium_vr_releases', 'vr_re')
      ->fields('vr_re', array('rid'))
      ->orderBy('rid', 'DESC')
      ->range(0, 2)
      ->execute();
  }
  if ($new_re_id == null) {
    $row1 = $result->fetchAssoc();
    $new_re_id = $row1['rid'];
  }
  if ($old_re_id == null) {
    $row2 = $result->fetchAssoc();
    $old_re_id = $row2['rid'];
  }
  _drush_drulenium_visual_regression_diff($old_re_id, $new_re_id);
  drush_backend_batch_process();
  global $base_url;
  drush_log("View results on $base_url/admin/structure/drulenium/vr/compare/$old_re_id/$new_re_id", 'success');
}

/**
 * Start a diff of the selected preset categories.
 *
 * @param string $preset_category
 */
function _drush_drulenium_visual_regression_loop_categories($preset_category) {
  $presets = drulenium_preset_load();
  foreach ($presets as $preset_cid => $value) {
    $category = $value->category;
    if (empty($category)) {
      $category = 'Miscellaneous';
    }
    if ($category == $preset_category) {
      // Get last two releases of this preset.
      $result = db_select('drulenium_vr_releases', 'vr_re')
        ->fields('vr_re', array('rid'))
        ->condition('vr_re.preset_cid', $preset_cid)
        ->orderBy('rid', 'DESC')
        ->range(0, 2)
        ->execute();
      $row1 = $result->fetchAssoc();
      $new_re_id = $row1['rid'];
      $row2 = $result->fetchAssoc();
      $old_re_id = $row2['rid'];
      _drush_drulenium_visual_regression_diff($old_re_id, $new_re_id);
    }
  }
  drush_backend_batch_process();
}

/**
 * Generate comparision report for 2 releases.
 */
function _drush_drulenium_visual_regression_diff($old_re_id, $new_re_id) {

  // Wipe the current comparison's to trigger a refresh.
  $or = db_or()->condition('release_base_id', $old_re_id)->condition('release_cmp_id', $new_re_id);
  $num_pages = db_delete('drulenium_vr_results')
    ->condition($or)
    ->execute();

  drulenium_visual_regression_create_initial_comparision_report_in_db($old_re_id, $new_re_id);

  // Process the queue using Batch API.
  $batch = array(
    'file' => drupal_get_path('module', 'drulenium_visual_regression') . '/drulenium_visual_regression.module',
    'operations' => array(),
    'finished' => 'drulenium_visual_regression_compare_batch_finished',
    'title' => t('Performing %operation on the selected items...', array('%operation' => "snapshot comparision")),
  );
  $progress = 0; // where to start
  $limit = 1; // how many to process for each run

  // Get unprocessed runs of specified compare
  // where snapshot is empty.
  $query = db_select('drulenium_vr_results', 'vr_re');
  $query
    ->fields('vr_re')
    ->condition('vr_re.release_base_id', $old_re_id)
    ->condition('vr_re.release_cmp_id', $new_re_id)
    ->isNull('vr_re.diff_percent');
  $query->addExpression('COUNT(rid)', 'run_count');
  $result = $query->execute();
  $row = $result->fetchAssoc();
  $max = $row['run_count'];

  while ($progress <= $max) {
    $batch['operations'][] = array('drulenium_visual_regression_compare_batch_op', array($old_re_id, $new_re_id, $progress, $limit));
    $progress = $progress + $limit;
  }

  batch_set($batch);
}
