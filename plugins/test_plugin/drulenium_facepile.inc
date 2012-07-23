<?php

/**
 * @file facepile.inc
 * 
 * Main (ctools) plugin file for "facepile" plugin type
 */

$plugin = array(
    'title' => t('Verify Text'), 
    'description' => t('Drulenium Verify Text plugin'), 
    'html tag name' => 'facepile' 
)
;

function drulenium_facepile_defaults() {
  return array(
      'text' => '', 
      'size' => 'small', 
      'pages' => '', 
      'numrows' => 1, 
      'width' => 200, 
      'colorscheme' => 'light' 
  );
}

function drulenium_facepile_fb_settings($options) {
  
  $form = array();
  
  $form['text'] = array(
      '#type' => 'textfield', 
      '#title' => t('Text'), 
      '#description' => t('The Text you want to Verify.') 
  );
  
  $form['pages'] = array(
      '#type' => 'textarea', 
      '#title' => t('Pages'), 
      '#description' => t('The pages you want to Test') 
  );
  
 
  $defaults = drulenium_facepile_defaults();
  
  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  
  return $form;

}

function drulenium_facepile_drupal_settings($options) {
  return array();
}
