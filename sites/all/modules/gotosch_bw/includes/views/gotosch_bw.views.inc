<?php

/**
 * Implements hook_views_data()
 */
function gotosch_bw_views_data() {
  $data = array();

  $data['views']['gotosch_bw_node_partial_form'] = array(
    'title' => t('Node partial form'),
    'help' => t('Display a minified node creation form.'),
    'area' => array(
      'handler' => 'gotosch_bw_handler_node_partial_form',
    ),
  );

  return $data;
}
