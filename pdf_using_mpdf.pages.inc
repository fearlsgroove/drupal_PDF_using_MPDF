<?php

/**
 * @file
 * Contains all description to generate PDF document from raw HTML content.
 */

/**
 * Generate HTML of a given node.
 *
 * @return bool
 *   TRUE if PDF is successfully generated and FALSE if it isn't.
 */
function pdf_using_mpdf_generate_pdf($node) {

  if (!node_access('view', $node)) {
    drupal_set_message(t('You are not authorized to generate PDF for this page.'), 'warning');
    drupal_goto(urlencode('node') . '/' . $node->nid);
    return;
  }

  if (empty($node)) {
    drupal_get_messages('error');
    drupal_set_message(t('PDF cannot be generated for this path.'), 'error');
    return;
  }
  else {
    $pdf_using_mpdf_pdf_filename = variable_get('pdf_using_mpdf_pdf_filename', PDF_USING_MPDF_PDF_DEFAULT_FILENAME);
    $pdf_using_mpdf_pdf_filename = token_replace($pdf_using_mpdf_pdf_filename, array('node' => $node));

    $view = node_view($node);
    $view['links']['node']['#links'] = array();
    $html = drupal_render($view);

    _pdf_using_mpdf_generator($html, $pdf_using_mpdf_pdf_filename);
  }
}
