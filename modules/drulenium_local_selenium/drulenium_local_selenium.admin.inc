<?php

function drulenium_local_selenium_settings_form($form, &$form_state) {
 /*
  * Image styles for comparision
  * browser type, width, height https://github.com/facebook/php-webdriver/wiki/Example-command-reference
  * Selenium host url
  */
  $form['drulenium_vr_release_selenium_host'] = array(
      '#title' => t('Selenium host url'),
      '#type' => 'textfield',
      '#description' => t('Site URL to selenium host. Usually http://localhost:4444/wd/hub'),
      '#default_value' => variable_get('drulenium_vr_release_selenium_host', "http://localhost:4444/wd/hub"),
  );
 /* ChromeDriver
  * The path to the driver executable must be set by the webdriver.chrome.driver system property;
  * for more information, see http://code.google.com/p/selenium/wiki/ChromeDriver.
  * The latest version can be downloaded from http://chromedriver.storage.googleapis.com/index.html
  */
  $form['drulenium_vr_release_selenium_browser'] = array(
      '#type' => 'select',
      '#title' => t('Browser'),
      '#options' => array(
          'firefox' => t('firefox'),
          'chrome' => t('chrome'),
      ),
      '#default_value' => variable_get('drulenium_vr_release_selenium_browser', 'firefox'),
      '#description' => t('Pick a browser to test with.'),
  );
  $form['drulenium_vr_release_selenium_browser_width'] = array(
      '#title' => t('Browser Width'),
      '#type' => 'textfield',
      '#description' => t('Enter Integer values. Leave it blank to maximize the window'),
      '#default_value' => variable_get('drulenium_vr_release_selenium_browser_width', ""),
  );
  $form['drulenium_vr_release_selenium_browser_height'] = array(
      '#title' => t('Browser Height'),
      '#type' => 'textfield',
      '#description' => t('Enter Integer values. Leave it blank to maximize the window'),
      '#default_value' => variable_get('drulenium_vr_release_selenium_browser_height', ""),
  );
  $form['drulenium_vr_release_imagemagick_path'] = array(
      '#title' => t('Path to the Image Magick binary'),
      '#type' => 'textfield',
      '#description' => t('The complete path of the ImageMagick <kbd>convert</kbd> binary. For example: <kbd>/usr/bin</kbd> or <kbd>C:\Program Files\ImageMagick-6.3.4-Q16</kbd>. Run "which" command in linux terminal to get the path, like "which convert"'),
      '#default_value' => variable_get('drulenium_vr_release_imagemagick_path', "/opt/local/bin"),
  );

  $form = system_settings_form($form);
  return $form;
}
