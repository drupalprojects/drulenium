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
      'og_group_ref' => 'If Organic group node id is specified, this release entity will be part of the group.',
    ),
    'aliases' => array('vrc'),
  );
  $items['vr-one-step'] = array(
      'callback' => 'drush_drulenium_visual_regression_one_step',
      'description' => 'Create baseline & test releases, generate snapshots & compare the two releases.',
      'drupal dependencies' => array('drulenium_visual_regression'),
      'arguments' => array(
          'release_name' => 'Name of the release to create.',
          'release_message' => 'Description of the release like commit message.',
      ),
      'options' => array(
          'preset_cid' => 'Test Preset cid to base the test on.', // drush vrc --preset_cid=anonymous_permissions_test_case
          'preset_category' => 'Test Preset category of all tests to run.',
          'pages' => 'List of pages to use on the base_url',
          'og_group_ref' => 'If Organic group node id is specified, this release entity will be part of the group.',
      ),
      'aliases' => array('vr1'),
  );
  $items['vr-test-import'] = array(
      'callback' => 'drush_drulenium_visual_regression_test_import',
      'description' => 'Import test case.',
      'drupal dependencies' => array('drulenium_visual_regression'),
      'arguments' => array(
          'test_url' => 'Test URL of the test case.',
          'category' => 'Category of the test.',
      ),
      'aliases' => array('vr-t-i'),
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
  $items['vr-diff-upload'] = array(
    'callback' => 'drush_drulenium_visual_regression_upload',
    'description' => 'Generate comparision report for 2 releases.',
    'drupal dependencies' => array('drulenium_visual_regression'),
    'arguments' => array(
      'from' => 'Release ID of the first set of screenshots.',
      'to' => 'Release ID of the second set of screenshots.',
    ),
    'options' => array(
      'preset_category' => 'Test Preset category of all tests to upload the diff.',
      'project_id' => 'Upload the diff results under this project.',
    ),
    'aliases' => array('vrdup'),
  );
  $items['vr-download-webdriver'] = array(
    'callback'    => 'drush_drulenium_visual_regression_download_webdriver',
    'description' => dt('This command used to download selenium php webdriver.'),
    'aliases'     => array('vr-dl-se'),
  );
  $items['vr-download-blockly'] = array(
    'callback'    => 'drush_drulenium_visual_regression_download_blockly',
    'description' => dt('This command used to download blockly javascript library.'),
    'aliases'     => array('vr-dl-bl'),
  );
  return $items;
}

/**
 * Command callback. Create a release & generate snapshots.
 */
function drush_drulenium_visual_regression_one_step($release_name = "", $release_message = "") {
  //will call selected module implementing hook_drulenium_one_step_process
  $screenshot_server_module = variable_get('drulenium_vr_config_server_opt', 'drulenium_hosting_client');
  $return_array = module_invoke($screenshot_server_module, 'drulenium_one_step_process', $release_name, $release_message);
}

/**
 * Command callback. Import test.
 */
function drush_drulenium_visual_regression_test_import($test_url, $category = "") {
  $import_code = file_get_contents($test_url);
  module_load_include('inc', 'ctools', 'includes/export');
  $item = ctools_export_crud_import('drulenium_preset', $import_code);
  ctools_export_crud_save('drulenium_preset', $item);
  $cid = $item->name;
  if(!empty($category)){
    // Override category
    $item->category = $category;
    ctools_export_crud_save('drulenium_preset', $item);
  }
  drush_log(t('imported').$cid, 'success');
}

/**
 * Helper function downloads blockly javascript library.
 */
