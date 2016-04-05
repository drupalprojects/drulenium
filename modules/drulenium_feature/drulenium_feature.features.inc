<?php
/**
 * @file
 * drulenium_feature.features.inc
 */

/**
 * Implements hook_views_api().
 */
function drulenium_feature_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_eck_bundle_info().
 */
function drulenium_feature_eck_bundle_info() {
  $items = array(
    'drulenium_test_task_click_button_task' => array(
      'machine_name' => 'drulenium_test_task_click_button_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'click_button_task',
      'label' => 'Click Button Task',
      'config' => array(),
    ),
    'drulenium_test_task_fill_textbox_task' => array(
      'machine_name' => 'drulenium_test_task_fill_textbox_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'fill_textbox_task',
      'label' => 'Fill Textbox Task',
      'config' => array(),
    ),
    'drulenium_test_task_goto_task' => array(
      'machine_name' => 'drulenium_test_task_goto_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'goto_task',
      'label' => 'GoTo Task',
      'config' => array(),
    ),
    'drulenium_test_task_hover_over_task' => array(
      'machine_name' => 'drulenium_test_task_hover_over_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'hover_over_task',
      'label' => 'Hover Over Task',
      'config' => array(),
    ),
    'drulenium_test_task_snapshot_task' => array(
      'machine_name' => 'drulenium_test_task_snapshot_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'snapshot_task',
      'label' => 'Snapshot Task',
      'config' => array(),
    ),
    'drulenium_test_task_verify_text_task' => array(
      'machine_name' => 'drulenium_test_task_verify_text_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'verify_text_task',
      'label' => 'Verify Text Task',
      'config' => array(),
    ),
    'drulenium_test_task_wait_task' => array(
      'machine_name' => 'drulenium_test_task_wait_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'wait_task',
      'label' => 'Wait Task',
      'config' => array(),
    ),
    'drulenium_visual_diff_drulenium_visual_diff' => array(
      'machine_name' => 'drulenium_visual_diff_drulenium_visual_diff',
      'entity_type' => 'drulenium_visual_diff',
      'name' => 'drulenium_visual_diff',
      'label' => 'Drulenium visual diff',
      'config' => array(),
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function drulenium_feature_eck_entity_type_info() {
  $items = array(
    'drulenium_test_task' => array(
      'name' => 'drulenium_test_task',
      'label' => 'Drulenium Test Task',
      'properties' => array(
        'title' => array(
          'label' => 'Title',
          'type' => 'text',
          'behavior' => 'title',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'changed' => array(
          'label' => 'Changed',
          'type' => 'integer',
          'behavior' => 'changed',
        ),
      ),
    ),
    'drulenium_visual_diff' => array(
      'name' => 'drulenium_visual_diff',
      'label' => 'Drulenium visual diff',
      'properties' => array(
        'title' => array(
          'label' => 'Title',
          'type' => 'text',
          'behavior' => 'title',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'changed' => array(
          'label' => 'Changed',
          'type' => 'integer',
          'behavior' => 'changed',
        ),
      ),
    ),
  );
  return $items;
}
