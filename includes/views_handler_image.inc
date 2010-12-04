<?php
// $Id: views_handler_field_field.inc,v 1.1.2.6 2010/11/27 23:28:28 dereine Exp $

/**
 * A field that displays fields.
 */
class views_handler_image extends views_handler_field {
  /**
   * Called to add the field to a query.
   */
  function query() {
    parent::query();
  }

  function option_definition() {
    $options = parent::option_definition();


    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

  }

  /**
   * Loads the objects providing the field information necessary to render the
   *   field in the View.
   */
  function pre_render(&$values) {
    if (!empty($values)) {
      foreach ($values as $key => $value) {
        if (!empty($value->{$this->field_alias})) {
          $values[$key]->_field_data[$this->field_alias] = file_load($value->{$this->field_alias});
        }
      }
    }
  }

  function render($values) {
    if (isset($values->_field_data[$this->field_alias])) {
      // Prepare arguments for rendering based on the objects cached in the
      // pre-render phase and the display options for this field.
      $image = $values->_field_data[$this->field_alias];

      $variables = array(
        'style_name' => 'thumbnail',
        'path'       => $image->uri,
        'alt'        => 'Image Preview',
        'title'      => 'Image Preview',
        'getsize'    => TRUE,
      );

      return(theme('image_style', $variables));
    }
    else {
      return '';
    }
  }
}
