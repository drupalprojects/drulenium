<?php

/**
 * @file
 * Administration cleanup page callbacks for the drulenium visual regression module.
 */

function drulenium_visual_regression_cache_settings_form($form, &$form_state) {
  $form = array();
  $form['clear_unused_images'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear Unused images'),
    '#default_value' => 0,
    '#description' => t("Check the box, If you want to delete Unused images.")
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Clear'),
  );
  return $form;
}

function drulenium_visual_regression_local_settings_form($form, &$form_state) {
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
  $form['drulenium_vr_release_imagemagick_path'] = array(
      '#title' => t('Path to the Image Magick binary'),
      '#type' => 'textfield',
      '#description' => t('The complete path of the ImageMagick <kbd>convert</kbd> binary. For example: <kbd>/usr/bin</kbd> or <kbd>C:\Program Files\ImageMagick-6.3.4-Q16</kbd>. Run "which" command in linux terminal to get the path, like "which convert"'),
      '#default_value' => variable_get('drulenium_vr_release_imagemagick_path', "/opt/local/bin"),
  );

  $form = system_settings_form($form);
  return $form;
}

function drulenium_visual_regression_browserstack_settings_form($form, &$form_state) {
  $form['browserstack_username'] = array(
      '#type' => 'textfield',
      '#title' => t('BrowserStack Username'),
      '#description' => t('Your BrowserStack account username.'),
      '#default_value' => variable_get('browserstack_username', ''),
      '#required' => TRUE,
  );
  $form['browserstack_access_key'] = array(
      '#type' => 'password',
      '#title' => t('BrowserStack Access Key'),
      '#description' => t('Your BrowserStack account access key.<br/> Access Key is stored in plain text in the database.'),
      '#default_value' => variable_get('browserstack_access_key', ''),
      '#required' => TRUE,
  );
  $form['browserstack_debugging'] = array(
      '#type' => 'radios',
      '#title' => t('Debugging'),
      '#options' => array(
        'false' => t('False'),
        'true' => t('True'),
      ),
      '#default_value' => variable_get('browserstack_debugging', 'false'),
      '#description' => t('Debugging to true, generates a live screencast and logs (text, visual, and raw).'),
  );

  $browser_stack_username = variable_get('browserstack_username', "");
  $browser_stack_access_key = variable_get('browserstack_access_key', "");
  $cache_bin = "cache";
  $cid = "drulenium_browserstack_browsers";
  if ($cache = cache_get($cid, $cache_bin)) {
    $cache_data = $cache->data;
    $all_browsers = drupal_json_decode($cache_data);
  } elseif (!empty($browser_stack_username) && !empty($browser_stack_access_key)) {
    $browsers_api_url = "https://{$browser_stack_username}:{$browser_stack_access_key}@www.browserstack.com/automate/browsers.json";
    $resp = drupal_http_request($browsers_api_url);
    $cache_data = $resp->data;
    if(($resp->code == 200) && !empty($cache_data)) {
      $all_browsers = drupal_json_decode($cache_data);
      cache_set($cid, $cache_data, $cache_bin, SAUCE_CACHE_EXPIRE_TIME);
    }
  }
  if(!empty($all_browsers)) {
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
    $form['drulenium_vr_browserstack_browsers'] = array(
        '#type' => 'fieldset',
        '#title' => t('Browsers'),
        '#description' => t('Configuration for taking screenshots using BrowserStack.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $form['drulenium_vr_browserstack_browsers']['drulenium_vr_browserstack_os'] = array(
        '#type' => 'select',
        '#title' => t('OS'),
        '#options' => $os_options,
        '#default_value' => variable_get('drulenium_vr_browserstack_os', ''),
        '#description' => t('Pick an operating system for the device to test.'),
    );
    foreach ($os_options as $os) {
      asort($browser_options[$os]);
      $os_id = drulenium_visual_regression_name_to_id($os);
      $form['drulenium_vr_browserstack_browsers']['drulenium_vr_browserstack_browser_'.$os_id] = array(
          '#type' => 'select',
          '#title' => t('Browser'),
          '#options' => $browser_options[$os],
          '#default_value' => variable_get('drulenium_vr_browserstack_browser_'.$os_id, ''),
          '#description' => t('Pick a browser.'),
          '#states' => array(
              'visible' => array(
                  ':input[name="drulenium_vr_browserstack_os"]' => array('value' => $os),
              ),
          ),
      );
      foreach ($browser_options[$os] as $browser) {
        asort($browser_version_options[$os][$browser]);
        $browser_id = drulenium_visual_regression_name_to_id($browser);
        $form['drulenium_vr_browserstack_browsers']['drulenium_vr_browserstack_browser_version_'."{$os_id}_{$browser_id}"] = array(
            '#type' => 'select',
            '#title' => t('Version'),
            '#options' => $browser_version_options[$os][$browser],
            '#default_value' => variable_get('drulenium_vr_browserstack_browser_version_'."{$os_id}_{$browser_id}", ''),
            '#description' => t('Pick a version.'),
            '#states' => array(
                'visible' => array(
                    ':input[name="drulenium_vr_browserstack_os"]' => array('value' => $os),
                    ':input[name="drulenium_vr_browserstack_browser_'.$os_id.'"]' => array('value' => $browser),
                ),
            ),
        );
      }
    }
  }

  $form = system_settings_form($form);
  return $form;
}

function drulenium_visual_regression_saucelabs_settings_form($form, &$form_state) {
  /*
   * Sauce Labs
  */
  $form['saucelabs_username'] = array(
      '#type' => 'textfield',
      '#title' => t('SauceLabs Username'),
      '#description' => t('Your Saucelabs account username.'),
      '#default_value' => variable_get('saucelabs_username', ''),
      '#required' => TRUE,
  );
  $form['saucelabs_api'] = array(
      '#type' => 'password',
      '#title' => t('SauceLabs Access Key'),
      '#description' => t('Your Saucelabs account access key.<br/> Access Key is stored in plain text in the database.'),
      '#default_value' => variable_get('saucelabs_api', ''),
      '#required' => TRUE,
  );
  $saucelabs_username = variable_get('saucelabs_username', '');
  $saucelabs_access_key = variable_get('saucelabs_api', '');
  $cache_bin = "cache";
  $cid = "drulenium_saucelabs_browsers";
  if ($cache = cache_get($cid, $cache_bin)) {
    $cache_data = $cache->data;
    $all_browsers = drupal_json_decode($cache_data);
  } elseif (!empty($saucelabs_username) && !empty($saucelabs_access_key) && ($library = libraries_load("sausage")) && !empty($library['loaded'])) {
    $s = new Sauce\Sausage\SauceAPI($saucelabs_username, $saucelabs_access_key);
    $all_browsers = $s->getAllBrowsers();
    $cache_data = drupal_json_encode($all_browsers);
    cache_set($cid, $cache_data, $cache_bin, SAUCE_CACHE_EXPIRE_TIME);
  }
  if(!empty($all_browsers)) {
    $os_options = array();
    $browser_options = array();
    $browser_version_options = array();
    foreach ($all_browsers as $browser_array) {
      $short_version = $browser_array['short_version'];
      // TODO: Show long name in select box & use api name internally
      //$long_name = $browser_array['long_name'];
      $browser_api_name = $browser_array['api_name'];
      $os = $browser_array['os'];

      $os_options[$os] = $os;
      $browser_options[$os][$browser_api_name] = $browser_api_name;
      $browser_version_options[$os][$browser_api_name][$short_version] = $short_version;
    }
    asort($os_options);
    $form['drulenium_vr_saucelabs_browsers'] = array(
        '#type' => 'fieldset',
        '#title' => t('Browsers'),
        '#description' => t('Configuration for taking screenshots using Sauce Labs.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $form['drulenium_vr_saucelabs_browsers']['drulenium_vr_saucelabs_os'] = array(
        '#type' => 'select',
        '#title' => t('OS'),
        '#options' => $os_options,
        '#default_value' => variable_get('drulenium_vr_saucelabs_os', ''),
        '#description' => t('Pick an operating system for the device to test. Mac for iPad, Linux for Android...'),
    );
    foreach ($os_options as $os) {
      asort($browser_options[$os]);
      $os_id = drulenium_visual_regression_name_to_id($os);
      $form['drulenium_vr_saucelabs_browsers']['drulenium_vr_saucelabs_browser_'.$os_id] = array(
          '#type' => 'select',
          '#title' => t('Browser'),
          '#options' => $browser_options[$os],
          '#default_value' => variable_get('drulenium_vr_saucelabs_browser_'.$os_id, ''),
          '#description' => t('Pick a browser.'),
          '#states' => array(
              'visible' => array(
                  ':input[name="drulenium_vr_saucelabs_os"]' => array('value' => $os),
              ),
          ),
      );
      foreach ($browser_options[$os] as $browser) {
        asort($browser_version_options[$os][$browser]);
        $browser_id = drulenium_visual_regression_name_to_id($browser);
        $form['drulenium_vr_saucelabs_browsers']['drulenium_vr_saucelabs_browser_version_'."{$os_id}_{$browser_id}"] = array(
            '#type' => 'select',
            '#title' => t('Version'),
            '#options' => $browser_version_options[$os][$browser],
            '#default_value' => variable_get('drulenium_vr_saucelabs_browser_version_'."{$os_id}_{$browser_id}", ''),
            '#description' => t('Pick a version.'),
            '#states' => array(
                'visible' => array(
                    ':input[name="drulenium_vr_saucelabs_os"]' => array('value' => $os),
                    ':input[name="drulenium_vr_saucelabs_browser_'.$os_id.'"]' => array('value' => $browser),
                ),
            ),
        );
      }
    }
  }

  $form = system_settings_form($form);
  return $form;
}

function drulenium_visual_regression_github_settings_form($form, &$form_state) {
  // Github Config
  $form['github_username'] = array(
      '#type' => 'textfield',
      '#title' => t('GitHub Username'),
      '#description' => t('Your GitHub account username.'),
      '#default_value' => variable_get('github_username', ''),
      '#required' => TRUE,
  );
  $form['github_password'] = array(
      '#type' => 'password',
      '#title' => t('GitHub Password'),
      '#description' => t('Your GitHub account password.<br/> Password is stored in plain text in the database.'),
      '#default_value' => variable_get('github_password', ''),
      '#required' => TRUE,
  );
  $form['github_email'] = array(
      '#type' => 'textfield',
      '#title' => t('GitHub Email'),
      '#description' => t('Your GitHub account email id.'),
      '#default_value' => variable_get('github_email', ''),
      '#required' => TRUE,
  );
  $form['github_repo'] = array(
      '#type' => 'textfield',
      '#title' => t('GitHub Repository'),
      '#description' => t('Your GitHub repository name.'),
      '#default_value' => variable_get('github_repo', ''),
      '#required' => TRUE,
  );

  $form = system_settings_form($form);
  return $form;
}

function drulenium_visual_regression_cache_settings_form_submit($form, &$form_state) {
  if($form_state['values']['clear_unused_images']){
    //Read the images directory
    //loop through each filename and check if it exists in database
    //Delete the file if it does not exist
    $mydir = drupal_realpath(file_default_scheme() . '://drulenium');
    $files = file_scan_directory($mydir, '/highlight_.*\.png$/');
    $count = 0;
    $delete_count = 0;
    foreach ($files as $file) {
      $file_name = $file->filename;
      $query = db_select('drulenium_vr_results', 'vr_re');
      $image_url = $query
        ->fields('vr_re', array('diff_highlight'))
        ->condition('vr_re.diff_highlight', "http://dev.proxy.drulenium.com/sites/dev.proxy.drulenium.com/files/drulenium/{$file_name}")
        ->execute()->fetchfield();
      if(empty($image_url)){
        //Delete the file
        $file_path = "{$mydir}/{$file_name}";
        $res = file_unmanaged_delete($file_path);
        if($res){
          $delete_count++;
        }
        $count++;
      }
    }
    drupal_set_message("Deleted {$delete_count} out of {$count} unused images.");
  }
}
