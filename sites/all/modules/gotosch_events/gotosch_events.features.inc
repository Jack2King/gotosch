<?php
/**
 * @file
 * gotosch_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function gotosch_events_ctools_plugin_api($module = NULL, $api = NULL) {
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
function gotosch_events_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
 /*
function gotosch_events_node_info() {
  $items = array(
    'calendar_event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('Post information about planned activities or meetings.'),
      'has_title' => '1',
      'title' => t('Event title'),
      'help' => '',
    ),
  );
  return $items;
}
*/
/**
 * Implements hook_default_registration_type().
 */
function gotosch_events_default_registration_type() {
  $items = array();
  $items['calendar_event'] = entity_import('registration_type', '{
    "name" : "calendar_event",
    "label" : "Calendar Event",
    "locked" : "0",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_rdf_default_mappings().
 */
function gotosch_events_rdf_default_mappings() {
  $schemaorg = array();

  // Exported RDF mapping: event
  $schemaorg['node']['calendar_event'] = array(
    'rdftype' => array(
      0 => 'schema:Event',
      1 => 'sioc:Item',
      2 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),

    'opigno_calendar_date' => array(
      'predicates' => array(
        0 => 'schema:startDate',
      ),
    ),
    'url' => array(
      'predicates' => array(
        0 => 'schema:url',
      ),
      'type' => 'rel',
    ),
    'field_location' => array(
      'predicates' => array(),
    ),
    'field_address' => array(
      'predicates' => array(
        0 => 'schema:location',
      ),
    ),
  );

  return $schemaorg;
}
