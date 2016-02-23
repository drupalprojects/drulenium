<?php

/**
 * @file takescreenshot.inc
 * 
 * Main (ctools) plugin file for "takescreenshot" plugin type
 */

$plugin = array(
  'title' => t('Take screenshot'),
  'description' => t('Drulenium Take screenshot plugin'),
);

function drulenium_takescreenshot_defaults() {
  return array(
    'locator' => '',
    'snapshot_mode' => 'include',
    'width' => '0',
    'height' => '0',
    'offset_width' => '0',
    'offset_height' => '0',
  );
}

function drulenium_takescreenshot_fb_settings($options) {
  $form = array();
  $form['snapshot_mode'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'include' => t('Include'),
      'exclude' => t('Exclude'),
    ),
    '#description' => t('Choose "Include" to take the snapshot of only the specified HTML element.<br/>Choose "Exclude" to take snapshot of the whole page except the specified HTML elements.'),
  );
  $form['locator'] = array(
    '#type' => 'textfield',
    '#title' => t('Element Locator'),
    '#description' => t('The Locator of the element you want to include or exclude in the screenshot. CSS selector like {#food span.dairy}.<br/>Multiple selectors can be coma seperated for exclusion.')
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Width for the element. To override the automatically calculated width of the element.')
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Height for the element. To override the automatically calculated height of the element.')
  );
  $form['offset_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width Offset'),
    '#description' => t('Extra width for the element. Negative values(like -10) for subtracting from the automatically calculated width of the element.')
  );
  $form['offset_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height Offset'),
    '#description' => t('Extra height for the element. Negative values(like -10) for subtracting from the automatically calculated height of the element.')
  );

  $defaults = drulenium_takescreenshot_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
