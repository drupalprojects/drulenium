<?php

/**
 * @file drulenium_ctools_export_ui.inc
 *
 * Ctools export-ui plugin for drulenium module
 */

$plugin = array(
  'schema' => 'drulenium_preset',
  'menu' => array(
    'menu prefix' => 'drulenium',
    'menu item' => 'tests',
    'menu title' => 'Tests',
    'menu description' => 'Run, Add, edit or delete tests.',
  ),
  'title' => t('Drulenium'),
  'access' => 'administer_tests',

  'title singular' => t('test'),
  'title plural' => t('tests'),
  'title singular proper' => t('Test'),
  'title plural proper' => t('Tests'),

  'handler' => array(
    'class' => 'drulenium_presets_ui',
    'parent' => 'ctools_export_ui',
  ),
);

$plugin['menu']['items']['edit']['path'] = '%ctools_export_ui/edit';
$plugin['menu']['items']['edit']['page arguments'] = array('drulenium_presets_ui', 'edit', 2);

/**
 * submit handler
 */
function drulenium_presets_ui_form_submit(&$form, &$form_state)
{
  $type = "all_plugins";
  $form_state['item']->settings = $form_state['values']['drulenium_settings_' . $type];

  $blockly_workspace_data = $form_state['values']['blockly_workspace_data'];
  $form_state['item']->settings["blockly"]["data"] = $blockly_workspace_data;
  $plugins_array = array();
  if (!empty($blockly_workspace_data)) {
    $xml = simplexml_load_string($blockly_workspace_data);
    if (false === $xml) {

    } else {
      drulenium_blockly_to_plugins_recursive($xml->block, $plugins_array);
    }
  }
  $form_state['item']->settings["plugins"] = $plugins_array;
}

/**
 * The preset add/edit form
 */
function drulenium_presets_ui_form(&$form, &$form_state) {
  ctools_include('dependent');
  $export = $form_state['item'];

  $form['description'] = array(
    '#title' => t('description'),
    '#type' => 'textfield',
    '#default_value' => ! empty($export->description) ? $export->description : '',
    '#description' => t('Description for this preset.')
  );

  $form['category'] = array(
    '#title' => t('Category'),
    '#type' => 'textfield',
    '#default_value' => ! empty($export->category) ? $export->category : '',
    '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".')
  );

  if (($library = libraries_load("blockly")) && !empty($library['library path'])) {
    $path = $library['library path'];
    $form['#attached']['js'][] = $path . '/appengine/storage.js';
    $form['#attached']['js'][] = $path . '/blockly_compressed.js';
    $form['#attached']['js'][] = $path . '/blocks_compressed.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/blocks/fill_textbox.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/blocks/take_screenshot.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/blocks/click_button.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/blocks/mouse_over.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/blocks/block_separator.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/blocks/wait.js';
    $form['#attached']['js'][] = $path . '/msg/js/en.js';
    $form['#attached']['js'][] = drupal_get_path('module', 'drulenium') . '/js/blockly/drulenium_blockly.js';

    $form['test_plugin_type']['blockly_mkup'] = array(
      '#markup' => '<a onclick="Drupal.behaviors.druleniumBlockly.save();">Save Blocks</a>
          <div id="blocklyDiv" style="height: 680px; width: 800px;"></div>
          <a onclick="Drupal.behaviors.druleniumBlockly.save();">Save Blocks</a>
          <xml id="toolbox" style="display: none">
          <block type="fill_textbox"></block>
          <block type="take_screenshot"></block>
          <block type="click_button"></block>
          <block type="mouse_over"></block>
          <block type="wait"></block>
          <block type="block_separator"></block>
          </xml>
      ',
    );

    $blockly_workspace_data = "";
    if(isset($export->settings["blockly"]["data"])) {
      $blockly_workspace_data = $export->settings["blockly"]["data"];
    }
    $form['blockly_workspace_data'] = array(
      '#type' => 'hidden',
      '#default_value' => $blockly_workspace_data,
    );
  }

  // TODO: Remove hardcoded unused plugin type
  $type = 'all_plugins';
  $id = 'drulenium-settings-' . $type;
  $wrapper_id = $id . '-wrapper';
  $form['drulenium_settings_' . $type] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#input' => TRUE,
    '#process' => array(
      'ctools_dependent_process'
    ),
    '#id' => $id,
    '#prefix' => '<div id="' . $wrapper_id . '">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE,
    '#tree' => TRUE
  );
  $form['drulenium_settings_' . $type] += _drulenium_presets_ui_create_block_form($export);
}

