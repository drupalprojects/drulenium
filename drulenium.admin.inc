<?php

/**
 * @file drulenium_ctools_export_ui.inc
 * 
 * Settings form for global drulenium custimazation
 */
function drulenium_admin_settings_form($form, &$form_state) {
  $languages = language_list();

  $form = array();
  $form['drulenium_appid'] = array(
    '#title' => t('Base URL'), 
    '#type' => 'textfield', 
    '#description' => t('<p> Site Base URL if different from current site.</p>'), 
    '#default_value' => variable_get('drulenium_appid', '') 
  );

  $form = system_settings_form($form);
  return $form;
}

function _get_facebook_locale() {
  $fb_locale = array();
  $cache = cache_get('drulenium');
  if (empty($cache->data)) {
    $result = drupal_http_request("https://www.facebook.com/translations/FacebookLocales.xml");
    $xml = simplexml_load_string($result->data);
    foreach ( $xml->locale as $line ) {
      $representation = ( array ) $line->codes->code->standard->representation;
      $name = ( array ) $line->englishName;
      $key = $representation[0];
      $value = $name[0];
      $fb_locale[$key] = $value;
    }
    cache_set('drulenium', serialize($fb_locale), 'cache');
  }
  else {
    $fb_locale = unserialize($cache->data);
  }
  return $fb_locale;
}
