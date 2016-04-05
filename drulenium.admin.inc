<?php

/**
 * @file drulenium_ctools_export_ui.inc
 * 
 * Settings form for global drulenium customization
 */
function drulenium_admin_settings_form($form, &$form_state) {
  $form = array();
  $form['drulenium_test_users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test Users'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $user_types = user_roles(TRUE);
  foreach ($user_types as $rid => $role_name){
    $users = array();
    if($rid == 2){
      $result = db_query(
        'select u.uid, u.name from {users} u WHERE u.uid <> 0 AND u.status = 1 ORDER by u.name ASC');
    }else{
      $result = db_query(
        'select ur.uid, u.name from {users_roles} ur left join {users} u on u.uid = ur.uid WHERE ur.rid = :role_id AND u.status = 1 ORDER by u.name ASC',
        array(':role_id' => $rid)
      );
    }
    foreach ($result as $row) {
      $users[$row->uid] = $row->name;
    }
    $form['drulenium_test_users']['drulenium_role_'.$rid] = array(
       '#type' => 'select',
       '#title' => t($role_name),
       '#options' => $users,
       '#default_value' => variable_get('drulenium_role_'.$rid, ''),
   );
  }

  $form = system_settings_form($form);
  return $form;
}
