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
