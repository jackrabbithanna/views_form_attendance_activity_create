<?php

/**
 * @file
 * Views file for Views Form Attendance Activity Create
 */

/**
 * Implements hook_views_data_alter().
 *
 * @param $data
 *   Information about Views' tables and fields.
 */
function views_form_attendance_activity_create_views_data_alter(&$data) {
  // Add the Title form field to
  $data['civicrm_contact']['attendance_form_element'] = array(
    'field' => array(
      'title' => t('Attendance Form Element'),
      'help' => t('Select the attendance value for the created activity'),
      'handler' => 'views_form_attendance_activity_create_field_handler_attendance',
    ),
  );
}

