<?php

/**
 * @file mouseover.inc
 * 
 * Main (ctools) plugin file for "mouseover" plugin type
 */

$plugin = array(
  'title' => t('Hover over'),
  'description' => t('Drulenium Hover over plugin'),
);

function drulenium_mouseover_defaults() {
  return array(
    'locator' => '',
  );
}

function drulenium_mouseover_fb_settings($options) {
  $form = array();
  $form['locator'] = array(
    '#type' => 'textfield',
    '#title' => t('Element Locator'),
    '#description' => t('The Locator of the element you want to hover. CSS selector like {#food span.dairy}')
  );

  $defaults = drulenium_mouseover_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
