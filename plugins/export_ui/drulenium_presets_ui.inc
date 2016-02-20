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
        'menu title' => 'Tests',
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
  $type = 'drulenium_facepile';
  //$type = $form_state['values']['test_plugin_type']['plugin_type'];
  $form_state['item']->plugin_type = $type;
  //$form_state['item']->drulenium_attrs = $form_state['values']['drulenium_settings_' . $type]['drulenium_attrs_' . $type];
  //$form_state['item']->settings = $form_state['values']['drulenium_settings_' . $type]['settings_' . $type];
  $form_state['item']->settings = $form_state['values']['drulenium_settings_' . $type];

  if(isset($form_state['values']['test_plugin_type']['plugin_items'])) {
    uasort($form_state['values']['test_plugin_type']['plugin_items'], "drulenium_sort_plugin_items_by_weight");
    $form_state['item']->settings["plugins"] = $form_state['values']['test_plugin_type']['plugin_items'];
  }
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

  $all_plugins = drulenium_test_plugin_load();
  $options = array();
  foreach ( $all_plugins as $plugin ) {
    $options[$plugin['name']] = $plugin['description'];
  }

  $form['test_plugin_type'] = array(
      '#type' => 'fieldset',
      '#title' => t('Plugins'),
      '#description' => t('Add plugins by clicking the buttons below.'),
      '#collapsible' => TRUE,
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="settings-fieldset-wrapper">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
  );

  foreach ( $all_plugins as $type => $plugin ) {
    if (empty($form_state['num_of_add_plugin_'.$type])) {
      $form_state['num_of_add_plugin_'.$type] = 0;
    }
    $form['test_plugin_type']["add_plugin_$type"] = array(
        '#type' => 'submit',
        '#value' => $plugin['title'],
        '#suffix' => ' &nbsp;',
        '#submit' => array('drulenium_plugin_add_more_add_one'),
        // See the examples in dynamic_fieldable_content_settings.module for more details on the
        // properties of #ajax.
        '#ajax' => array(
            'callback' => 'drulenium_plugin_add_more_callback',
            'wrapper' => 'settings-fieldset-wrapper',
        ),
    );
  }

  $form['plugin_items']['#tree'] = TRUE;
  $plugin_ctr = 0;
  if(isset($export->settings['plugins'])) {
    foreach ($export->settings['plugins'] as $type_and_num => $plugin_arr) {
      $plugin_ctr++;
      $plugin_type = $plugin_arr["type"];
      $plugin_weight = $plugin_arr["weight"];
      $plugin_name = $all_plugins[$plugin_type]['title'];
      $form['test_plugin_type']['plugin_items'][$type_and_num] = array(
          // We'll use a form element of type '#markup' to display the item name.
          'name' => array(
              '#markup' => $plugin_name,
              '#prefix' => '<span class="element-invisible">',
              '#suffix' => '</span>',
          ),
          'type' => array('#type' => 'value', '#value' => $plugin_type),

          // The 'weight' field will be manipulated as we move the items around in
          // the table using the tabledrag activity.  We use the 'weight' element
          // defined in Drupal's Form API.
          'weight' => array(
              '#type' => 'weight',
              '#title' => t('Weight'),
              '#default_value' => $plugin_weight,
              '#delta' => 30,
              '#attributes' => array(
                  'class' => array(
                      'plugin-item-weight',
                      'plugin-weight-' . $type_and_num,
                      'element-invisible',
                  ),
              ),
              '#title_display' => 'invisible',
          ),
      );
      // Drulenium attrs
      $form['test_plugin_type']['plugin_items'][$type_and_num]['description'] = array(
          '#type' => 'fieldset',
          '#title' => 'Plugin attributes',
          '#attributes' => array(
            'class' => array(
                'element-invisible',
            ),
          ),
      );
      $form['test_plugin_type']['plugin_items'][$type_and_num]['weight'] = form_process_weight($form['test_plugin_type']['plugin_items'][$type_and_num]['weight']);

      $form['test_plugin_type']['plugin_items'][$type_and_num]['description'] += drulenium_test_plugin_fb_settings_form($plugin_type, $plugin_arr["description"]);

      //Add classes for javascript handling
      foreach (element_children($form['test_plugin_type']['plugin_items'][$type_and_num]['description']) as $ele_id) {
        $form['test_plugin_type']['plugin_items'][$type_and_num]['description'][$ele_id]['#attributes']['class'] = array("plugin-type-$plugin_type", "plugin-num-$plugin_ctr", "plugin-setting-$ele_id");
      }
    }
    $form['test_plugin_type']['mkup'] = array(
        '#markup' => drulenium_presets_ui_form_table_theme($form),
    );
  }

  // TODO: Make local selenium addon testing work with multiple plugins
  /*$form['test_plugin_type']['plugin_type'] = array(
      '#title' => t('type'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => !empty($export->plugin_type) ? $export->plugin_type : 'drulenium_facepile',
      '#description' => t('Description for this preset.')
  );

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

    // drupal settings
    $form['drulenium_settings_' . $type]['settings_' . $type] = array(
        '#type' => 'fieldset',
        '#title' => 'Test settings',
        '#collapsible' => TRUE
    );

    //$form['drulenium_settings_' . $type]['settings_' . $type] += drulenium_test_plugin_drupal_settings_form($type, $export->settings);


    $drulenium_attrs_form = drulenium_test_plugin_drupal_settings_form($type, $export->settings);

    foreach ( $drulenium_attrs_form as $k => $v ) {
      $form['drulenium_settings_' . $type]['settings_' . $type][$k] = $v;
    }

    $form['drulenium_settings_' . $type]['settings_' . $type] += _drulenium_presets_ui_create_block_form($export);
    //dsm($export);
  }*/
  // TODO: Remove hardcoded unused plugin type
  $type = 'drulenium_facepile';
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
  // drupal settings
  /*$form['drulenium_settings_' . $type]['settings_' . $type] = array(
      '#type' => 'fieldset',
      '#collapsible1' => TRUE
  );
  $form['drulenium_settings_' . $type]['settings_' . $type] += _drulenium_presets_ui_create_block_form($export);*/
  $form['drulenium_settings_' . $type] += _drulenium_presets_ui_create_block_form($export);
  $form['#attached']['js'] = array(
      drupal_get_path('module', 'drulenium') . '/js/drulenium_preset_form.js',
  );
  $form['#attached']['css'] = array(
      drupal_get_path('module', 'drulenium') . '/css/drulenium_preset_form.css',
  );
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function drulenium_plugin_add_more_add_one($form, &$form_state) {
  $clicked_plugin_type = end($form_state['triggering_element']['#parents']);
  //remove add_plugin_ string from $clicked_plugin_type
  $plugin_type = substr($clicked_plugin_type, 11);
  // Count existing plugins of this type
  $ctr = 0;
  $plugin_keys = array_keys($form_state['item']->settings['plugins']);
  foreach ($plugin_keys as $plugin_type_num) {
    if(_drulenium_startsWith(plugin_type_num, plugin_type)) {
      $ctr++;
    }
  }
  $form_state['item']->settings['plugins'][$plugin_type."_{$ctr}"] = array('type' => $plugin_type, 'weight' => 0, 'description' => array());
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function drulenium_plugin_add_more_callback($form, $form_state) {
  return $form['test_plugin_type'];
}

function _drulenium_presets_ui_create_block_form($export) {
  $form = array();
  /* $form['block'] = array(
   '#type' => 'checkbox',
      '#title' => t('Create a block'),
      '#description' => t('Create a drupal block that contains this plugin.  You have to enable the block manually to show up.'),

      '#default_value' => isset($export->settings['block']) ? $export->settings['block'] : 0
  );*/
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

  $user_types = user_roles(FALSE);
  $form['test_as_roles'] = array(
      '#type' => 'radios',
      '#title' => t('Test as specific user roles'),
      '#options' => $user_types,
      '#description' => t('Test as users that belong to the above user role.'),
      '#default_value' => isset($export->settings['test_as_roles']) ? $export->settings['test_as_roles'] : 1,
  );
  return $form;
}

