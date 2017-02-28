<?php
/**
 * @file
 * gotosch module integration for the gotosch Events module.
 */


/**
 * Implements hook_gotosch_bw_create_all_widget().
 */
function gotosch_events_gotosch_bw_create_all_widget($group) {
  if (og_user_access('node', $group->nid, 'create event content')) {
    $link = l(t('Create an event'), 'node/add/calendar-event',
      array(
        'attributes' => array('class' => 'gotosch-events-create'),
        'query' => array('og_group_ref' => $group->nid),
      )
    );

    return array(
      'gotosch_events' => array(
        'link' => $link,
        'text' => t('Create in-person or online meetups'),
        '#weight' => 6,
      ),
    );
  }
}
