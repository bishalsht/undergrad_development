<?php
$postparams = json_decode(file_get_contents('php://input'));

$retObj = new stdClass();

if (!isset($postparams->User_Id )) {
	$retObj->status = false;
	$retObj->message = "missing params";
	echo json_encode($retObj, JSON_PRETTY_PRINT);

	exit();
}
require 'dbconfig.php';

$transactionQuery = $database->query("SELECT ParkingPlace_Id, Type_of_Vehicle, Arrival_Time FROM Transaction_Table WHERE User_Id = ".$postparams->User_Id." AND Arrival_Time > 0 AND Departure_Time < 0");
$transactionRow = $transactionQuery->fetch_assoc();

if ($transactionRow == null) {
	$retObj->status = false;
	$retObj->message = "no such transaction";
	echo json_encode($retObj, JSON_PRETTY_PRINT);

	exit();
}

$informationSql = "SELECT `ParkingPlace_Name`, `Address`, ";
$informationSql .= ($transactionRow['Type_of_Vehicle'] == '2')?"`Two_Wheeler_Rate`" : "`Four_Wheeler_Rate`";
$informationSql .= " as rate FROM `registered_parking_places` WHERE `ParkingPlace_Id` = '".$transactionRow['ParkingPlace_Id'] ."' AND `Verified` = '1'";

$informationQuery = $database->query($informationSql);
$informationRow = $informationQuery->fetch_assoc();

if ($informationRow == null) {
	$retObj->status = false;
	$retObj->message = "no verified parking lot";
	echo json_encode($retObj, JSON_PRETTY_PRINT);

	exit();
}

$dept_time = time();
$parkedTime = $dept_time-$transactionRow['Arrival_Time'];
$cost = $informationRow['rate']*ceil((($parkedTime)/(60*60)));
$updateSql = "UPDATE Transaction_Table SET Departure_Time = '".$dept_time."', Transaction_Cost = '".$cost."' WHERE User_Id = '".$postparams->User_Id."' AND Arrival_Time > 0 AND Departure_Time < 0";
$database->query($updateSql);

$hrs = ((floor($parkedTime/(60*60)) < 10)? "0".(floor($parkedTime/(60*60))):(floor($parkedTime/(60*60))));
$mins = ((floor(($parkedTime%(60*60))/60)<10)?"0".(floor(($parkedTime%(60*60))/60)):(floor(($parkedTime%(60*60))/60)));
$secs = (($parkedTime%60<10)?"0".($parkedTime%60):($parkedTime%60));
$ParkedTimeString =  "$hrs:$mins:$secs";

$retObj->status = true;
$retObj->message = "";
$retObj->parkingplace_name = $informationRow['ParkingPlace_Name'];
$retObj->address = $informationRow['Address'];
$retObj->cost = $cost;
$retObj->parked_time = $ParkedTimeString;

echo json_encode($retObj, JSON_PRETTY_PRINT);

?>