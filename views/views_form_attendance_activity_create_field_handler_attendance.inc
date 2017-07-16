<?php

/**
 * @file
 * A Views field handler for selecting an Activity Attendance value.
 */

/**
 * Class views_form_attendance_activity_create_field_handler_attendance
 */
class views_form_attendance_activity_create_field_handler_attendance  extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Render a Views form item placeholder.
    // This causes Views to wrap the View in a form.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Add to and alter the form created by Views.
   */
  function views_form(&$form, &$form_state) {
    // get our attendance options
    $attendance_options = ['default' => 'Use Default', 'no_create' => 'Do not create activity'] + _views_form_attendance_activity_create_get_attendance_value_options();

    // Create a container for our replacements
    $form[$this->options['id']] = [
      '#type' => 'container',
      '#tree' => TRUE,
      '#weight' => 0,
    ];
    $form['cid'] = [
      '#type' => 'container',
      '#tree' => TRUE,
      '#weight' => 1,
    ];
    // Iterate over the result and add our replacement fields to the form.
    foreach ($this->view->result as $row_index => $row) {
      // Add a text field to the form.  This array convention
      // corresponds to the placeholder HTML comment syntax.
      $form['cid'][$row_index] = [
        '#type' => 'value',
        '#value' => $row->id,
      ];
      $form[$this->options['id']][$row_index] = [
        '#type' => 'select',
        '#options' => $attendance_options,
        '#default_value' => 'default',
        //'#element_validate' => array('views_form_attendance_activity_create_field_handler_attendance_validate'),
      ];
    }
  }

  /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    $attendance_custom_field = variable_get('views_form_attendance_activity_create_attendance_cf_id', '');
    $attendance_date_custom_field = variable_get('views_form_attendance_activity_create_attendance_date_cf_id', '');
    // Iterate over the view result.
    if (!empty($attendance_custom_field) && !empty($attendance_date_custom_field)) {
      foreach ($this->view->result as $row_index => $row) {
        // Grab the correspondingly submitted form values
        $attendance_value = $form_state['values'][$this->options['id']][$row_index];
        $cid = $form_state['values']['cid'][$row_index];
        // cid should go tho target_contact_id
        // activity status = Completed

      }
    }
  }

}
