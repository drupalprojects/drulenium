<?php

/**
 * @file drulenium_ctools_export_ui.inc
 * 
 * Ctools export-ui plugin for drulenium module
 */

$plugin = array(
    'schema' => 'drulenium_preset', 
  'menu' => array(
    'menu prefix' => 'admin/structure/drulenium',
    'menu item' => 'presets',
    'menu title' => 'Presets',
    'menu description' => 'Add, edit or delete presets.',
  ), 
    'title' => t('Drupal presets'), 
    
    'title singular' => t('preset'), 
    'title plural' => t('presets'), 
    'title singular proper' => t('Preset'), 
    'title plural proper' => t('Presets'),

  'handler' => array(
    'class' => 'drulenium_presets_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
 * submit handler
 */
function drulenium_presets_ui_form_submit(&$form, &$form_state) {
  //dpm($form_state);
  $type = $form_state['values']['test_plugin_type']['plugin_type'];
  $form_state['item']->plugin_type = $type;
  $form_state['item']->drulenium_attrs = $form_state['values']['drulenium_settings_' . $type]['drulenium_attrs_' . $type];
  $form_state['item']->settings = $form_state['values']['drulenium_settings_' . $type]['settings_' . $type];
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
  
  $all_plugins = drulenium_test_plugin_load();
  $options = array();
  foreach ( $all_plugins as $plugin ) {
    $options[$plugin['name']] = $plugin['description'];
  }
  
  $form['test_plugin_type'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Drulenium plugin type'), 
      '#collapsible' => TRUE, 
      '#tree' => TRUE 
  );
  
  $form['test_plugin_type']['plugin_type'] = array(
      '#title' => t('type'), 
      '#type' => 'radios', 
      '#options' => $options, 
      '#default_value' => ! empty($export->plugin_type) ? $export->plugin_type : 'like', 
      '#description' => t('Description for this preset.') 
  );
  
  // output all form settings here but show/hide using
  // ctools 'dependent' plugin
  

  foreach ( $all_plugins as $type => $plugin ) {
    
    // fb settings
    $id = 'drulenium-settings-' . $type;
    $wrapper_id = $id . '-wrapper';
    $form['drulenium_settings_' . $type] = array(
        '#type' => 'fieldset', 
        '#title' => t('%description  - settings', array(
            '%description' => $plugin['description'] 
        )), 
        '#input' => TRUE, 
        '#process' => array(
            'ctools_dependent_process' 
        ), 
        '#dependency' => array(
            'radio:test_plugin_type[plugin_type]' => array(
                $type 
            ) 
        ), 
        '#id' => $id, 
        '#prefix' => '<div id="' . $wrapper_id . '">', 
        '#suffix' => '</div>', 
        '#collapsible' => TRUE, 
        '#tree' => TRUE 
    );
    
    // Drulenium attrs
    $form['drulenium_settings_' . $type]['drulenium_attrs_' . $type] = array(
        '#type' => 'fieldset', 
        '#collapsible' => TRUE, 
        '#title' => 'Drulenium plugin attributes' 
    );
    
    $form['drulenium_settings_' . $type]['drulenium_attrs_' . $type] += drulenium_test_plugin_fb_settings_form($type, $export->drulenium_attrs);
 /*   
    // drupal settings
    $form['drulenium_settings_' . $type]['settings_' . $type] = array(
        '#type' => 'fieldset', 
        '#title' => 'Drupal settings', 
        '#collapsible' => TRUE 
    );
    
    //$form['drulenium_settings_' . $type]['settings_' . $type] += drulenium_test_plugin_drupal_settings_form($type, $export->settings);
    

    $drulenium_attrs_form = drulenium_test_plugin_drupal_settings_form($type, $export->settings);
    
    foreach ( $drulenium_attrs_form as $k => $v ) {
      $form['drulenium_settings_' . $type]['settings_' . $type][$k] = $v;
    }
    
    $form['drulenium_settings_' . $type]['settings_' . $type] += _drulenium_presets_ui_create_block_form($export);
 */ 
  }
}

function _drulenium_presets_ui_create_block_form($export) {
  $form = array();
  $form['block'] = array(
      '#type' => 'checkbox', 
      '#title' => t('Create a block'), 
      '#description' => t('Create a drupal block that contains this plugin.  You have to enable the block manually to show up.'), 
      
      '#default_value' => isset($export->settings['block']) ? $export->settings['block'] : 0 
  );
  return $form;
}

