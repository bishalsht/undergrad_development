<?php

$postparams = json_decode(file_get_contents('php://input'));

$returnObj = new stdClass();
if (!isset($postparams->ParkingPlaceId)) {
	$returnObj->status = false;
	$returnObj->message = "params not provided";

	echo json_encode($returnObj, JSON_PRETTY_PRINT);
	exit();
}

require 'dbconfig.php';

$sql = "SELECT * FROM Registered_Parking_Places WHERE ParkingPlace_Id ='".$postparams->ParkingPlaceId."' ";
$result = $database->query($sql);

if($result->num_rows == 0)
{
	$returnObj->status = false;
	$returnObj->message = "no parking lot of the given id";
}
else
{	
	$returnObj->status = true;
	$returnObj->message = "";
	$row = $result->fetch_assoc();
	$returnObj->ParkingPlace_Name = $row['ParkingPlace_Name'];	
	$returnObj->Address = $row['Address'];	
	$returnObj->Latitude = $row['Latitude'];	
	$returnObj->Longitude = $row['Longitude'];	
	$returnObj->Two_Wheeler_Capacity = $row['Two_Wheeler_Capacity'];	
	$returnObj->Two_wheeler_Rate = $row['Two_wheeler_Rate'];	
	$returnObj->Four_wheeler_Capacity = $row['Four_wheeler_Capacity'];	
	$returnObj->Four_wheeler_Rate = $row['Four_wheeler_Rate'];	
	$returnObj->Verified = $row['Verified'];	
}

echo json_encode($returnObj, JSON_PRETTY_PRINT);

mysqli_close($database);
?>