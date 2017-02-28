<?php
/**
 * @file
 * gotosch_bw.features.inc
 */

/**
 * Implements hook_ctools_plugin_directory().
 */
function gotosch_bw_ctools_plugin_directory($module, $plugin) {
  if ($module == 'ctools' && $plugin == 'content_types') {
    return 'plugins/' . $plugin;
  }
}

/**
 * Implements hook_views_api().
 */
function gotosch_bw_views_api() {
  return array(
    'api' => 3,
    'path' => drupal_get_path('module', 'gotosch_bw') . '/includes/views',
  );
}