function drulenium_blockly_to_plugins_recursive ($xml_block_node, &$plugins_array) {
  $plugin_type = drulenium_blockly_get_plugin_type($xml_block_node);
  $plugin_count = count($plugins_array);
  $plugins_array[$plugin_type."_".$plugin_count] = array(
    "type" => $plugin_type,
    "weight" => $plugin_count,
    "description" => drulenium_blockly_get_plugin_fields($xml_block_node),
  );
  if (empty($xml_block_node->next)) {
    // Stop recursive if there is no <next> XML node
    // The Base Case/Guard Clause that stops the
    // function from running forever
    return $plugins_array;
  }
  else {
    //call function again with simpler args
    drulenium_blockly_to_plugins_recursive($xml_block_node->next->block, $plugins_array);
  }
}

function drulenium_blockly_get_plugin_fields($xml_block_node){
  $fields_arr = array();
  foreach($xml_block_node->field as $field) {
    $field_name = (string)$field["name"];
    $field_value = (string)$field;
    $fields_arr[$field_name] = $field_value;
  }
  return $fields_arr;
}

function drulenium_blockly_get_plugin_type($xml_block_node){
  $block_type = (string)$xml_block_node[0]["type"];
  switch ($block_type) {
    case BLOCKLY_FILL_TEXTBOX:
      $plugin_type = PLUGIN_FILL_TEXTBOX;
      break;
    case BLOCKLY_CLICK_BUTTON:
      $plugin_type = PLUGIN_CLICK_BUTTON;
      break;
    case BLOCKLY_TAKE_SCREENSHOT:
      $plugin_type = PLUGIN_TAKE_SCREENSHOT;
      break;
    case BLOCKLY_HOVER_OVER:
      $plugin_type = PLUGIN_HOVER_OVER;
      break;
    case BLOCKLY_WAIT:
      $plugin_type = PLUGIN_WAIT;
      break;
  }
  return $plugin_type;
}

function _drulenium_presets_ui_create_block_form($export) {
  $form = array();

  $form['pages'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'block') . '/block.js'),
    ),
  );

  // Per-path pages.
  $form['pages']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'pages',
    '#weight' => 0,
  );
  $form['pages']['path']['custom'] = array(
    '#type' => 'textarea',
    '#title' => t('Test on specific pages'),
    '#description' => "Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog.",
    '#default_value' => isset($export->settings['pages']['path']['custom']) ? $export->settings['pages']['path']['custom'] : '',
  );

  // Per-node-type testing.
  $form['pages']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'pages',
    '#weight' => 20,
  );
  $node_types = array();
  $result = db_query(
    'SELECT type, name FROM {node_type}');
  foreach ($result as $row) {
    $node_types[$row->type] = $row->name;
  }
  $form['pages']['node']['custom'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Test for specific content types'),
    '#options' => $node_types,
    '#description' => t('Test on pages that display content of the given type(s).'),
    '#default_value' => isset($export->settings['pages']['node']['custom']) ? $export->settings['pages']['node']['custom'] : array(),
  );

  // Per-term-type testing.
  $form['pages']['term'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'pages',
    '#weight' => 20,
  );
  $node_types = array();
  $result = db_query(
    'SELECT vid, name FROM {taxonomy_vocabulary}');
  foreach ($result as $row) {
    $node_types[$row->vid] = $row->name;
  }
  $form['pages']['term']['custom'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Test for specific Taxonomy Vocabularies'),
    '#options' => $node_types,
    '#description' => t('Test on taxonomy term pages of the given Vocabulary type(s)'),
    '#default_value' => isset($export->settings['pages']['term']['custom']) ? $export->settings['pages']['term']['custom'] : array(),
  );

  /*$user_types = user_roles(FALSE);
  $form['test_as_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Test as specific user roles'),
    '#options' => $user_types,
    '#description' => t('Test as users that belong to the above user role.'),
    '#default_value' => isset($export->settings['test_as_roles']) ? $export->settings['test_as_roles'] : 1,
  );*/
  return $form;
}
