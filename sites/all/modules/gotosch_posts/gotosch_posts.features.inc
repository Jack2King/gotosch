<?php
/**
 * @file
 * gotosch_posts.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function gotosch_posts_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function gotosch_posts_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}


/**
 * Implements hook_node_info().
 */
function gotosch_posts_node_info() {
  $items = array(
    'post' => array(
      'name' => t('Post'),
      'base' => 'node_content',
      'description' => t('Start a conversation or share some information.'),
      'has_title' => '1',
      'title' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
