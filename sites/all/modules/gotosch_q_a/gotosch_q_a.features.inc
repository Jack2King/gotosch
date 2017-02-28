<?php
/**
 * @file
 * gotosch_q_a.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function gotosch_q_a_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function gotosch_q_a_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}


/**
 * Implements hook_node_info().
 */
function gotosch_q_a_node_info() {
  $items = array(
    'answer' => array(
      'name' => t('Answer'),
      'base' => 'node_content',
      'description' => t('Use answers for responses to Question content, which can be rated by user votes.'),
      'has_title' => '1',
      'title' => t('Title'),
      'help' => '',
    ),
    'question' => array(
      'name' => t('Question'),
      'base' => 'node_content',
      'description' => t('Ask a question to receive answers from other community members'),
      'has_title' => '1',
      'title' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
