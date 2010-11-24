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

    case 'text_textarea_with_summary':
      $display = !empty($items[$delta]['summary']) || !empty($instance['settings']['display_summary']);
      $summary_widget = array(
        '#type' => $display ? 'textarea' : 'value',
        '#default_value' => isset($items[$delta]['summary']) ? $items[$delta]['summary'] : NULL,
        '#title' => t('Summary'),
        '#rows' => $instance['widget']['settings']['summary_rows'],
        '#description' => t('Leave blank to use trimmed value of full text as the summary.'),
        '#attached' => array(
          'js' => array(drupal_get_path('module', 'text') . '/text.js'),
        ),
        '#attributes' => array('class' => array('text-summary')),
        '#prefix' => '<div class="text-summary-wrapper">',
        '#suffix' => '</div>',
        '#weight' => -10,
      );
      // Fall through to the next case.

    case 'text_textarea':
      $main_widget = $element + array(
        '#type' => 'textarea',
        '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
        '#rows' => $instance['widget']['settings']['rows'],
        '#attributes' => array('class' => array('text-full')),
      );
      break;
  }

  if ($main_widget) {
    // Conditionally alter the form element's type if text processing is enabled.
    if ($instance['settings']['text_processing']) {
      $element = $main_widget;
      $element['#type'] = 'text_format';
      $element['#format'] = isset($items[$delta]['format']) ? $items[$delta]['format'] : NULL;
      $element['#base_type'] = $main_widget['#type'];
    }
    else {
      $element['value'] = $main_widget;
    }
  }
  if ($summary_widget) {
    $element['summary'] = $summary_widget;
  }

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
    $download = _timeline_download_image($url);

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

function timeline_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'text_default':
    case 'text_trimmed':
      foreach ($items as $delta => $item) {
        $output = _text_sanitize($instance, $langcode, $item, 'value');
        if ($display['type'] == 'text_trimmed') {
          $output = text_summary($output, $instance['settings']['text_processing'] ? $item['format'] : NULL, $display['settings']['trim_length']);
        }
        $element[$delta] = array('#markup' => $output);
      }
      break;

    case 'text_summary_or_trimmed':
      foreach ($items as $delta => $item) {
        if (!empty($item['summary'])) {
          $output = _text_sanitize($instance, $langcode, $item, 'summary');
        }
        else {
          $output = _text_sanitize($instance, $langcode, $item, 'value');
          $output = text_summary($output, $instance['settings']['text_processing'] ? $item['format'] : NULL, $display['settings']['trim_length']);
        }
        $element[$delta] = array('#markup' => $output);
      }
      break;

    case 'text_plain':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => strip_tags($item['value']));
      }
      break;
  }

  return $element;
}
?>