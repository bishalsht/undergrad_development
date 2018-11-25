<?php

$postparams = json_decode(file_get_contents('php://input'));

$returnObj = new stdClass();

if (!isset($postparams->User_Id)) {
	$returnObj->status = false;
	$returnObj->message = "params missing";
	echo json_encode($returnObj, JSON_PRETTY_PRINT);
	exit();
}

require 'dbconfig.php';

$sql = "UPDATE Transaction_Table SET Arrival_Time = ".time()." WHERE `User_Id` = ".$postparams->User_Id." and Arrival_Time < 0";
$result = $database->query($sql);

$returnObj->status = true;
$returnObj->message = "";

echo json_encode($returnObj, JSON_PRETTY_PRINT);
file_put_contents("abc.txt", json_encode($returnObj, JSON_PRETTY_PRINT));


?>