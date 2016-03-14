<?php

/**
 * @file filltextbox.inc
 * 
 * Main (ctools) plugin file for "filltextbox" plugin type
 */

$plugin = array(
  'title' => t('Fill textbox'),
  'description' => t('Drulenium Fill textbox plugin'),
);

function drulenium_filltextbox_defaults() {
  return array(
    'text' => '', 
    'locator' => '',
    'locator_type' => 'css',
  );
}

function drulenium_filltextbox_fb_settings($options) {
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
    '#description' => t('Type of the Locator of the element you want to fill with text.<br/> Example CSS selector like {#food span.dairy}'),
  );
  $form['locator'] = array(
      '#type' => 'textfield',
      '#title' => t('Element Locator'),
      '#description' => t('The Locator of the element you want to fill with text.')
  );
  $form['text'] = array(
      '#type' => 'textfield', 
      '#title' => t('Text'), 
      '#description' => t('The Text you want to put in textbox.') 
  );

  $defaults = drulenium_filltextbox_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
