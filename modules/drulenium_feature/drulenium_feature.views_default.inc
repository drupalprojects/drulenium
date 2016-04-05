<?php
/**
 * @file
 * drulenium_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function drulenium_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'drulenium_diff_details';
  $view->description = 'Table view of all tasks diff in a comparison request';
  $view->tag = 'default';
  $view->base_table = 'eck_drulenium_visual_diff';
  $view->human_name = 'drulenium_diff_details';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
  );
  $handler->display->display_options['style_options']['default'] = 'id';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No data to show yet';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_dr_diff_base_task_target_id']['id'] = 'field_dr_diff_base_task_target_id';
  $handler->display->display_options['relationships']['field_dr_diff_base_task_target_id']['table'] = 'field_data_field_dr_diff_base_task';
  $handler->display->display_options['relationships']['field_dr_diff_base_task_target_id']['field'] = 'field_dr_diff_base_task_target_id';
  $handler->display->display_options['relationships']['field_dr_diff_base_task_target_id']['label'] = 'entity ref from field_dr_diff_base_task';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_dr_diff_cmp_task_target_id']['id'] = 'field_dr_diff_cmp_task_target_id';
  $handler->display->display_options['relationships']['field_dr_diff_cmp_task_target_id']['table'] = 'field_data_field_dr_diff_cmp_task';
  $handler->display->display_options['relationships']['field_dr_diff_cmp_task_target_id']['field'] = 'field_dr_diff_cmp_task_target_id';
  $handler->display->display_options['relationships']['field_dr_diff_cmp_task_target_id']['label'] = 'entity ref from field_dr_diff_cmp_task';
  /* Field: Drulenium visual diff: Id */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'eck_drulenium_visual_diff';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: Drulenium visual diff: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'eck_drulenium_visual_diff';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Drulenium visual diff: Diff Percentage */
  $handler->display->display_options['fields']['field_dr_diff_percentage']['id'] = 'field_dr_diff_percentage';
  $handler->display->display_options['fields']['field_dr_diff_percentage']['table'] = 'field_data_field_dr_diff_percentage';
  $handler->display->display_options['fields']['field_dr_diff_percentage']['field'] = 'field_dr_diff_percentage';
  $handler->display->display_options['fields']['field_dr_diff_percentage']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Drulenium visual diff: Highlighted image */
  $handler->display->display_options['fields']['field_diff_file_highlight']['id'] = 'field_diff_file_highlight';
  $handler->display->display_options['fields']['field_diff_file_highlight']['table'] = 'field_data_field_diff_file_highlight';
  $handler->display->display_options['fields']['field_diff_file_highlight']['field'] = 'field_diff_file_highlight';
  $handler->display->display_options['fields']['field_diff_file_highlight']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_diff_file_highlight']['alter']['text'] = '<a href="[field_diff_file_highlight-value]" target="_blank"><img src="[field_diff_file_highlight-value]" width=200 /></a>';
  /* Field: Drulenium visual diff: GIF image */
  $handler->display->display_options['fields']['field_diff_file_gif']['id'] = 'field_diff_file_gif';
  $handler->display->display_options['fields']['field_diff_file_gif']['table'] = 'field_data_field_diff_file_gif';
  $handler->display->display_options['fields']['field_diff_file_gif']['field'] = 'field_diff_file_gif';
  $handler->display->display_options['fields']['field_diff_file_gif']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_diff_file_gif']['alter']['text'] = '<a href="[field_diff_file_gif-value]" target="_blank"><img src="[field_diff_file_gif-value]" width=200 /></a>';
  /* Field: Drulenium visual diff: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'eck_drulenium_visual_diff';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Drulenium visual diff: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'eck_drulenium_visual_diff';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Contextual filter: Drulenium Test Task: Test release (field_dr_task_release) */
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['id'] = 'field_dr_task_release_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['table'] = 'field_data_field_dr_task_release';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['field'] = 'field_dr_task_release_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['relationship'] = 'field_dr_diff_base_task_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Drulenium Test Task: Test release (field_dr_task_release) */
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['id'] = 'field_dr_task_release_target_id_1';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['table'] = 'field_data_field_dr_task_release';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['field'] = 'field_dr_task_release_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['relationship'] = 'field_dr_diff_cmp_task_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id_1']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Drulenium visual diff: drulenium_visual_diff type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eck_drulenium_visual_diff';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'drulenium_visual_diff' => 'drulenium_visual_diff',
  );
  $export['drulenium_diff_details'] = $view;

  $view = new view();
  $view->name = 'drulenium_release_details';
  $view->description = 'Table view of all tasks in a release';
  $view->tag = 'default';
  $view->base_table = 'eck_drulenium_test_task';
  $view->human_name = 'Drulenium Release details';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = 'drulenium/vr/release';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
  );
  $handler->display->display_options['style_options']['default'] = 'id';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No tasks to show here.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Drulenium Test Task: Id */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'eck_drulenium_test_task';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: Drulenium Test Task: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'eck_drulenium_test_task';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Drulenium Test Task: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'eck_drulenium_test_task';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Drulenium Test Task: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'eck_drulenium_test_task';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Processed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: Drulenium Test Task: Output */
  $handler->display->display_options['fields']['field_dr_task_output']['id'] = 'field_dr_task_output';
  $handler->display->display_options['fields']['field_dr_task_output']['table'] = 'field_data_field_dr_task_output';
  $handler->display->display_options['fields']['field_dr_task_output']['field'] = 'field_dr_task_output';
  $handler->display->display_options['fields']['field_dr_task_output']['element_default_classes'] = FALSE;
  /* Field: Drulenium Test Task: Status */
  $handler->display->display_options['fields']['field_dr_task_status']['id'] = 'field_dr_task_status';
  $handler->display->display_options['fields']['field_dr_task_status']['table'] = 'field_data_field_dr_task_status';
  $handler->display->display_options['fields']['field_dr_task_status']['field'] = 'field_dr_task_status';
  /* Field: Drulenium Test Task: drulenium_test_task type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'eck_drulenium_test_task';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['type']['machine_name'] = TRUE;
  /* Contextual filter: Drulenium Test Task: Test release (field_dr_task_release) */
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['id'] = 'field_dr_task_release_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['table'] = 'field_data_field_dr_task_release';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['field'] = 'field_dr_task_release_target_id';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['exception']['value'] = '';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['default_argument_options']['index'] = '5';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_dr_task_release_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Drulenium Test Task: drulenium_test_task type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eck_drulenium_test_task';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Task category';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $export['drulenium_release_details'] = $view;

  return $export;
}
