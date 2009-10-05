<?php
// $Id: apachesolr_rdf.hooks.php,v 1.1.2.5 2009/08/10 15:16:52 drunkenmonkey Exp $

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard Drupal
 * manner.
 */

// TODO: Write examples

/**
 * Tell the apachesolr_rdf module about schemas for which the index and search
 * methods are implemented.
 *
 * @return
 * An associative array with schema ids as keys and associative
 * arrays with the schema data as values. The schema data consists of the
 * following items:
 * - name: A translated, human-readable name for the schema.
 * - description: A translated description of what the schema does, its
 *     specifics and what has to be done to use it.
 * - create_document
 * - search_form_alter
 * - execute_search
 * - schema_arguments_form (optional)
 * - index_options_form (optional)
 * - search_options_form (optional)
 *
 * The three functions have to be specified as associative arrays with the
 * following keys:
 * - module: The module defining this function.
 * - file: The file with the function definition, relative to the module's
 *     base directory.
 * - function: The name of the function.
 *
 * @see create_document
 * @see search_form_alter
 * @see execute_search
 * @see schema_arguments_form
 * @see index_options_form
 * @see search_options_form
 */
function apachesolr_rdf_apachesolr_rdf_schemas() {}

/**
 * Creates an Apache_Solr_Document from the specified resource.
 *
 * @param $uri A string specifying the indexed resource's URI.
 * @param $predicates An array of predicate URIs mapped to arrays of objects
 *        that are used for that subject/predicate pair. The string
 *        value and type of these objects can be extracted with the
 *        apachesolr_rdf_extract_object_string() function.
 * @param $info The settings (id, name, options, ...) of the index the document
 *        is created for.
 * @return An object of type Apache_Solr_Document, containing information
 *         built from the parameters.
 */
function create_document($uri, $predicates, $info) {}

/**
 * Alters the search form by adding advanced RDF search options and telling the
 * form to use the right submit function.
 *
 * @param $form As in hook_form_alter().
 * @param $form_state As in hook_form_alter().
 * @param $id The search id as specified in the settings.
 * @param $schema_args The schema arguments for the server this search is using.
 */
function search_form_alter(&$form, &$form_state, $id, $schema_args) {
  $form['#submit'] = array('apachesolr_rdf_search_submit');

  $advanced = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Search'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $advanced['context'] = array(
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#title' => t('Context'),
    '#autocomplete_path' => "apachesolr_rdf/context/autocomplete/$id",
  );

  $advanced['advanced_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['advanced'] = $advanced;
}

/**
 * Executes an Apache Solr RDF search.
 *
 * @param $info The settings of the executed search.
 * @param $keys The search keys.
 * @param $filters The filters that were specified by the user to be applied.
 * @param $sort The field the results should be sorted on.
 * @param $page The results page to display, for multi-page results.
 *
 * @return A themed string representing the search result.
 */
function execute_search($info, $keys, $filters, $sort, $page) {}

/**
 * Function for adding additional input fields to a server add/edit form.
 *
 * @param $form_state The current form state, as passed to the main form builder
 *        function.
 * @param $info The current settings of the relevant server (when editing).
 * @return A form array defining a sub-form for adding/editing schema-dependent
 *         settings of a server.
 *
 * @see schema_argumens_form_validate
 * @see schema_argumens_form_submit
 */
function schema_arguments_form(&$form_state, $info = NULL) {}

/**
 * Validate function for schema_arguments_form().
 *
 * Should work like a normal form_validate function.
 *
 * Gets called automatically, when defined, and doesn't have to be explicitly
 * specified in the schema array. The name is generated by appending "_validate"
 * to the name of the defined schema_arguments_form function.
 *
 * @param $sub_form The schema-dependent form as returned by the
 *        schema_arguments_form function.
 * @param $sub_form_values The $form_state['values'] array corresponding to this
 *        sub-form.
 * @param $prefix A prefix that has to be used when setting errors for this
 *        sub-form.
 *
 * @see schema_arguments_form
 */
function schema_arguments_form_validate($sub_form, &$sub_form_values, $prefix) {
  // Errors can be set this way:
  form_set_error($prefix . $field_name, $message);
}

/**
 * Submit function for schema_arguments_form().
 *
 * Instead of just executing code, these submit functions should return the
 * value that corresponds to the submitted values, usually an array. This
 * value will then be set as the server's schema_args value.
 * If this function is not specified, the raw array of form values will be used
 * directly.
 *
 * Gets called automatically, when defined, and doesn't have to be explicitly
 * specified in the schema array. The name is generated by appending "_submit"
 * to the name of the defined schema_arguments_form function.
 *
 * @param $sub_form_values The $form_state['values'] array corresponding to this
 *        sub-form.
 * @return A processed representation of the submitted form data.
 *
 * @see schema_arguments_form
 */
function schema_arguments_form_submit($sub_form_values) {
  $sub_form_values['foo'] = unserialize($sub_form_values['foo']);
  return $sub_form_values;
}

/**
 * Function for adding additional input fields to an index add/edit form.
 *
 * @param $form_state The current form state, as passed to the main form builder
 *        function.
 * @param $info The current settings of the relevant index (when editing).
 * @return A form array defining a sub-form for adding/editing schema-dependent
 *         settings of an index.
 *
 * For defining validation and submit functions for this sub-form, see
 * schema_arguments_form and its validation and submit function.
 */
function index_options_form(&$form_state, $info = NULL) {}

/**
 * Function for adding additional input fields to a search add/edit form.
 *
 * @param $form_state The current form state, as passed to the main form builder
 *        function.
 * @param $info The current settings of the relevant search (when editing).
 * @return A form array defining a sub-form for adding/editing schema-dependent
 *         settings of a search.
 *
 * For defining validation and submit functions for this sub-form, see
 * schema_arguments_form and its validation and submit function.
 */
function search_options_form(&$form_state, $info = NULL) {}
