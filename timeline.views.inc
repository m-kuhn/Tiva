<?php
// $Id$

/*
 * Implement hook_views_data
 */

function timeline_views_data() {
  $data['archives'] = array(
    'table' => array(
      'group' => t('Archive'),
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

  /*
   * Define the records table...
   */

  $data['records']['table']['group'] = t('Archive');

  $data['records']['table']['base'] = array(
    'field' => 'rid',
    'title' => t('Timeline Records'),
    'help' => t("Images recorded by the timeline tool."),
  );

  $data['records']['table']['join'] = array(
    'node' => array (
      'left_field' => 'nid',
      'field'      => 'nid',
    ),
  );

  // rid
  $data['records']['rid'] = array(
    'title' => t('ID'),
    'help' => t('The record ID of the field'),
    'field' => array(
      'handler' => 'views_handler_field_custom',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // request_time
  $data['records']['request_time'] = array(
    'title' => t('Time'),
    'help' => t('The time the download of this image was initiated'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // image
  $data['records']['fid'] = array(
    'title' => t('Image'),
    'help' => t('The image of this field'),
    'field' => array(
      'handler' => 'views_handler_image',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // image
  $data['records']['newest_fid'] = array(
    'title' => t('Newest Image'),
    'help' => t('The newest image of this field'),
    'field' => array(
      'handler' => 'views_handler_image',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['records']['nid'] = array(
    'title' => t('Archive'),
    'help' => t('The node this record belongs to.'),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Archive'),
    ),
  );

  return $data;
}


/**
 * Implementation of hook_views_query_alter()
 *
 * This updates the query to only include the newest picture record and not join
 * the whole table
 * 
 */
function timeline_views_query_alter(&$view,&$query) {
  if (isset($query->fields['records_newest_fid'])) {

    unset($query->table_queue['records']);
    $table = $query->fields['records_newest_fid']['table'];
    $query->fields['records_newest_fid']['table'] = '';
    $query->fields['records_newest_fid']['field'] =
      "(SELECT fid FROM {$table} tbl WHERE tbl.nid = node.nid AND tbl.request_time = (SELECT MAX(tbl.request_time) FROM {$table} tbl WHERE tbl.nid = node.nid))";
  }
}