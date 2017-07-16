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
    $form['#attached']['css'][] = drupal_get_path('module', 'views_form_attendance_activity_create') . '/css/views_form.css';

    $form['default_options_section'] = [
      '#type' => 'container',
      '#tree' => TRUE,
      '#weight' => -1,
      '#prefix' => '<h3>Default Values</h3>',
      '#suffix' => '<h3>Contacts</h3>',
    ];

    $form['default_options_section']['date'] = [
      '#type' => 'date_popup',
      '#date_format' => 'Y-m-d H:i:s',
      '#date_year_range' => '-1:+1',
      '#default_value' => date('Y-m-d H:i:s'),
      '#required' => TRUE,
    ];
    $form['default_options_section']['type'] = [
      '#type' => 'select',
      '#title' => 'Activity Type',
      '#options' =>  _views_form_attendance_activity_create_get_activity_type_options(),
      '#default_value' => variable_get('views_form_attendance_activity_create_activity_type', ''),
      '#required' => TRUE,
    ];
    $form['default_options_section']['value'] = [
      '#type' => 'select',
      '#title' => 'Default Activity Value',
      '#options' => _views_form_attendance_activity_create_get_attendance_value_options(),
      '#default_value' => 'Present',
    ];

    // Create a container for our replacements
    $form[$this->options['id']] = [
      '#type' => 'container',
      '#tree' => TRUE,
      '#weight' => 0,
    ];

    // Iterate over the result and add our replacement fields to the form.
    foreach ($this->view->result as $row_index => $row) {
      $form[$this->options['id']][$row_index] = [
        '#type' => 'container',
        '#tree' => TRUE,
      ];
      $form[$this->options['id']][$row_index]['cid'] = [
        '#type' => 'value',
        '#value' => $row->id,
      ];
      $form[$this->options['id']][$row_index]['select_row'] = [
        '#type' => 'checkbox',
        '#return_value' => 1,
        '#theme_wrappers' => [],
      ];
      $form[$this->options['id']][$row_index]['attendance'] = [
        '#type' => 'select',
        '#options' => ['default' => 'Use Default'] + _views_form_attendance_activity_create_get_attendance_value_options(),
        '#default_value' => 'default',
        '#theme_wrappers' => [],
      ];

    }
  }

  /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    $attendance_custom_field = variable_get('views_form_attendance_activity_create_attendance_cf_id', '');
    $attendance_date_custom_field = variable_get('views_form_attendance_activity_create_attendance_date_cf_id', '');

    $default_date = $form_state['values']['default_options_section']['date'];
    $activity_type_id = $form_state['values']['default_options_section']['type'];
    $default_attendance_status = $form_state['values']['default_options_section']['value'];

    // Iterate over the view result.
    if (!empty($attendance_custom_field) && !empty($attendance_date_custom_field)) {
      foreach ($this->view->result as $row_index => $row) {
        // Grab the correspondingly submitted form values
        $attendance_value = $form_state['values'][$this->options['id']][$row_index]['attendance'];
        $cid = $form_state['values'][$this->options['id']][$row_index]['cid'];
        $create_activity = $form_state['values'][$this->options['id']][$row_index]['select_row'];

        if (!empty($create_activity)) {
          $params = [
            'sequential' => 1,
            'activity_type_id' => $activity_type_id,
            'status_id'         => "Completed",
            'target_id'         => $cid,
            $attendance_date_custom_field => $default_date,
          ];
          if ($attendance_value == 'default') {
            $params[$attendance_custom_field] = $default_attendance_status;
          }
          else {
            $params[$attendance_custom_field] = $attendance_value;
          }
          try {
            $result = civicrm_api3('Activity', 'create', $params);
            drupal_set_message('Activity created for contact with cid:' . $cid);
          }
          catch (CiviCRM_API3_Exception $e) {
            drupal_set_message($e->getMessage());
            drupal_set_message('Error creating activity for contact with cid:' . $cid);
          }
        }
      }
    }
  }
}
