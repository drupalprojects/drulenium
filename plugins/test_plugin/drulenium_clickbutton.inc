<?php

/**
 * @file clickbutton.inc
 * 
 * Main (ctools) plugin file for "clickbutton" plugin type
 */

$plugin = array(
  'title' => t('Click button'),
  'description' => t('Drulenium Click button plugin'),
);

function drulenium_clickbutton_defaults() {
  return array(
    'locator' => '',
  );
}

function drulenium_clickbutton_fb_settings($options) {
  $form = array();
  $form['locator'] = array(
    '#type' => 'textfield',
    '#title' => t('Element Locator'),
    '#description' => t('The Locator of the element you want to click. CSS selector like {#food span.dairy}')
  );

  $defaults = drulenium_clickbutton_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
