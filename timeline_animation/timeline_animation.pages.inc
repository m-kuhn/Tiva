<?php
// $Id$

/**
 * Generate the node add/edit form array.
 */
/*
function timeline_animation_form($form, &$form_state, $node) {
  global $user;

  // During initial form build, add the node entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['node'])) {
    if (!isset($node->title)) {
      $node->title = NULL;
    }
    node_object_prepare($node);
    $form_state['node'] = $node;
  }
  else {
    $node = $form_state['node'];
  }

  // Some special stuff when previewing a node.
  if (isset($form_state['node_preview'])) {
    $form['#prefix'] = $form_state['node_preview'];
    $node->in_preview = TRUE;
  }
  else {
    unset($node->in_preview);
  }

  // Identify this as a node edit form.
  // @todo D8: Remove. Modules can implement hook_form_BASE_FORM_ID_alter() now.
  $form['#node_edit_form'] = TRUE;

  $form['#attributes']['class'][] = 'node-form';
  if (!empty($node->type)) {
    $form['#attributes']['class'][] = 'node-' . $node->type . '-form';
  }

  // Basic node information.
  // These elements are just values so they are not even sent to the client.
  foreach (array('nid', 'vid', 'uid', 'created', 'type', 'language') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($node->$key) ? $node->$key : NULL,
    );
  }

  // Changed must be sent to the client, for later overwrite error checking.
  $form['changed'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($node->changed) ? $node->changed : NULL,
  );
  // Invoke hook_form() to get the node-specific bits. Can't use node_invoke(),
  // because hook_form() needs to be able to receive $form_state by reference.
  // @todo hook_form() implementations are unable to add #validate or #submit
  //   handlers to the form buttons below. Remove hook_form() entirely.
  $function = node_type_get_base($node) . '_form';
  if (function_exists($function) && ($extra = $function($node, $form_state))) {
    $form = array_merge_recursive($form, $extra);
  }
  // If the node type has a title, and the node type form defined no special
  // weight for it, we default to a weight of -5 for consistency.
  if (isset($form['title']) && !isset($form['title']['#weight'])) {
    $form['title']['#weight'] = -5;
  }
  // @todo D8: Remove. Modules should access the node using $form_state['node'].
  $form['#node'] = $node;

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Add a log field if the "Create new revision" option is checked, or if the
  // current user has the ability to check that option.
  $form['revision_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision information'),
    '#collapsible' => TRUE,
    // Collapsed by default when "Create new revision" is unchecked
    '#collapsed' => !$node->revision,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('node-form-revision-information'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'node') . '/node.js'),
    ),
    '#weight' => 20,
    '#access' => $node->revision || user_access('administer nodes'),
  );
  $form['revision_information']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision'),
    '#default_value' => $node->revision,
    '#access' => user_access('administer nodes'),
  );
  // Check the revision log checkbox when the log textarea is filled in.
  // This must not happen if "Create new revision" is enabled by default, since
  // the state would auto-disable the checkbox otherwise.
  if (!$node->revision) {
    $form['revision_information']['revision']['#states'] = array(
      'checked' => array(
        'textarea[name="log"]' => array('empty' => FALSE),
      ),
    );
  }
  $form['revision_information']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log message'),
    '#rows' => 4,
    '#default_value' => !empty($node->log) ? $node->log : '',
    '#description' => t('Provide an explanation of the changes you are making. This will help other authors understand your motivations.'),
  );

  // Node author information for administrators
  $form['author'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('administer nodes'),
    '#title' => t('Authoring information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('node-form-author'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'node') . '/node.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 90,
  );
  $form['author']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => !empty($node->name) ? $node->name : '',
    '#weight' => -1,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );
  $form['author']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored on'),
    '#maxlength' => 25,
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array('%time' => !empty($node->date) ? date_format(date_create($node->date), 'Y-m-d H:i:s O') : format_date($node->created, 'custom', 'Y-m-d H:i:s O'), '%timezone' => !empty($node->date) ? date_format(date_create($node->date), 'O') : format_date($node->created, 'custom', 'O'))),
    '#default_value' => !empty($node->date) ? $node->date : '',
  );

  // Node options for administrators
  $form['options'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('administer nodes'),
    '#title' => t('Publishing options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('node-form-options'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'node') . '/node.js'),
    ),
    '#weight' => 95,
  );
  $form['options']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Published'),
    '#default_value' => $node->status,
  );
  $form['options']['promote'] = array(
    '#type' => 'checkbox',
    '#title' => t('Promoted to front page'),
    '#default_value' => $node->promote,
  );
  $form['options']['sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky at top of lists'),
    '#default_value' => $node->sticky,
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#access' => variable_get('node_preview_' . $node->type, DRUPAL_OPTIONAL) != DRUPAL_REQUIRED || (!form_get_errors() && isset($form_state['node_preview'])),
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('node_form_submit'),
  );
  $form['actions']['preview'] = array(
    '#access' => variable_get('node_preview_' . $node->type, DRUPAL_OPTIONAL) != DRUPAL_DISABLED,
    '#type' => 'submit',
    '#value' => t('Preview'),
    '#weight' => 10,
    '#submit' => array('node_form_build_preview'),
  );
  if (!empty($node->nid) && node_access('delete', $node)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('node_form_delete_submit'),
    );
  }
  // This form uses a button-level #submit handler for the form's main submit
  // action. node_form_submit() manually invokes all form-level #submit handlers
  // of the form. Without explicitly setting #submit, Form API would auto-detect
  // node_form_submit() as submit handler, but that is the button-level #submit
  // handler for the 'Save' action. To maintain backwards compatibility, a
  // #submit handler is auto-suggested for custom node type modules.
  $form['#validate'][] = 'node_form_validate';
  if (!isset($form['#submit']) && function_exists($node->type . '_node_form_submit')) {
    $form['#submit'][] = $node->type . '_node_form_submit';
  }
  $form += array('#submit' => array());

  $form['#builder_function'] = 'node_form_submit_build_node';
  field_attach_form('node', $node, $form, $form_state, $node->language);

  return $form;
}
*/
?>