function drush_drulenium_visual_regression_download_blockly() {

  // Get the path from the argument, if site, or use the default.
  $drush_context = drush_get_context('DRUSH_DRUPAL_ROOT');

  // Can we use Libraries API?
  if (module_exists('libraries') && function_exists('libraries_get_path')) {
    $libraries_dir = libraries_get_path('blockly');
  }

  // Load the path.
  $path = $libraries_dir ? $drush_context . '/' . $libraries_dir : $drush_context . '/sites/all/libraries/blockly';

  drush_log(dt('Download destination path: "@path"', array('@path' => $path)), 'notice');

  // Set the directory to the download location.
  $olddir = getcwd();

  $download_path = str_replace('/blockly', '', $path);

  chdir($download_path);

  $download_uri = 'http://drulenium.org/downloads/blockly.zip';

  // Download the zip archive.
  if ($filepath = drush_download_file($download_uri, $download_path . '/blockly.zip')) {
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "select2" if needed.
    if ($dirname != 'blockly') {
      drush_move_dir($dirname, 'blockly', TRUE);
      $dirname = 'blockly';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Blockly library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Blockly library to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

}

/**
 * Helper function downloads selenium webdriver plugin.
 */
function drush_drulenium_visual_regression_download_webdriver() {

  // Get the path from the argument, if site, or use the default.
  $drush_context = drush_get_context('DRUSH_DRUPAL_ROOT');

  // Can we use Libraries API?
  if (module_exists('libraries') && function_exists('libraries_get_path')) {
    $libraries_dir = libraries_get_path('selenium_webdriver');
  }

  // Load the path.
  $path = $libraries_dir ? $drush_context . '/' . $libraries_dir : $drush_context . '/sites/all/libraries/selenium_webdriver';

  drush_log(dt('Download destination path: "@path"', array('@path' => $path)), 'notice');

  // Set the directory to the download location.
  $olddir = getcwd();

  $download_path = str_replace('/selenium_webdriver', '', $path);

  chdir($download_path);

  $download_uri = 'https://github.com/facebook/php-webdriver/archive/master.zip';

  // Download the zip archive.
  if ($filepath = drush_download_file($download_uri, $download_path . '/php-webdriver-master.zip')) {
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "select2" if needed.
    if ($dirname != 'selenium_webdriver') {
      drush_move_dir($dirname, 'selenium_webdriver', TRUE);
      $dirname = 'selenium_webdriver';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Selenium webdriver has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Selenium webdriver to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

}

/**
 * Command callback. Create a release & generate snapshots.
 */
function drush_drulenium_visual_regression_create($vr_base_url = "", $release_name = "") {
  global $base_url;

  // Option
  $preset_cid = drush_get_option('preset_cid', ''); // Default to empty string
  $preset_category = drush_get_option('preset_category', '');
  $og_group_ref = drush_get_option('og_group_ref', '');

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

  $drulenium_server = FALSE;
  if (variable_get('drulenium_vr_config_server_opt', 'drulenium_hosting_client') == "drulenium_hosting_client") {
    $drulenium_server = TRUE;
  }

  if (!empty($preset_category)) {
    $presets = drulenium_preset_load();
    foreach ($presets as $preset_cid => $value) {
      $category = $value->category;
      if(empty($category))  $category = 'Miscellaneous';
      if($category == $preset_category){
        $config_array = drulenium_visual_regression_get_preset_config($preset_cid);
        $drulenium_vr_path_custom = $config_array['specific_pages'];
        $pages_array = array();
        $secureid = drupal_hash_base64(drupal_random_bytes(55));
        $re_id = drulenium_visual_regression_create_release_in_db($secureid, $release_name, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass, $specific_node_types = array(), $specific_menu_types = array(), $specific_term_types = array(), $specific_pages = $drulenium_vr_path_custom, $preset_cid, $settings_array = array(), $og_group_ref);
        if($drulenium_server){
          drulenium_visual_regression_create_release_in_drulenium_db($re_id, $secureid, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass);
        }else{
          drulenium_visual_regression_release_setup_batch($re_id);
        }
      }
    }
  }
  else {
    $re_id = drulenium_visual_regression_create_release_in_db($secureid, $release_name, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass, $specific_node_types = array(), $specific_menu_types = array(), $specific_term_types = array(), $specific_pages = $drulenium_vr_path_custom, $preset_cid, $settings_array = array(), $og_group_ref);

    if($drulenium_server){
      drulenium_visual_regression_create_release_in_drulenium_db($re_id, $secureid, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass);
    }else{
      drulenium_visual_regression_release_setup_batch($re_id);
    }
  }
  if(!$drulenium_server){
    drush_backend_batch_process();
  }
  $url = $base_url . '/drulenium/vr/release/' . $re_id;
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
    $result = db_select('drulenium', 'vr_re')
      ->fields('vr_re', array('drulenium_id'))
      ->orderBy('drulenium_id', 'DESC')
      ->range(0, 2)
      ->execute();
  }
  if ($new_re_id == null) {
    $row1 = $result->fetchAssoc();
    $new_re_id = $row1['drulenium_id'];
  }
  if ($old_re_id == null) {
    $row2 = $result->fetchAssoc();
    $old_re_id = $row2['drulenium_id'];
  }
  _drush_drulenium_visual_regression_diff($old_re_id, $new_re_id);
  drush_backend_batch_process();
  global $base_url;
  drush_log("View results on $base_url/drulenium/vr/compare/$old_re_id/$new_re_id", 'success');
}

/**
 * Command callback. Generate comparision report of releases..
 *
 * Optionally one per category if the preset_category is used.
 */
function drush_drulenium_visual_regression_upload($old_re_id = null, $new_re_id = null) {
  $preset_category = drush_get_option('preset_category', '');
  $project_id = drush_get_option('project_id', '');
  if (!empty($preset_category)) {
    return _drush_drulenium_visual_regression_diff_upload_categories($preset_category, $project_id);
  }
  if ($old_re_id == null || $new_re_id == null) {
    // Get the last two releases.
    $result = db_select('drulenium', 'vr_re')
      ->fields('vr_re', array('drulenium_id'))
      ->orderBy('drulenium_id', 'DESC')
      ->range(0, 2)
      ->execute();
  }
  if ($new_re_id == null) {
    $row1 = $result->fetchAssoc();
    $new_re_id = $row1['drulenium_id'];
  }
  if ($old_re_id == null) {
    $row2 = $result->fetchAssoc();
    $old_re_id = $row2['drulenium_id'];
  }
  drulenium_visual_regression_update_result_in_drulenium_db($old_re_id, $new_re_id, $project_id);
  drush_backend_batch_process();
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
      /*$preset_numeric_id = db_select('drulenium_preset', 'p')
        ->fields('p', array('oid'))
        ->condition('p.name', $preset_cid)
        ->execute()
        ->fetchfield();*/
      // Get last two releases of this preset.
      $result = db_select('drulenium', 'vr_re')
        ->fields('vr_re', array('drulenium_id'))
        ->condition('vr_re.test_id', $preset_cid)
        ->orderBy('drulenium_id', 'DESC')
        ->range(0, 2)
        ->execute();
      $row1 = $result->fetchAssoc();
      $new_re_id = $row1['drulenium_id'];
      $row2 = $result->fetchAssoc();
      $old_re_id = $row2['drulenium_id'];
      _drush_drulenium_visual_regression_diff($old_re_id, $new_re_id);
      global $base_url;
      drush_log("View results on $base_url/drulenium/vr/compare/$old_re_id/$new_re_id", 'success');
    }
  }
  drush_backend_batch_process();
}

/**
 * Start a diff upload of the selected preset categories.
 *
 * @param string $preset_category
 */
function _drush_drulenium_visual_regression_diff_upload_categories($preset_category, $project_id = '') {
  $presets = drulenium_preset_load();
  // Generate $build_id
  $build_id = drupal_hash_base64(drupal_random_bytes(55));
  foreach ($presets as $preset_cid => $value) {
    $category = $value->category;
    if (empty($category)) {
      $category = 'Miscellaneous';
    }
    if ($category == $preset_category) {
      /*$preset_numeric_id = db_select('drulenium_preset', 'p')
        ->fields('p', array('oid'))
        ->condition('p.name', $preset_cid)
        ->execute()
        ->fetchfield();*/
      // Get last two releases of this preset.
      $result = db_select('drulenium', 'vr_re')
      ->fields('vr_re', array('drulenium_id'))
      ->condition('vr_re.test_id', $preset_cid)
      ->orderBy('drulenium_id', 'DESC')
      ->range(0, 2)
      ->execute();
      $row1 = $result->fetchAssoc();
      $new_re_id = $row1['drulenium_id'];
      $row2 = $result->fetchAssoc();
      $old_re_id = $row2['drulenium_id'];
      drulenium_visual_regression_update_result_in_drulenium_db($old_re_id, $new_re_id, $project_id, $build_id);
    }
  }
  drush_backend_batch_process();
}

/**
 * Generate comparision report for 2 releases.
 */
function _drush_drulenium_visual_regression_diff($old_re_id, $new_re_id) {

  // Wipe the current comparison's to trigger a refresh.
  // TODO: Delete entitites
  // entity_delete_multiple($entity_type, array $ids)
  /*$or = db_or()->condition('release_base_id', $old_re_id)->condition('release_cmp_id', $new_re_id);
  $num_pages = db_delete('drulenium_vr_results')
    ->condition($or)
    ->execute();*/

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
  $query = db_select('eck_drulenium_visual_diff', 'diff');
  $query-> leftJoin('field_data_field_dr_diff_base_task', 'base_tsk', 'diff.id = base_tsk.entity_id');
  $query-> leftJoin('field_data_field_dr_diff_cmp_task', 'cmp_tsk', 'diff.id = cmp_tsk.entity_id');
  $query-> leftJoin('field_data_field_dr_task_release', 'base_rel', 'base_tsk.field_dr_diff_base_task_target_id = base_rel.entity_id');
  $query-> leftJoin('field_data_field_dr_task_release', 'cmp_rel', 'cmp_tsk.field_dr_diff_cmp_task_target_id = cmp_rel.entity_id');
  $query-> leftJoin('field_data_field_dr_diff_percentage', 'diff_percent', 'diff.id = diff_percent.entity_id');
  $query->fields('diff', array('id'));
  $query->condition('base_rel.field_dr_task_release_target_id', $old_re_id);
  $query->condition('cmp_rel.field_dr_task_release_target_id', $new_re_id);
  $query->isNull('diff_percent.field_dr_diff_percentage_value');
  $query->addExpression('COUNT(id)', 'run_count');
  $result = $query->execute();
  $row = $result->fetchAssoc();
  $max = $row['run_count'];

  while ($progress <= $max) {
    $batch['operations'][] = array('drulenium_visual_regression_compare_batch_op', array($old_re_id, $new_re_id, $progress, $limit));
    $progress = $progress + $limit;
  }

  batch_set($batch);
}
