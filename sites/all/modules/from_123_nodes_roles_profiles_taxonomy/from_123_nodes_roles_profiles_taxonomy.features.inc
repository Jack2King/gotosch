<?php
/**
 * @file
 * from_123_nodes_roles_profiles_taxonomy.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function from_123_nodes_roles_profiles_taxonomy_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function from_123_nodes_roles_profiles_taxonomy_node_info() {
  $items = array(
    'author' => array(
      'name' => t('Author'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'book' => array(
      'name' => t('Book'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'institute' => array(
      'name' => t('Institute'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('机构名称'),
      'help' => '',
    ),
    'post' => array(
      'name' => t('Post'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Post Title'),
      'help' => '',
    ),
    'school' => array(
      'name' => t('School'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'zoom_video_conference' => array(
      'name' => t('Zoom video conference'),
      'base' => 'node_content',
      'description' => t('A meeting, Zoom video conference integration'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_default_profile2_type().
 */
function from_123_nodes_roles_profiles_taxonomy_default_profile2_type() {
  $items = array();
  $items['consultant'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "consultant",
    "label" : "Consultant",
    "weight" : "4",
    "data" : { "registration" : 0, "use_one_page" : 1 }
  }');
  $items['interests'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "interests",
    "label" : "Interests",
    "weight" : "0",
    "data" : { "registration" : false }
  }');
  $items['main'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "main",
    "label" : "Main profile",
    "weight" : "0",
    "data" : { "registration" : 0, "use_one_page" : 1 }
  }');
  $items['parent'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "parent",
    "label" : "Parent",
    "weight" : "3",
    "data" : { "registration" : 0, "use_one_page" : 1 }
  }');
  $items['recommendations'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "recommendations",
    "label" : "Recommendations",
    "weight" : "5",
    "data" : { "registration" : 0, "use_one_page" : 1 }
  }');
  $items['student'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "student",
    "label" : "Student",
    "weight" : "2",
    "data" : { "registration" : 0, "use_one_page" : 1 }
  }');
  $items['teacher'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "teacher",
    "label" : "Teacher",
    "weight" : "1",
    "data" : { "registration" : 0, "use_one_page" : 1 }
  }');
  return $items;
}
