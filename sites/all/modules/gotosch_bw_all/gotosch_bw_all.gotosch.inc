<?php
/**
 * @file
 * gotosch module integration for the gotosch Posts module.
 */

/**
 * Implements hook_gotosch_entity_integration().
 */
/*
function gotosch_bw_all_gotosch_entity_integration() {
  return array(
    'node' => array(
      'post' => array(
        'tft_file' => TRUE,
        'is_group_content' => FALSE,  //new review
        'field_related'  =>TRUE,
      ),
    ),
  );
}
*/

/**
 * Implements hook_gotosch_bw_group_widget().
 */
function gotosch_bw_all_gotosch_bw_group_widget() {
  return array(
    'gotosch_all' => array(
      'title' => t('All'),
      'type' => 'view',
      'vid' => 'gotosch_bw_all',
      'display' => 'default',
      'weight' => -10,
      'default' => 1,
      'bundle' => 'post',

    ),
  );
}
/**
 * Implements hook_gotosch_bw_create_all_widget().
 */
function gotosch_bw_all_gotosch_bw_create_all_widget($group) {
  if (user_access('create post content')) {
    $link = l(t('Create a post'),'node/add/post',
      array('attributes' => array('class' => 'gotosch-posts-create'), 'query' => array('field_related' => $group->nid))
    );
    return array(
      'gotosch_posts' => array(
        'default' => TRUE,
        'link' => $link,
        'text' =>  t('Foster a topic through commenting'),
        '#weight' => -1,
      ),
    );
  }
}
