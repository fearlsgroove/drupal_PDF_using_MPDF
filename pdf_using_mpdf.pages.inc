<?php

/**
 * @file
 * Contains all description to generate PDF document from raw HTML content.
 */

/**
 * Generate HTML of a given node.
 *
 * @return mpdf
 *   generated PDF document.
 */
function pdf_using_mpdf_generate_pdf() {
  global $theme;

  if (user_access('administer custom mpdf')) {
    $arg = arg();
    $node = _pdf_using_mpdf_node_generator($arg[count($arg) - 1]);

    if (empty($node)) {
      drupal_get_messages('error');
      drupal_set_message(t('PDF cannot be generated for this path.'), 'error');
      return;
    }
    $theme_path = drupal_get_path('module', 'pdf_using_mpdf');

    $pdf_filename = variable_get('pdf_filename', PDF_DEFAULT_FILENAME);
    $pdf_filename = token_replace($pdf_filename, array('node' => $node));
    $file = file_scan_directory($theme_path, '/pdf.*\.tpl.php$/');

    // Finding 'pdf-node--[node_id].tpl.php' - First priority.
    if (isset ($file[$theme_path . '/' . 'pdf--node--' . $node->nid . '.tpl.php'])) {
      $html = theme('pdf_node_id', array('node' => $node));
    }

    // Finding 'pdf-node--[node_type].tpl.php' - Second priority.
    elseif (isset($file[$theme_path . '/' . 'pdf--node--' . $node->type . '.tpl.php'])) {
      $html = theme('pdf_node_type', array('node' => $node));
    }

    // Finding 'pdf-node.tpl.php' - Third priority.
    elseif (isset($file[$theme_path . '/' . 'pdf--node.tpl.php'])) {
      $html = theme('pdf_node', array('node' => $node));
    }
    else {
      $view = node_view($node);
      $view['links']['node']['#links'] = array();
      $html = drupal_render($view);
    }
    return _pdf_using_mpdf_generator($html, $pdf_filename);
  }
  else {
    drupal_get_messages('warning');
    drupal_set_message(t('Access Denied - you are not allowed to convert to PDF document. Contact site administrator.'), 'warning');
  }
}

/**
 * Generate node object if alias is given, or simply load node using nid.
 *
 * @param string $path
 *   path given for a node.
 *
 * @return node
 *   node array for a given path.
 */
function _pdf_using_mpdf_node_generator($path) {
  $node = array();
  $alias = drupal_get_normal_path($path);
  // Alias of the given node.
  if ($alias) {
    if ($alias != $path) {
      // If the given path is some node type.
      if (preg_match('/^node*/', $alias)) {
        $nid = preg_replace('!^node/!', '', $alias);
        $node = node_load($nid);
      }
      else {
        drupal_set_message(t('PDF cannot be generated for this path.'), 'error');
      }
    }
    elseif (ctype_digit($path)) {
      // Normal node id.
      $node = node_load($path);
    }
  }
  if (empty($node)) {
    // Drupal_set_message(t('PDF cannot be generated for this path.'), 'error').
  }
  else {
    return $node;
  }
}

/**
 * Generate the PDF file using the mPDF library.
 *
 * @param string $html
 *   contents of the template already with the node data.
 * @param string $filename
 *   name of the PDF file to be generated.
 */
