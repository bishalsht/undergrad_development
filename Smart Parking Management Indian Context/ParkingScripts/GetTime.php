<?php

$postparams = json_decode(file_get_contents('php://input'));

$returnObj = new stdClass();

if (!isset($postparams->User_Id )) {
	$returnObj->status = false;
	$returnObj->message = "missing params";
	$returnObj->arrival_timestamp = "";
	echo json_encode($returnObj, JSON_PRETTY_PRINT);
	exit();
}

require 'dbconfig.php';

$sql = "SELECT * FROM Transaction_Table WHERE User_Id ='".$postparams->User_Id."' and Arrival_Time > 0 and Departure_Time = -1";
$result = $database->query($sql);

if($result->num_rows == 0)
{
	$returnObj->status = false;
	$returnObj->message = "no bookings under the user";
	$returnObj->arrival_timestamp = "";
}
else
{	
	$returnObj->status = true;
	$returnObj->arrival_timestamp = $result->fetch_assoc()['Arrival_Time'];
	$returnObj->message = "";
}

echo json_encode($returnObj, JSON_PRETTY_PRINT);
?>