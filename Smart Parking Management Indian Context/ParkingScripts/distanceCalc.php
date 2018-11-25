<?php

$postParams = json_decode(file_get_contents('php://input'));

$retObj = new stdClass();
$retObj->status = null;
$retObj->msg = null;


if (!isset($postParams->origins)) {
	$retObj->status = false;
	$retObj->msg = "origins not given";
	echo json_encode($retObj, JSON_PRETTY_PRINT);
	exit();
}
if (!isset($postParams->destinations)) {
	$retObj->status = false;
	$retObj->msg = "destinations not given";
	echo json_encode($retObj, JSON_PRETTY_PRINT);
	exit();
}

$postParams->origins = urlencode($postParams->origins);
$postParams->destinations = urlencode($postParams->destinations);

$url = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=$postParams->origins&destinations=$postParams->destinations&key=AIzaSyAzAY-FIjcSuI6-wv7GtmvwMYWFLe5FB-Q";

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
$response = curl_exec($ch);

curl_close($ch);

if ($response == false){
	$retObj->status = false;
	$retObj->msg = "server couldn't be contacted";
	echo json_encode($retObj, JSON_PRETTY_PRINT);
	exit();
}

$responseObj = json_decode($response);

if ($responseObj->status != "OK") {
	$retObj->status = false;
	$retObj->msg = "address error";
	echo json_encode($retObj, JSON_PRETTY_PRINT);
	exit();
}

if ($responseObj->rows[0]->elements[0]->status != "OK") {
	$retObj->status = false;
	$retObj->msg = "address error";
	echo json_encode($retObj, JSON_PRETTY_PRINT);
	exit();
}

$retObj->status = true;
if ($responseObj->rows[0]->elements[0]->distance->value < 40) {
	$retObj->isInPeriphery = true;
}
else {
	$retObj->isInPeriphery = false;
}
$retObj->msg = "";

echo json_encode($retObj, JSON_PRETTY_PRINT);

?>