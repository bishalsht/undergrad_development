<?php

$postparams = json_decode(file_get_contents('php://input'));

if (!isset($postparams->User_Id ) || !isset($postparams->ParkingPlace_Id) || !isset($postparams->Booked_Time) || !isset($postparams->Type_of_Vehicle)) {
	
	$returnObj = new stdClass();
	$returnObj->status = false;
	$returnObj->transaction_id = "";
	$returnObj->message = "parameters missing";

	echo json_encode($returnObj, JSON_PRETTY_PRINT);

	exit();
}

require 'dbconfig.php';

$curr_timestamp = time();

$insert_sql = "INSERT INTO `Transaction_Table` (`User_Id`, `ParkingPlace_Id`, `Transaction_Time`, `Booked_Time`, `Arrival_Time`, `Departure_Time`, `Type_of_Vehicle`, `Transaction_Cost`) VALUES ('".$postparams->User_Id."', '".$postparams->ParkingPlace_Id."', '".$curr_timestamp."', '".$postparams->Booked_Time."', '-1', '-1', '".$postparams->Type_of_Vehicle."', '0');";

$database->query($insert_sql);

$sql = "SELECT LAST_INSERT_ID() as lid FROM Transaction_Table";
$result = $database->query($sql);
$transId = $result->fetch_assoc()['lid'];

$latlongSql = "SELECT `Latitude`, `Longitude` FROM `transaction_table` NATURAL JOIN `registered_parking_places` WHERE `Transaction_id` = '$transId'";
$latlongResult = $database->query($latlongSql);

$returnObj = new stdClass();
$returnObj->status = true;
$returnObj->transaction_id = $transId;
$returnObj->lat = $latlongResult->fetch_assoc()['Latitude'];
$returnObj->long = $latlongResult->fetch_assoc()['Longitude'];
$returnObj->message = "success";

echo json_encode($returnObj, JSON_PRETTY_PRINT);

mysqli_close($database);
?>
