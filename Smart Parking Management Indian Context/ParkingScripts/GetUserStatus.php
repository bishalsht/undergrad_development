<?php

$postparams = json_decode(file_get_contents('php://input'));

$returnObj = new stdClass();

if (!isset($postparams->User_Id )) {
	$returnObj->status = false;
	$returnObj->message = "missing params";
	echo json_encode($returnObj, JSON_PRETTY_PRINT);
	exit();
}

require 'dbconfig.php';

$sql = "SELECT `Arrival_Time`, `Departure_Time`, `ParkingPlace_Id`, `Latitude`, `Longitude` FROM `transaction_table` NATURAL JOIN `registered_parking_places` WHERE `User_Id` = '".$postparams->User_Id."' AND `Departure_Time` = '-1'";
$result = $database->query($sql);

$returnObj->status = true;

if ($result->num_rows == 0) {
	$returnObj->message = "notbooked";
}
else {
	$row = $result->fetch_assoc();

	if ($row['Arrival_Time']<0){
		$returnObj->message = "booked";
	}
	else if ($row['Arrival_Time']>0 && $row['Departure_Time']<0){
		$returnObj->message = "parkingstarted";
	}
	$returnObj->lat = $row['Latitude'];
	$returnObj->long = $row['Longitude'];
}

echo json_encode($returnObj, JSON_PRETTY_PRINT);
?>