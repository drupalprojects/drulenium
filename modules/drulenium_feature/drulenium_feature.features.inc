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
    'drulenium_test_task_goto_task' => array(
      'machine_name' => 'drulenium_test_task_goto_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'goto_task',
      'label' => 'GoTo Task',
      'config' => array(),
    ),
    'drulenium_test_task_snapshot_task' => array(
      'machine_name' => 'drulenium_test_task_snapshot_task',
      'entity_type' => 'drulenium_test_task',
      'name' => 'snapshot_task',
      'label' => 'Snapshot Task',
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
  );
  return $items;
}
