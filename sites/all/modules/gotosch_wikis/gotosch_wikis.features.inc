<?php
/**
 * @file
 * gotosch_wikis.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function gotosch_wikis_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function gotosch_wikis_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function gotosch_wikis_node_info() {
  $items = array(
    'wiki' => array(
      'name' => t('Wiki'),
      'base' => 'node_content',
      'description' => t('Create a collaborative document, allowing users to add, delete, or revise content. Provides document version controls.'),
      'has_title' => '1',
      'title' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
