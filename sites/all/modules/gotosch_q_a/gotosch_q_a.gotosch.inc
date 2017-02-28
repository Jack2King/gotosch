<?php
/**
 * @file
 * Commons module integration for the Commons Q&A module.
 */



/**
 * Implements hook_gotosch_bw_group_widget().
 */
function gotosch_q_a_gotosch_bw_group_widget() {
  return array(
    'gotosch_q_a' => array(
      'title' => t('Q & A'),
      'type' => 'view',
      'vid' => 'gotosch_bw_q_a',
      'display' => 'default',
      'weight' => 5,
      'bundle' => 'question',
    ),
  );
}

/**
 * Implements hook_gotosch_bw_create_all_widget().
 */
function gotosch_q_a_gotosch_bw_create_all_widget($group) {
  if (user_access('create question content')) {
    $link = l(t('Ask a question'),'node/add/question',
      array('attributes' => array('class' => 'commons-q-a-create'), 'query' => array('field_related' => $group->nid))
    );
    return array(
      'gotosch_q_a' => array(
        'link' => $link,
        'text' => t('Learn the best answer'),
        '#weight' => 5,
      ),
    );
  }
}
