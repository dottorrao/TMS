<?php
//setting header to json
header('Content-Type: application/json');

//database
define('DB_HOST', 'fdb18.awardspace.net');
define('DB_USERNAME', '2563817_db');
define('DB_PASSWORD', '12Akela12');
define('DB_NAME', '2563817_db');

//get connection
$mysqli = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);

if(!$mysqli){
	die("Connection failed: " . $mysqli->error);
}

//query to get data from the table
$query = sprintf('SELECT timestamp, humidity, temperature FROM `tempLog` ORDER BY `timeStamp` ASC');

//execute query
$result = $mysqli->query($query);

//loop through the returned data
$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

//free memory associated with result
$result->close();

//close connection
$mysqli->close();

//now print the data
print json_encode($data);

?>