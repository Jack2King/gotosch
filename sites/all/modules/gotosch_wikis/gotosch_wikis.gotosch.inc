<?php
/**
 * @file
 * gotosch module integration for the gotosch Wikis module.
 */



/**
 * Implements hook_gotosch_bw_group_widget().
 */
function gotosch_wikis_gotosch_bw_group_widget() {
  return array(
    'gotosch_wikis' => array(
      'title' => t('Wikis'),
      'type' => 'view',
      'vid' => 'gotosch_bw_wikis',
      'display' => 'default',
      'weight' => 8,
      'bundle' => 'wiki',
    ),
  );
}

/**
 * Implements hook_gotosch_bw_create_all_widget().
 */
function gotosch_wikis_gotosch_bw_create_all_widget($group) {
  if (user_access('create wiki content')) {
    $link = l(t('Start a wiki'), 'node/add/wiki',
      array('attributes' => array('class' => 'gotosch-wikis-create'), 'query' => array('field_related' => $group->nid))
    );
    return array(
      'gotosch_wikis' => array(
        'link' => $link,
        'text' => t('Collaborate on a document'),
        '#weight' => 8,
      ),
    );
  }
}
