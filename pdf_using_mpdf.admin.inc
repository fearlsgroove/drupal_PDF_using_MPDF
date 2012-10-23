<?php

/**
 * @file
 * Contains the administrative functions of the PDF conversion module.
 *
 * This file is included by the PDF conversion module, and includes the
 * settings form.
 *
 */

/**
 * Admin configure form control on page.
 */
function pdf_using_mpdf_config() {
  $file_lib =  file_scan_directory( MPDF_LIB_PATH , '/.*\.php$/' ) ;
  $file_module =  file_scan_directory( MPDF_MODULE_PATH . '/mpdf' , '/.*\.php$/' ) ;

  if ( isset ($file_lib[MPDF_LIB_PATH . '/' . MPDF_API_FILE] ) || isset ($file_module[MPDF_MODULE_PATH . '/mpdf/' . MPDF_API_FILE] ) ) {
    variable_set('mpdf_library_exist', TRUE);
    $form['pdf'] = array(
      '#type' => 'fieldset',
      '#title' => t('PDF Tool Option'),
    );
    $form['pdf']['radio_btn'] = array(
      '#type' => 'markup',
      '#title' => 'MPDF library',
      '#markup' => t('Add mPDF library to "' . MPDF_LIB_PATH . '/' . MPDF_API_FILE . '" or "' . MPDF_MODULE_PATH . '/mpdf/' . MPDF_API_FILE . '".' ),
    );
    $form['pdf']['pdf_author'] = array(
      '#type' => 'textfield',
      '#size' => 35,
      '#title' => t('Author'),
      '#default_value' => variable_get('pdf_set_author'),
      '#description' => t('Set Author of PDF. If not required, leave blank.'),
    );
    $form['pdf']['pdf_subject'] = array(
      '#type' => 'textfield',
      '#size' => 35,
      '#title' => t('Subject'),
      '#default_value' => variable_get('pdf_set_subject'),
      '#description' => t('Set Subject of PDF. If not required, leave blank.'),
    );
    $form['pdf']['save_pdf'] = array(
      '#type' => 'radios',
      '#title' => t('Open PDF File in'),
      '#options' => array( t('Web Browser'), t('Save Dialog Box') ),
      '#default_value' => (variable_get('pdf_save_option') == 'Web Browser' ) ? 0 : 1,
    );
    $form['pdf']['page_size'] = array(
      '#type' => 'select',
      '#title' => t('Page Size'),
      '#options' => array('2A0' => '2A0', '4A0' => '4A0', 'A0' => 'A0', 'A1' => 'A1', 'A2' => 'A2', 'A3' => 'A3', 'A4' => 'A4',
        'A5' => 'A5', 'A6' => 'A6', 'A7' => 'A7', 'A8' => 'A8', 'A9' => 'A9', 'A10' => 'A10', 'B0' => 'B0', 'B1' => 'B1', 
        'B2' => 'B2', 'B3' => 'B3', 'B4' => 'B4', 'B5' => 'B5', 'B6' => 'B6', 'B7' => 'B7', 'B8' => 'B8', 'B9' => 'B9', 
        'B10' => 'B10', 'C0' => 'C0', 'C1' => 'C1', 'C2' => 'C2', 'C3' => 'C3', 'C4' => 'C4', 'C5' => 'C5', 'C6' => 'C6', 
        'C7' => 'C7', 'C8' => 'C8', 'C9' => 'C9', 'C10' => 'C10', 'RA0' => 'RA0', 'RA1' => 'RA1', 'RA2' => 'RA2', 
        'SRA0' => 'SRA0', 'SRA1' => 'SRA1', 'SRA2' => 'SRA2', 'LETTER' => 'Letter', 'LEGAL' => 'Legal',
      ),
      '#default_value' => variable_get('pdf_page_size', PDF_PAGE_SIZE),
    );

    $form['pdf']['font_size'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Font Size'),
      '#default_value' => variable_get('pdf_font_size', PDF_FONT_SIZE),
    );
    $form['pdf']['watermark_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Watermark Text'),
      '#default_value' => variable_get('pdf_watermark_text'),
      '#description' => t('Display diagonal text on every page of PDF. If not required, leave it blank.'),
    );
    $form['pdf']['header'] = array(
      '#type' => 'fieldset',
      '#title' => t('PDF Header Option'),
      '#description' => t('use') . ' {PAGENO} ' . t('for page numbering or') . " ' {DATE j-m-Y} ' " . t('for current date.'),
    );
    // 'h' for header
    $form['pdf']['header']['h_left'] = array(
      '#type' => 'textfield',
      '#title' => t('Left side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get( 'pdf_header_left' ),
    );
    $form['pdf']['header']['h_center'] = array(
      '#type' => 'textfield',
      '#title' => t('Center content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get( 'pdf_header_center' ),
    );
    $form['pdf']['header']['h_right'] = array(
      '#type' => 'textfield',
      '#title' => t('Right side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get( 'pdf_header_right' ),
    );
    // 'f' for footer
    $form['pdf']['footer'] = array(
      '#type' => 'fieldset',
      '#title' => t('PDF Footer Option'),
      '#description' => t('use') . ' {PAGENO} ' . t("for page numbering or") . " ' {DATE j-m-Y} ' " . t('for current date.'),
    );
    $form['pdf']['footer']['f_left'] = array(
      '#type' => 'textfield',
      '#title' => t('Left side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get( 'pdf_footer_left' ),
    );
    $form['pdf']['footer']['f_center'] = array(
      '#type' => 'textfield',
      '#title' => t('Center content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get( 'pdf_footer_center' ),
    );
    $form['pdf']['footer']['f_right'] = array(
      '#type' => 'textfield',
      '#title' => t('Right side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get( 'pdf_footer_right' ),
    );
    // Setting password to PDF, if entered.
    $form['pdf']['permission'] = array(
      '#type' => 'fieldset',
      '#title' => t('PDF Password'),
      '#description' => t('If password is not required, leave blank.'),
    );
    $form['pdf']['permission']['password'] = array(
      '#type' => 'password_confirm',
    );
    $pwd = variable_get('pdf_password');
    if ( isset($pwd) ) {
      $form['pdf']['permission']['pwd'] = array(
        '#type' => 'markup',
        '#markup' => t('Password : ******** is already set. To change password, enter it again.'),
      );
      $form['pdf']['permission']['remove_pwd'] = array(
        '#type' => 'checkbox',
        '#title' => 'Remove Password',
        '#default_value' => '0',
      );
    }
    // Setting Style Sheets to PDF, if applied.
    $form['pdf']['style'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom Style Sheets for PDF'),
      '#description' => t('If not required, leave blank.'),
    );
    $form['pdf']['style']['css_file'] = array(
      '#type' => 'textfield',
      '#description' => t('Enter your name of css file, Eg: style.css. Place all stylesheets in same module directory, i.e.') . ' " ' . drupal_get_path('module', 'pdf_using_mpdf') . '/ " .' . t('If the file is in a folder in module directory, enter the name with  path to that folder, e.g: "css_folder/style.css" .'),
      '#default_value' => variable_get('pdf_css_file'),
    );
    
    $css = variable_get('pdf_css_file');
    if ( isset($css) ) {
      $form['pdf']['style']['msg'] = array(
        '#type' => 'markup',
        '#markup' => t('Style Sheet is already set. To change css style, enter its name again.'),
      );
      $form['pdf']['style']['remove_css'] = array(
        '#type' => 'checkbox',
        '#title' => 'Remove Style Sheet',
        '#default_value' => '0',
      );
    }   
    $filename = variable_get('pdf_filename');
    $form['pdf']['filename'] = array(
      '#type' => 'textfield',
      '#title' => t('PDF Filename'),
      '#description' => t('If no name given to PDF file, Default filename : "' . PDF_DEFAULT_FILENAME . '" will be used.'),
      '#default_value' => isset($filename) && $filename != NULL ? $filename : PDF_DEFAULT_FILENAME,
    ); 
    $form['pdf']['save_config'] = array(
      '#type' => 'submit',
      '#value' => t('Save Configuration'),
    );
  }
  else {
    variable_set('mpdf_library_exist', FALSE);
    $form['pdf'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . t('Sorry ! No mPDF Library Found in"' . ' ' . MPDF_LIB_PATH . '/' . MPDF_API_FILE . ' ' . '"Please dowload mPDF supported PHP PDF generation tool from') . ' ' . l(t('MPDF Library.'), 'http://www.mpdf1.com/') . '</p>',
    );
  }
  
  return $form;    
}
/* ********************* End of admin.inc  ************************ */