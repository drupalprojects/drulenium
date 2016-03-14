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
    'locator_type' => 'css',
  );
}

function drulenium_mouseover_fb_settings($options) {
  $form = array();
  $form['locator_type'] = array(
    '#type' => 'select',
    '#title' => t('Locator Type'),
    '#options' => array(
      'id' => t('ID'),
      'name' => t('Name'),
      'css' => t('CSS'),
      'xpath' => t('XPath'),
    ),
    '#description' => t('Type of the Locator of the element you want to hover.<br/> Example CSS selector like {#food span.dairy}'),
  );
  $form['locator'] = array(
    '#type' => 'textfield',
    '#title' => t('Element Locator'),
    '#description' => t('The Locator of the element you want to hover.')
  );

  $defaults = drulenium_mouseover_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
