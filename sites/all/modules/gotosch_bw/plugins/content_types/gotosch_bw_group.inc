<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('gotosch Group Browsing widget'),
  'description' => t('gotosch Group Browsing widget.'),
  'category' => t('gotosch'),
  'single' => TRUE,
  'content type' => 'gotosch_bw_group',
  'all contexts' => TRUE,
);

/**
* Output function for the '[content_type]' content type.
*/
function gotosch_bw_gotosch_bw_group_content_type_render($subtype, $conf, $panel_args, $context) {
  //drupal_set_message('start  in gotosch_bw_gotosch_bw_group_content_type_render');
  $context = array_shift($context);
  $node = !empty($context->data) ? $context->data : NULL;

  $block = new stdClass();
  $block->title = '';
  $widget = gotosch_bw_generate_group_widget($node);
  //drupal_set_message('in gotosch_bw_gotosch_bw_group_content_type_render');
  //print('------------- start widget[content]----------');
  ///print_r($widget['content']);
  //print('-------end widget[content]----------- <br>  \n');

  $block->content = drupal_render($widget['content']);

  return $block;
}

/**
* Returns an edit form for the custom type.
*/
function gotosch_bw_gotosch_bw_group_content_type_edit_form($form, $form_state) {
  return $form;
}

/**
* Returns an edit form for the custom type.
*/
function gotosch_bw_gotosch_bw_group_content_type_edit_form_submit($form, $form_state) {
  return $form;
}
