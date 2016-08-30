<?php

function drulenium_hosting_client_settings_form($form, &$form_state) {
  $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remote Drulenium.org Cloud Server'),
    '#description' => t('Configuration for Remote Drulenium.org Cloud server.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_api_secret'] = array(
    '#title' => t('API Secret'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('API Secret key of your account in drulenium.org. Register for free & get your API key at <a href="http://drulenium.org/user/register">http://drulenium.org/user/register</a>'),
    '#default_value' => variable_get('drulenium_vr_api_secret', ''),
  );

  $site_email = variable_get('site_mail', '');
  $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_release_notify_email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#description' => t('Email to Notify on completion.'),
    '#default_value' => variable_get('drulenium_vr_release_notify_email', $site_email),
  );

  $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_project_uuid'] = array(
      '#title' => t('Project UUID'),
      '#type' => 'textfield',
      '#description' => t('UUID of your project in drulenium.org'),
      '#default_value' => variable_get('drulenium_vr_project_uuid', ''),
  );

  drulenium_visual_regression_drulenium_browser_settings_form($form);

  $form = system_settings_form($form);
  return $form;
}

/*
 * Default: Firefox - latest version
 */
function drulenium_visual_regression_drulenium_browser_settings_form(&$form) {
  $cache_bin = "cache";
  $cid = "drulenium_drulenium_browsers";
  if ($cache = cache_get($cid, $cache_bin)) {
    $cache_data = $cache->data;
    $all_browsers = drupal_json_decode($cache_data);
  } else {
    $browsers_api_url = "http://www.drulenium.org/api/browsers.json";
    $resp = drupal_http_request($browsers_api_url);
    $cache_data = $resp->data;
    if(($resp->code == 200) && !empty($cache_data)) {
      $all_browsers = drupal_json_decode($cache_data);
      cache_set($cid, $cache_data, $cache_bin, SAUCE_CACHE_EXPIRE_TIME);
    }
  }
  if(!empty($all_browsers)) {
    $default_os_browser_version = array(
        "os" => 'OS X -> El Capitan',
        "browser" => 'firefox',
        "browser_version" => '47.0',
    );
    $os_options = array();
    $browser_options = array();
    $browser_version_options = array();
    foreach ($all_browsers as $browser_array) {
      $short_version = $browser_array['browser_version'];
      $browser_api_name = $browser_array['browser'];
      $os = "{$browser_array['os']} -> {$browser_array['os_version']}";

      $os_options[$os] = $os;
      $browser_options[$os][$browser_api_name] = $browser_api_name;
      $browser_version_options[$os][$browser_api_name][$short_version] = $short_version;
    }
    asort($os_options);
    $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_drulenium_browsers'] = array(
        '#type' => 'fieldset',
        '#title' => t('Browsers'),
        '#description' => t('Configuration for taking screenshots using drulenium.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_drulenium_browsers']['drulenium_vr_drulenium_os'] = array(
        '#type' => 'select',
        '#title' => t('OS'),
        '#options' => $os_options,
        '#default_value' => variable_get('drulenium_vr_drulenium_os', $default_os_browser_version['os']),
        '#description' => t('Pick an operating system for the device to test.'),
    );
    foreach ($os_options as $os) {
      asort($browser_options[$os]);
      $os_id = drulenium_visual_regression_name_to_id($os);
      $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_drulenium_browsers']['drulenium_vr_drulenium_browser_'.$os_id] = array(
          '#type' => 'select',
          '#title' => t('Browser'),
          '#options' => $browser_options[$os],
          '#default_value' => variable_get('drulenium_vr_drulenium_browser_'.$os_id, $default_os_browser_version['browser']),
          '#description' => t('Pick a browser.'),
          '#states' => array(
              'visible' => array(
                  ':input[name="drulenium_vr_drulenium_os"]' => array('value' => $os),
              ),
          ),
      );
      foreach ($browser_options[$os] as $browser) {
        // Sort versions in descending order
        arsort($browser_version_options[$os][$browser]);
        $browser_id = drulenium_visual_regression_name_to_id($browser);
        $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_drulenium_browsers']['drulenium_vr_drulenium_browser_version_'."{$os_id}_{$browser_id}"] = array(
            '#type' => 'select',
            '#title' => t('Version'),
            '#options' => $browser_version_options[$os][$browser],
            '#default_value' => variable_get('drulenium_vr_drulenium_browser_version_'."{$os_id}_{$browser_id}", $default_os_browser_version['browser_version']),
            '#description' => t('Pick a version.'),
            '#states' => array(
                'visible' => array(
                    ':input[name="drulenium_vr_drulenium_os"]' => array('value' => $os),
                    ':input[name="drulenium_vr_drulenium_browser_'.$os_id.'"]' => array('value' => $browser),
                ),
            ),
                );
      }
    }
  }
}
