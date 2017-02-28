<?php
/**
 * @file
 * from_123_nodes_roles_profiles_taxonomy.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function from_123_nodes_roles_profiles_taxonomy_user_default_roles() {
  $roles = array();

  // Exported role: coach.
  $roles['coach'] = array(
    'name' => 'coach',
    'weight' => 7,
  );

  // Exported role: consultant.
  $roles['consultant'] = array(
    'name' => 'consultant',
    'weight' => 11,
  );

  // Exported role: parent.
  $roles['parent'] = array(
    'name' => 'parent',
    'weight' => 12,
  );

  // Exported role: staff.
  $roles['staff'] = array(
    'name' => 'staff',
    'weight' => 10,
  );

  // Exported role: student.
  $roles['student'] = array(
    'name' => 'student',
    'weight' => 6,
  );

  // Exported role: teacher.
  $roles['teacher'] = array(
    'name' => 'teacher',
    'weight' => 5,
  );

  return $roles;
}
