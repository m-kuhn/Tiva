<?php
// $Id$

/*
 * Implement hook_views_data
 */

function timeline_views_data() {
  $data['archives'] = array(
    'table' => array(
      'group' => t('Archive'),
      'base'  => array(
        'field' => 'nid',
        'title' => 'Node ID',
      ),
      'join' => array(
        'node' => array (
          'left_field' => 'nid',
          'field'      => 'nid',
        ),
      ),
    ),
    'url' => array(
      'title' => t('URL'),
      'help' => t('The url of this archive.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
    ),
    'preview_picture' => array(
      'title' => t('Preview Picture'),
      'help'  => t('The picture you want as preview.'),
      'field' => array(
        'handler' => 'views_handler_image',
        'click sortable' => TRUE,
      ),
    )
  );

  return $data;
}
