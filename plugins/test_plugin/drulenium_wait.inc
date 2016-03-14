<?php

/**
 * @file wait.inc
 * 
 * Main (ctools) plugin file for "wait" plugin type
 */

$plugin = array(
  'title' => t('Wait'),
  'description' => t('Drulenium Wait plugin'),
);

function drulenium_wait_defaults() {
  return array(
    'locator' => '',
    'seconds' => '3',
  );
}

function drulenium_wait_fb_settings($options) {
  $form = array();
  $form['locator'] = array(
    '#type' => 'textfield',
    '#title' => t('Element Locator'),
    '#description' => t('Wait for at most number of Seconds mentioned, for the specified Element to be present on the DOM of a page and visible. CSS selector like {#food span.dairy}')
  );
  $form['seconds'] = array(
    '#type' => 'textfield',
    '#title' => t('Seconds'),
    '#description' => t('number of seconds to wait.')
  );

  $defaults = drulenium_wait_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}
