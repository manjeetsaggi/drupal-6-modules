<pre><?php
require_once './couchdb.class.php';

$couch = new CouchDB();
$couch->baseURL = "http://localhost:5984";

$couch->deleteDatabase("test-damz");
$couch->createDatabase("test-damz");
$database = $couch->getDatabase("test-damz");

print_r($database);

print_r($database->storeDocument("test", array('toto' => 'titi')));

print_r($database->getDocument("test"));
