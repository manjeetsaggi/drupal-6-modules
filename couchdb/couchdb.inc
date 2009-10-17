<?php
// $Id: couchdb.inc,v 1.1 2009/07/14 13:45:41 damz Exp $

/**
 * @file
 * A highly optimized CouchDB wrapping class.
 */

/**
 * A CouchDB wrapping class.
 */
class CouchDB {

  /**
   * The base URL of the CouchDB server.
   */
  public $baseURL;

  /**
   * Additional cURL options to use.
   *
   * @var array
   */
  public $additionalCurlOptions = array();

  /**
   * The cURL handle.
   */
  protected $curlHandle = NULL;

  /**
   * The HTTP credential to use, or NULL if none are required.
   */
  public $httpAuthCredentials = NULL;

  /**
   * Constructor.
   */
  public function __construct($base_url) {
    $this->baseURL = $base_url;
  }

  /**
   * Initializes the cURL connection.
   */
  protected function curlInitialize() {
    if (!isset($this->curlHandle)) {
      $this->curlHandle = curl_init();
      $curl_options = $this->additionalCurlOptions + array(
        CURLOPT_URL => $this->baseURL,
        CURLOPT_FOLLOWLOCATION => TRUE,
        CURLOPT_MAXREDIRS => 5,
        CURLOPT_RETURNTRANSFER => TRUE,
        CURLOPT_SSL_VERIFYPEER => FALSE,
        CURLOPT_SSL_VERIFYHOST => FALSE,
        CURLOPT_HEADERFUNCTION => array(&$this, 'curlHeaderCallback'),
      );
      if (isset($this->httpAuthCredentials)) {
        $curl_options[CURLOPT_USERPWD] = $this->httpAuthCredentials;
      }
      curl_setopt_array($this->curlHandle, $this->additionalCurlOptions + $curl_options);
    }
  }

  /**
   * Execute a cURL request.
   */
  protected function curlExec($curl_options) {
    $this->curlInitialize();
    curl_setopt_array($this->curlHandle, $this->additionalCurlOptions + $curl_options);

    $this->headers = array();
    $this->content = curl_exec($this->curlHandle);
    $this->url = curl_getinfo($this->curlHandle, CURLINFO_EFFECTIVE_URL);
    $this->status = curl_getinfo($this->curlHandle, CURLINFO_HTTP_CODE);

    // Automatically decode JSON-encoded responses.
    if ($content = json_decode($this->content)) {
      $this->content = $content;
    }

    return $this->content;
  }

  /**
   * Reads headers.
   *
   * @param $curlHandler
   *   The cURL handler.
   * @param $header
   *   An header.
   */
  protected function curlHeaderCallback($curlHandler, $header) {
    if (preg_match('/^([^:]+): (.*)$/', $header, $matches)) {
      list(, $name, $value) = $matches;
      $this->headers[$name][] = $value;
    }

    // This is required by cURL.
    return strlen($header);
  }

  /**
   * Get the content retrieved by the last request.
   */
  public function getContent() {
    return $this->content;
  }

  /**
   * Get the headers of the response to the last request.
   */
  public function getHeaders() {
    return $this->headers;
  }

  /**
   * Get the headers of the response to the last request.
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Execute a GET request to the CouchDB server.
   *
   * @param $path
   *   The path to use, relative to the CouchDB URL.
   * @param $headers
   *   An array containing additional HTTP request headers, each formatted as
   *   "name: value".
   * @return
   *   The retrieved content, also available as $this->getContent().
   *   Headers are available as $this->getHeaders().
   */
  public function get($path, array $headers = array()) {
    return $this->curlExec(array(
      CURLOPT_URL => $this->baseURL . '/' . $path,
      CURLOPT_CUSTOMREQUEST => 'GET',
      CURLOPT_HTTPGET => TRUE,
      CURLOPT_NOBODY => FALSE,
      CURLOPT_HTTPHEADER => $headers,
    ));
  }

  /**
   * Execute a POST request to the CouchDB server.
   *
   * @param $path
   *   The path to use, relative to the CouchDB URL.
   * @param $values
   *   The values to send to the server.
   * @param $headers
   *   An array containing additional HTTP request headers, each formatted as
   *   "name: value".
   * @return
   *   The retrieved content, also available as $this->getContent().
   *   Headers are available as $this->getHeaders().
   */
  public function post($path, array $values = array(), array $headers = array()) {
    return $this->curlExec(array(
      CURLOPT_URL => $this->baseURL . '/' . $path,
      CURLOPT_CUSTOMREQUEST => 'POST',
      CURLOPT_POST => TRUE,
      CURLOPT_POSTFIELDS => $values,
      CURLOPT_HTTPHEADER => $headers,
    ));
  }

