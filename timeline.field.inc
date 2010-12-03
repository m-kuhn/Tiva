<?php
// $Id$


function timeline_form_archive_node_form_alter(&$form, $form_state, $form_id) {
  unset($form['preview_picture']);
}

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

function timeline_url_validate($element, &$form_state) {
  $errors = array();
  $url = $element['#value'];
  // If we already validated that url, return.
  if (isset($form_state['storage']['timeline']['url'][$url])) {
    return;
  }

  // default supported extensions.
  // ToDo: make them configurable in admin display.
  $extensions = 'png gif jpg jpeg';
  $errors += timeline_validate_extensions($url, $extensions);

  if (empty($errors)) {
    $download = _timeline_download_image($url, 'preview');

    $errors += $download['errmsg'];

    // cache value
    if (empty($errors)) {
      $form_state['storage'] = array(
        'timeline' => array(
          'url' => array(
            $url => $download['file'],
          ),
        ),
      );
    }
  }

  foreach ($errors as $error) {
    form_error($element, t($error));
  }
}

/*
 *
 */
function _ajax_check_url_callback ($form, $form_state) {
  $img_tag = '';
  $url = $form_state['values']['archive_url']['und'][0];

  if (isset($form_state['storage']['timeline']['url'][$url['value']])) {
    $variables = array(
      'style_name' => 'thumbnail',
      'path'       => $form_state['storage']['timeline']['url'][$url['value']]->uri,
      'alt'        => 'Image Preview',
      'title'      => 'Image Preview',
      'getsize'    => TRUE,
    );

    $img_tag = theme('image_style', $variables);
  }

  return '<div id="url-check-div">' . $img_tag . '</div>';
}

?>