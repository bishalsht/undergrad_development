<?php

$retObj = new stdClass();

$postparams = json_decode(file_get_contents('php://input'));

if (!isset($postparams->name) && !isset($postparams->mobile) && !isset($postparams->email) && !isset($postparams->photourl)) {
	$retObj->status = false;
	$retObj->message = "parameters missing";
	echo json_encode($retObj, JSON_PRETTY_PRINT);
	exit();
}

require 'dbconfig.php';

$sql = "SELECT * FROM Registered_Users WHERE Email_Id ='".$postparams->email."'";
$result = $database->query($sql);

if($result->num_rows > 0)
{
	$retObj->status = true;
	$retObj->message = "preexisting user";
	$retObj->userid = mysqli_fetch_row($result)[0];
}
else
{
	$insert_sql = "INSERT INTO Registered_Users (`User_Name`, `Mobile_Number`, `Email_Id`, `Photo_Url`) VALUES ('".$postparams->name."', '".$postparams->mobile."', '".$postparams->email."', '".$postparams->photourl."');";
	mysqli_query($database,$insert_sql);

	$retObj->userid = mysqli_insert_id($database);

	mysqli_close($database);
	$retObj->status = true;
	$retObj->message = "new user added";
}

echo json_encode($retObj, JSON_PRETTY_PRINT);
?>