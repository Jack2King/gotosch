<?php
/**
 * @file
 * from_123_nodes_roles_profiles_taxonomy.features.og_features_role.inc
 */

/**
 * Implements hook_og_features_default_roles().
 */
function from_123_nodes_roles_profiles_taxonomy_og_features_default_roles() {
  $roles = array();

  // Exported OG Role: 'node:class:student'.
  $roles['node:class:student'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'class',
    'name' => 'student',
  );

  // Exported OG Role: 'node:class:teacher'.
  $roles['node:class:teacher'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'class',
    'name' => 'teacher',
  );

  // Exported OG Role: 'node:institute:consultant'.
  $roles['node:institute:consultant'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'institute',
    'name' => 'consultant',
  );

  // Exported OG Role: 'node:institute:manager'.
  $roles['node:institute:manager'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'institute',
    'name' => 'manager',
  );

  // Exported OG Role: 'node:institute:student'.
  $roles['node:institute:student'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'institute',
    'name' => 'student',
  );

  // Exported OG Role: 'node:school:consultant'.
  $roles['node:school:consultant'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'school',
    'name' => 'consultant',
  );

  // Exported OG Role: 'node:school:manager'.
  $roles['node:school:manager'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'school',
    'name' => 'manager',
  );

  // Exported OG Role: 'node:school:student'.
  $roles['node:school:student'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'school',
    'name' => 'student',
  );

  // Exported OG Role: 'node:school:teacher'.
  $roles['node:school:teacher'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'school',
    'name' => 'teacher',
  );

  return $roles;
}
