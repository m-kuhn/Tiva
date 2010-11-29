<?php
// $Id$

module_load_include('inc', 'node', '/node.pages');

/**
 * Mostly taken from
 */
function animation_add($archive) {
  global $user;

  $node = (object) array(
    'uid' => $user->uid,
    'name' => (isset($user->name) ? $user->name : ''),
    'type' => 'animation',
    'language' => LANGUAGE_NONE
  );

  $parentnode = node_load($archive);

  drupal_set_title(t('Create Animation for @archive', array('@archive' => $parentnode->title)), PASS_THROUGH);
  $form = drupal_get_form('node_form', $node, $archive);

  return $form;
}

/*
 * Get the original node form, set the default archive set in the url and disable
 * the select dropdown.
 *
 */
function animation_add_form($form, &$form_state, $node, $archive) {
//  $form = node_form($form, &$form_state, $node);

  $form['timeline_animation_archive']['und']['#default_value'] = $archive;
  $form['timeline_animation_archive']['und']['#disabled'] = true;
  return $form;
}

?>