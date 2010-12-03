<?php
// $Id$



/*
 * Here follow the definitions for our URL-Field.
 * We have to do this to perform ajax. No idea why.
 *
 */

function timeline_field_widget_info() {
  return array(
    'text_timeline_url' => array(
      'label' => t('Source URL'),
      'field types' => array('text'),
      'settings' => array('size' => 60),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}


function timeline_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $summary_widget = array();
  $main_widget = array();

  switch ($instance['widget']['type']) {
    case 'text_timeline_url':
      $main_widget = $element + array(
        '#type' => 'textfield',
        '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
        '#size' => $instance['widget']['settings']['size'],
        '#maxlength' => $field['settings']['max_length'],
        '#attributes' => array('class' => array('text-full')),
        '#ajax'             => array(
          'callback'        => '_ajax_check_url_callback',
          'wrapper'         => 'url-check-div',
          'method'          => 'replace',
          'effect'          => 'fade',
        ),
        '#suffix'           => '<div id="url-check-div"></div>',
        '#element_validate' => array('timeline_url_validate'),
      );
      break;
  }

  $element['value'] = $main_widget;

  return $element;
}


?>