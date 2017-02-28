<?php
/**
 * @file
 * from_123_nodes_roles_profiles_taxonomy.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function from_123_nodes_roles_profiles_taxonomy_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_granted|node|school|form';
  $field_group->group_name = 'group_granted';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'school';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Institutes/Consultants granted',
    'weight' => '47',
    'children' => array(
      0 => 'field_institutes',
      1 => 'field_consultants',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_granted|node|school|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_schools|node|institute|form';
  $field_group->group_name = 'group_schools';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'institute';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Schools serving',
    'weight' => '15',
    'children' => array(
      0 => 'field_school_related',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Schools serving',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-schools field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_schools|node|institute|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Institutes/Consultants granted');
  t('Schools serving');

  return $field_groups;
}
