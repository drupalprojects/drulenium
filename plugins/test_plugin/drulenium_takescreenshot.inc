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
  );
}

function drulenium_takescreenshot_fb_settings($options) {
  $form = array();
  $form['locator'] = array(
    '#type' => 'textfield',
    '#title' => t('Element Locator'),
    '#description' => t('The Locator of the element you want to take screenshot. CSS selector like {#food span.dairy}')
  );

  $defaults = drulenium_takescreenshot_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
