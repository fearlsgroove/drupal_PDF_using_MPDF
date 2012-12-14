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
function pdf_using_mpdf_generate_pdf($node) {
  global $theme;

  if (empty($node)) {
    drupal_get_messages('error');
    drupal_set_message(t('PDF cannot be generated for this path.'), 'error');
    return;
  }
  else {
    $theme_path = drupal_get_path('module', 'pdf_using_mpdf');
    $node = node_load($node->nid);

    $pdf_using_mpdf_pdf_filename = variable_get('pdf_using_mpdf_pdf_filename', PDF_USING_MPDF_PDF_DEFAULT_FILENAME);
    $pdf_using_mpdf_pdf_filename = token_replace($pdf_using_mpdf_pdf_filename, array('node' => $node));

    $view = node_view($node);
    $view['links']['node']['#links'] = array();
    $html = drupal_render($view);

    return _pdf_using_mpdf_generator($html, $pdf_using_mpdf_pdf_filename);
  }
}