function _pdf_using_mpdf_generator($html, $filename = NULL) {
  // International Paper Sizes ( width x height).
  $paper_size = array(
    '4A0' => array('w' => 1682, 'h' => 2378),
    '2A0' => array('w' => 1189, 'h' => 1682),
    'A0' => array('w' => 841, 'h' => 1189),
    'A1' => array('w' => 594, 'h' => 841),
    'A2' => array('w' => 420, 'h' => 594),
    'A3' => array('w' => 297, 'h' => 420),
    'A4' => array('w' => 210, 'h' => 297),
    'A5' => array('w' => 148, 'h' => 210),
    'A6' => array('w' => 105, 'h' => 148),
    'A7' => array('w' => 74, 'h' => 105),
    'A8' => array('w' => 52, 'h' => 74),
    'A9' => array('w' => 37, 'h' => 52),
    'A10' => array('w' => 26, 'h' => 37),

    'B0' => array('w' => 1000, 'h' => 1414),
    'B1' => array('w' => 707, 'h' => 1000),
    'B2' => array('w' => 500, 'h' => 707),
    'B3' => array('w' => 353, 'h' => 500),
    'B4' => array('w' => 250, 'h' => 353),
    'B5' => array('w' => 176, 'h' => 250),
    'B6' => array('w' => 125, 'h' => 176),
    'B7' => array('w' => 88, 'h' => 125),
    'B8' => array('w' => 62, 'h' => 88),
    'B9' => array('w' => 44, 'h' => 62),
    'B10' => array('w' => 31, 'h' => 44),

    'C0' => array('w' => 917, 'h' => 1297),
    'C1' => array('w' => 648, 'h' => 917),
    'C2' => array('w' => 458, 'h' => 648),
    'C3' => array('w' => 324, 'h' => 458),
    'C4' => array('w' => 229, 'h' => 324),
    'C5' => array('w' => 162, 'h' => 229),
    'C6' => array('w' => 114, 'h' => 162),
    'C7' => array('w' => 81, 'h' => 114),
    'C8' => array('w' => 57, 'h' => 81),
    'C9' => array('w' => 40, 'h' => 57),
    'C10' => array('w' => 28, 'h' => 40),

    'RA0' => array('w' => 860, 'h' => 1220),
    'RA1' => array('w' => 610, 'h' => 860),
    'RA2' => array('w' => 430, 'h' => 610),
    'SRA0' => array('w' => 900, 'h' => 1280),
    'SRA1' => array('w' => 640, 'h' => 900),
    'SRA2' => array('w' => 450, 'h' => 640),

    'Letter' => array('w' => 215.9, 'h' => 279.4),
    'Legal' => array('w' => 215.9, 'h' => 355.6),
    'Ledger' => array('w' => 279.4, 'h' => 431.8),
  );

  $root_path = drupal_get_path('module', 'pdf_using_mpdf');
  $page = variable_get('pdf_page_size', PDF_PAGE_SIZE);
  $font_size = variable_get('pdf_font_size', PDF_FONT_SIZE);

  // Search for mpdf tool first.
  $pattern = '/^mpdf.php$/';

  //Libraries module used to detect mPDF library in case of a multisite installation.
  $tools = array_keys(file_scan_directory(libraries_get_path('mpdf'), $pattern));

  //mPDF library if not in /sites/all/libraries or /sites/<domain-name>/libraries, looks for it in the module directory itself.
  $tools = array_merge($tools, array_keys(file_scan_directory(drupal_get_path('module', 'pdf_using_mpdf'), $pattern)));

  if (isset($tools[0])) {
    require_once $tools[0];

    // Creating Instance of mPDF Class Library.
    $mpdf = new mPDF('', array($paper_size[$page]['w'], $paper_size[$page]['h']), $font_size, '', 15, 15, 16, 16, 9, 9);
    $header_left = variable_get('pdf_header_left');
    $header_center = variable_get('pdf_header_center');
    $header_right = variable_get('pdf_header_right');

    // Enabling header option if available.
    if ($header_left != NULL || $header_center != NULL || $header_right != NULL) {
      $header = array(
        'odd' => array(
          'L' => array(
            'content' => $header_left,
            'font-size' => 9,
            'font-style' => 'B',
            'font-family' => 'serif',
            'color' => 'gray',
          ),
          'C' => array(
            'content' => $header_center,
            'font-size' => 9,
            'font-style' => 'B',
            'font-family' => 'serif',
            'color' => 'gray',
          ),
          'R' => array(
            'content' => $header_right,
            'font-size' => 9,
            'font-style' => 'B',
            'font-family' => 'serif',
            'color' => 'gray',
          ),
          'line' => 1,
        ),
        'even' => array(),
      );
      $mpdf->SetHeader($header);
    }

    $footer_left = variable_get('pdf_footer_left');
    $footer_center = variable_get('pdf_footer_center');
    $footer_right = variable_get('pdf_footer_right');
    // Enabling Footer option if available.
    if ($footer_left != NULL || $footer_center != NULL || $footer_right != NULL) {
      $footer = array(
        'odd' => array(
          'L' => array(
            'content' => $footer_left,
            'font-size' => 9,
            'font-style' => 'B',
            'font-family' => 'serif',
            'color' => 'gray',
          ),
          'C' => array(
            'content' => $footer_center,
            'font-size' => 9,
            'font-style' => 'B',
            'font-family' => 'serif',
            'color' => 'gray',
          ),
          'R' => array(
            'content' => $footer_right,
            'font-size' => 9,
            'font-style' => 'B',
            'font-family' => 'serif',
            'color' => 'gray',
          ),
          'line' => 1,
        ),
        'even' => array(),
      );
      $mpdf->SetFooter($footer);
    }

    // Setting Watermark Text to PDF.
    $text = variable_get('pdf_watermark_text');
    if (isset($text) && $text != NULL) {
      // '0.1' opacity level (from 0.1 to 0.9).
      $mpdf->SetWatermarkText($text, '0.1');
      $mpdf->showWatermarkText = TRUE;
    }
    // Setting Author to PDF.
    $author = variable_get('pdf_set_author');
    if (isset($author) && $author != NULL) {
      $mpdf->SetAuthor($author);
    }
    // Setting Subject to PDF.
    $subject = variable_get('pdf_set_subject');
    if (isset($subject) && $subject != NULL) {
      $mpdf->SetSubject($subject);
    }

    // Setting Password to PDF.
    $password = variable_get('pdf_password');
    if (isset($password) && $password != NULL) {
      // Print and Copy is allowed.
      $mpdf->SetProtection(array('print', 'copy'), $password, $password);
    }

    // Setting CSS stylesheet to PDF.
    $stylesheet = variable_get('pdf_css_file');

    if (isset($stylesheet) && $stylesheet != NULL) {
      $css_file = file_scan_directory($root_path, '/.*\.css$/');
      if (isset($css_file[$root_path . '/' . $stylesheet])) {
        $stylesheet = file_get_contents($root_path . '/' . $stylesheet);
        $mpdf->WriteHTML($stylesheet, 1);
      }
      else {
        drupal_set_message(t('CSS style Sheet mentioned in PDF setting was not found.'), 'warning');
        return;
      }
    }
    // Writing html content for pdf buffer.
    $mpdf->WriteHTML($html);

    // Generating PDF File.
    if (variable_get('pdf_save_option') == '0') {
      // Open in same browser.
      $mpdf->Output($filename . '.pdf', 'I');
      die;
    }
    else {
      // Dialog box for Download PDF.
      $mpdf->Output($filename . '.pdf', 'D');
    }
  }
  else {
//    drupal_set_message(t('mPDF library is not included. Please check your " ' . MPDF_LIB_PATH . ' " directory or " /sites/your-domain-name/libraries/ directory " or " ' . MPDF_MODULE_PATH . ' " directory.'), 'error');
    return;
  }
}
