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

  /**
   * Default options form.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['views_form_attendance_activity_create_attendance_cf_id'] = array('default' => '');
    $options['views_form_attendance_activity_create_attendance_cf_label'] = array('default' => '');
    $options['views_form_attendance_activity_create_attendance_date_cf_id'] = array('default' => '');
    $options['views_form_attendance_activity_create_attendance_activity_cf_id'] = array('default' => '');
    $options['views_form_attendance_activity_create_attendance_activity_cf_label'] = array('default' => '');
    $options['views_form_attendance_activity_create_activity_type'] = array('default' => '');
    return $options;
  }

  /**
   * Creates the form item for the options added.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $activity_custom_field_options = [];
    if (civicrm_initialize()) {
      try {
        $result = civicrm_api3('Activity', 'getfields', array(
          'sequential' => 1,
          'api_action' => "create",
        ));
        if (!empty($result['count'])) {
          foreach ($result['values'] as $index => $field_data) {
            if (strpos($field_data['name'], 'custom_') === 0) {
              $activity_custom_field_options[$field_data['name']] = $field_data['title'];
            }
          }
        }
      }
      catch (Exception $e) {
      }
    }

    $form['views_form_attendance_activity_create_attendance_cf_id'] = [
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Attendance Custom Field'),
      '#options' => $activity_custom_field_options,
      '#default_value' => $this->options['views_form_attendance_activity_create_attendance_cf_id'],
    ];
    $form['views_form_attendance_activity_create_attendance_cf_label'] = [
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Attendance Custom Field Label'),
      '#default_value' => $this->options['views_form_attendance_activity_create_attendance_cf_label'],
      '#size' => 40,
    ];

    $form['views_form_attendance_activity_create_attendance_date_cf_id'] = [
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Attendance Date Custom Field'),
      '#options' => $activity_custom_field_options,
      '#default_value' => $this->options['views_form_attendance_activity_create_attendance_date_cf_id'],
    ];

    $form['views_form_attendance_activity_create_attendance_activity_cf_id'] = [
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Activity Custom Field'),
      '#options' => $activity_custom_field_options,
      '#default_value' => $this->options['views_form_attendance_activity_create_attendance_activity_cf_id'],
    ];
    $form['views_form_attendance_activity_create_attendance_activity_cf_label'] = [
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Attendance Activity Custom Field Label'),
      '#default_value' => $this->options['views_form_attendance_activity_create_attendance_activity_cf_label'],
      '#size' => 40,
    ];
    $form['views_form_attendance_activity_create_activity_type'] = [
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Activity Type'),
      '#options' => _views_form_attendance_activity_create_get_activity_type_options(),
      '#default_value' => $this->options['views_form_attendance_activity_create_activity_type'],
    ];
  }

  /**
   * If the view is using a table style, provide a
   * placeholder for a "select all" checkbox.
   */
  function label() {
    if (!empty($this->view->style_plugin) && $this->view->style_plugin instanceof views_plugin_style_table) {
      return '<!--views-form-attendance-activity-create-select-all-->';
    }
    else {
      return parent::label();
    }
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
    $form['#attached']['css'][] = drupal_get_path('module', 'views_form_attendance_activity_create') . '/css/views_form_attendance_activity_create.css';
    $form['#attached']['js'][] = drupal_get_path('module', 'views_form_attendance_activity_create') . '/js/views_form_attendance_activity_create.js';
    $class = 'vfaac-views-form';

    $form['#prefix'] = '<div class="' . $class . '">';
    $form['#suffix'] = '</div>';

    // Force browser to reload the page if Back is hit.
    if (!empty($_SERVER['HTTP_USER_AGENT']) && preg_match('/msie/i', $_SERVER['HTTP_USER_AGENT'])) {
      drupal_add_http_header('Cache-Control', 'no-cache'); // works for IE6+
    }
    else {
      drupal_add_http_header('Cache-Control', 'no-store'); // works for Firefox and other browsers
    }

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
      '#default_value' => $this->options['views_form_attendance_activity_create_activity_type'],
      '#required' => TRUE,
    ];
    $form['default_options_section']['attendance'] = [
      '#type' => 'select',
      '#title' => 'Default Activity Attendance Value',
      '#options' => _views_form_attendance_activity_create_get_attendance_value_options($this->options['views_form_attendance_activity_create_attendance_cf_id']),
      '#default_value' => 'Present',
    ];
    $form['default_options_section']['activity'] = [
      '#type' => 'select',
      '#title' => 'Default Activity Value',
      '#options' => ['' => 'No value'] + _views_form_attendance_activity_create_get_activity_cf_value_options($this->options['views_form_attendance_activity_create_attendance_activity_cf_id']),
      '#default_value' => '',
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
      $form[$this->options['id']][$row_index]['vfaac_select_row'] = [
        '#type' => 'checkbox',
        '#return_value' => 1,
        '#theme_wrappers' => [],
        '#attributes' => array('class' => array('views-form-attendance-activity-create-select')),
      ];
      $form[$this->options['id']][$row_index]['attendance'] = [
        '#type' => 'select',
        '#title' => check_plain($this->options['views_form_attendance_activity_create_attendance_cf_label']),
        '#options' => ['default' => 'Use Default'] + _views_form_attendance_activity_create_get_attendance_value_options($this->options['views_form_attendance_activity_create_attendance_cf_id']),
        '#default_value' => 'default',
        '#prefix' => '<span class="attendance-create-select">',
        '#suffix' => '</span>',
      ];
      $form[$this->options['id']][$row_index]['activity'] = [
        '#type' => 'select',
        '#title' => check_plain($this->options['views_form_attendance_activity_create_attendance_activity_cf_label']),
        '#options' => ['' => 'No value', 'default' => 'Use Default'] + _views_form_attendance_activity_create_get_activity_cf_value_options($this->options['views_form_attendance_activity_create_attendance_activity_cf_id']),
        '#default_value' => 'default',
        '#prefix' => '<span class="attendance-create-select">',
        '#suffix' => '</span>',
      ];

    }
  }

  /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    $attendance_custom_field = $this->options['views_form_attendance_activity_create_attendance_cf_id'];
    $attendance_date_custom_field = $this->options['views_form_attendance_activity_create_attendance_date_cf_id'];
    $attendance_activity_custom_field = $this->options['views_form_attendance_activity_create_attendance_activity_cf_id'];

    $default_date = $form_state['values']['default_options_section']['date'];
    $activity_type_id = $form_state['values']['default_options_section']['type'];
    $default_attendance_status = $form_state['values']['default_options_section']['attendance'];
    $default_activity_value = $form_state['values']['default_options_section']['activity'];

    // Iterate over the view result.
    if (!empty($attendance_custom_field) && !empty($attendance_date_custom_field) && !empty($attendance_activity_custom_field)) {
      foreach ($this->view->result as $row_index => $row) {
        // Grab the correspondingly submitted form values
        $attendance_value = $form_state['values'][$this->options['id']][$row_index]['attendance'];
        $activity_value = $form_state['values'][$this->options['id']][$row_index]['activity'];
        $cid = $form_state['values'][$this->options['id']][$row_index]['cid'];
        $create_activity = $form_state['values'][$this->options['id']][$row_index]['vfaac_select_row'];

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
          if (!empty($activity_value)) {
            if ($activity_value == 'default') {
              if (!empty($default_activity_value)) {
                $params[$attendance_activity_custom_field] = $default_activity_value;
              }
            }
            else {
              $params[$attendance_activity_custom_field] = $activity_value;
            }
          }

          try {
            $result = civicrm_api3('Activity', 'create', $params);
            drupal_set_message(check_plain('Activity created for contact with cid:' . $cid));
          }
          catch (CiviCRM_API3_Exception $e) {
            drupal_set_message($e->getMessage());
            drupal_set_message(check_plain('Error creating activity for contact with cid:' . $cid));
          }
        }
      }
    }
  }
}
