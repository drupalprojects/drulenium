<?php

function drulenium_hosting_client_settings_form($form, &$form_state) {
 $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remote Drulenium Server'),
    '#description' => t('Configuration for Remote Drulenium server.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $site_email = variable_get('site_mail', '');
  $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_release_notify_email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#description' => t('Email to Notify on completion.'),
    '#default_value' => variable_get('drulenium_vr_release_notify_email', $site_email),
  );
  $form['drulenium_vr_config']['drulenium_vr_config_server']['drulenium_vr_remote_drulenium_config']['drulenium_vr_api_secret'] = array(
      '#title' => t('API Secret'),
      '#type' => 'textfield',
      '#description' => t('API Secret key of your account in drulenium.org'),
      '#default_value' => variable_get('drulenium_vr_api_secret', ''),
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
