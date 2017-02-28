<?php
/**
 * @file
 * from_123_nodes_roles_profiles_taxonomy.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function from_123_nodes_roles_profiles_taxonomy_taxonomy_default_vocabularies() {
  return array(
    'consulting' => array(
      'name' => 'Consulting',
      'machine_name' => 'consulting',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'religious_affiliation' => array(
      'name' => 'Religious Affiliation',
      'machine_name' => 'religious_affiliation',
      'description' => 'Religious Affiliation of schools',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'us_school_association' => array(
      'name' => 'US School Association',
      'machine_name' => 'us_school_association',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
