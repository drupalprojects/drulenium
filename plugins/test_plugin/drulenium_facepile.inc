<?php

/**
 * @file facepile.inc
 *
 * Main (ctools) plugin file for "facepile" plugin type
 */

$plugin = array(
    'title' => t('Verify Text'),
    'description' => t('Drulenium Verify Text plugin'),
)
;

function drulenium_facepile_defaults() {
  return array(
      'text' => '',
      'locator_type' => 'css',
      'locator' => '',
  );
}

function drulenium_facepile_fb_settings($options) {

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
    '#description' => t('Type of the Locator of the element you want to Verify.<br/> Example CSS selector like {#food span.dairy}'),
  );
  $form['locator'] = array(
      '#type' => 'textfield',
      '#title' => t('Element Locator'),
      '#description' => t('The Locator of the element you want to Verify.')
  );
  $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#description' => t('The Text you want to Verify.')
  );

  $defaults = drulenium_facepile_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