  /**
   * Execute a PUT request to the CouchDB server.
   *
   * @param $path
   *   The path to use, relative to the CouchDB URL.
   * @param $content
   *   The content to send to the server.
   * @return
   *   The retrieved content, also available as $this->getContent().
   *   Headers are available as $this->getHeaders().
   */
  public function put($path, $content) {
    // Prepare the content.
    $fp = fopen("php://temp", 'r+');
    fputs($fp, $content);
    rewind($fp);

    $result = $this->curlExec(array(
      CURLOPT_URL => $this->baseURL . '/' . $path,
      CURLOPT_CUSTOMREQUEST => 'PUT',
      CURLOPT_PUT => TRUE,
      CURLOPT_INFILE => $fp,
      CURLOPT_INFILESIZE => strlen($content),
      CURLOPT_HTTPHEADER => array(),
    ));

    fclose($fp);
    return $result;
  }

  /**
   * Execute a DELETE request to the CouchDB server.
   *
   * @param $path
   *   The path to use, relative to the CouchDB URL.
   * @return
   *   The retrieved content, also available as $this->getContent().
   *   Headers are available as $this->getHeaders().
   */
  public function delete($path) {
    return $this->curlExec(array(
      CURLOPT_URL => $this->baseURL . '/' . $path,
      CURLOPT_CUSTOMREQUEST => 'DELETE',
      CURLOPT_HTTPHEADER => array(),
    ));
  }

  /**
   * List databases available in the CouchDB server.
   */
  public function listDatabases() {
    return $this->get("_all_dbs");
  }

  /**
   * Create a database.
   *
   * @param $name
   *   The name of the database to create. Must to in the range: "a-z0-9_$()+-/".
   */
  public function createDatabase($name) {
    return $this->put(urlencode($name), '');
  }

  /**
   * Delete a database.
   *
   * @param $name
   *   The name of the database to delete. Must to in the range: "a-z0-9_$()+-/".
   */
  public function deleteDatabase($name) {
    return $this->delete(urlencode($name));
  }

  /**
   * Fetch information about a database.
   *
   * @param $name
   *   The name of the database to delete. Must to in the range: "a-z0-9_$()+-/".
   */
  public function databaseInfo($name) {
    if (!empty($name)) {
      return $this->get(urlencode($name));
    }
    else {
      return NULL;
    }
  }

  /**
   * Fetch a CouchDB database object.
   *
   * @param $name
   *   The name of the database to delete. Must to in the range: "a-z0-9_$()+-/".
   */
  public function getDatabase($name) {
    return new CouchDBDatabase($this, $name);
  }
}

/**
 * An abstraction of a CouchDB Database.
 */
class CouchDBDatabase {
  public $info;

  public $name;

  function __construct($couchdb, $database_name) {
    // Fetch information about this database, bailout if the database doesn't exist.
    $this->info = $couchdb->databaseInfo($database_name);
    if (empty($this->info) || isset($this->info->error)) {
      throw new CouchDBException("Unable to fetch information about the database.");
    }

    // Store the couchdb object.
    $this->couchdb = $couchdb;

    // Store the database name.
    $this->name = urlencode($database_name);
  }

  /**
   * Fetch a document.
   */
  function getDocument($doc_id, $rev = NULL) {
    return $this->couchdb->get($this->name . '/' . urlencode($doc_id) . ($rev ? '?rev=' . urlencode($rev) : ''));
  }

  /**
   * Fetch the revision IDs associated to a document.
   */
  function getDocumentRevisions($doc_id) {
    return $this->couchdb->get($this->name . '/' . urlencode($doc_id) . '?revs_info=true')->_revs_info;
  }

  /**
   * Store a document.
   */
  function storeDocument($doc_id, $document) {
    return $this->couchdb->put($this->name . '/' . urlencode($doc_id), json_encode($document));
  }

  /**
   * Delete a document.
   */
  function deleteDocument($doc_id, $rev) {
    return $this->couchdb->delete($this->name . '/' . urlencode($doc_id) . '?rev=' . urlencode($rev));
  }
}

/**
 * A simple CouchDB-specific exception.
 */
class CouchDBException extends Exception { }
