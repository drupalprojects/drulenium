<?php

class drulenium_presets_ui extends ctools_export_ui {

  function hook_menu(&$items) {

    // Change the item to a tab on the drulenium configuration page.
    $this->plugin['menu']['items']['list callback']['type'] = MENU_LOCAL_TASK;
    $this->plugin['menu']['items']['list callback']['weight'] = 10;

    parent::hook_menu($items);
  }

  /**
   * Builds the operation links for a specific exportable item.
   */
  function build_operations($item) {
    $plugin = $this->plugin;
    $schema = ctools_export_get_schema($plugin['schema']);
    $operations = $plugin['allowed operations'];
    $operations['import'] = FALSE;

    if ($item->{$schema['export']['export type string']} == t('Normal')) {
      $operations['revert'] = FALSE;
    }
    elseif ($item->{$schema['export']['export type string']} == t('Overridden')) {
      $operations['delete'] = FALSE;
    }
    else {
      $operations['revert'] = FALSE;
      $operations['delete'] = FALSE;
    }
    if (empty($item->disabled)) {
      $operations['enable'] = FALSE;
    }
    else {
      $operations['disable'] = FALSE;
    }

    $allowed_operations = array();
    /*
     * Adding operation to run the test.
     */
    $allowed_operations['run'] = array(
      'title' => 'Run',
      'href' => 'drulenium/vr/'.$item->oid,
    );
    foreach ($operations as $op => $info) {
      if (!empty($info)) {
        $allowed_operations[$op] = array(
          'title' => $info['title'],
          'href' => ctools_export_ui_plugin_menu_path($plugin, $op, $item->{$this->plugin['export']['key']}),
        );
        if (!empty($info['ajax'])) {
          $allowed_operations[$op]['attributes'] = array('class' => array('use-ajax'));
        }
        if (!empty($info['token'])) {
          $allowed_operations[$op]['query'] = array('token' => drupal_get_token($op));
        }
      }
    }

    return $allowed_operations;
  }
  
}
