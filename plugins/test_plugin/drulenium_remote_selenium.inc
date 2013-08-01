<?php

/**
 * @file selenium.inc
 *
 * Main (ctools) plugin file for "selenium" plugin type
 */

$plugin = array(
    'title' => t('Remote Selenium Test'),
    'description' => t('Remote Selenium Test plugin'),
    'html tag name' => 'selenium'
)
;

function drulenium_remote_selenium_defaults() {
  return array(
      'text' => '',
      'size' => 'small',
      'pages' => '',
      'numrows' => 1,
      'width' => 200,
      'colorscheme' => 'light' ,
  );
}

function drulenium_remote_selenium_fb_settings($options) {

  $form = array();
  /*
   $form['text'] = array(
       '#type' => 'textfield',
       '#title' => t('Text'),
       '#description' => t('The Text you want to Verify.')
   );
  */
  // Use the #managed_file FAPI element to upload an image file.
  $form['selenium_remote_file'] = array(
      '#title' => t('Remote Selenium Test Case URL'),
      '#type' => 'textfield',
      '#description' => t('Put the URL of the selenium test case here'),
      '#default_value' => variable_get('selenium_remote_file', ''),
  );
  //$form['selenium_file']['#upload_validators']['file_validate_extensions'][0] = null;
  //$defaults = drulenium_selenium_defaults();
  /*
  foreach ( $form as $id => $f ) {
  $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  */
  return $form;

}

function drulenium_remote_selenium_drupal_settings($options) {
  return array();
}
