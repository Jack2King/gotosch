<?php
/**
 * @file
 * gotosch_polls.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function gotosch_polls_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'gotosch_bw_polls';
  $view->description = 'gotosch Polls listing for the group browsing widget.';
  $view->tag = 'gotosch Polls, gotosch Browsing Widget';
  $view->base_table = 'node';
  $view->human_name = 'gotosch Browsing Widget - Polls';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'lite';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '6';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Header: Global: Node partial form */
  $handler->display->display_options['header']['gotosch_bw_node_partial_form']['id'] = 'gotosch_bw_node_partial_form';
  $handler->display->display_options['header']['gotosch_bw_node_partial_form']['table'] = 'views';
  $handler->display->display_options['header']['gotosch_bw_node_partial_form']['field'] = 'gotosch_bw_node_partial_form';
  $handler->display->display_options['header']['gotosch_bw_node_partial_form']['bundle'] = 'poll';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No polls have been added to this group.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';

  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'most recent';

  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  //$handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'poll' => 'poll',
  );

  $translatables['gotosch_bw_polls'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('No polls have been added to this group.'),
    t('OG membership from node'),
    t('following'),
    t('most recent'),
    t('most active'),
    t('All'),
    t('Following'),
  );

  $export['gotosch_bw_polls'] = $view;

  return $export;
}

/**
 * Implements hook_views_default_views_alter().
 *
 * Display polls on the browsing widget main view.
 */
function gotosch_polls_views_default_views_alter(&$views) {
  if (!empty($views['gotosch_bw_all'])) {
    $views['gotosch_bw_all']->display['default']->display_options['filters']['type']['value']['poll'] = 'poll';
  }
}
