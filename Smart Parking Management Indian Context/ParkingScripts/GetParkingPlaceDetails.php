<?php

require 'dbconfig.php';

$sql = "SELECT * FROM Registered_Parking_Places WHERE Verified = 1";

$returnObj = new stdClass();

if ($result = $database->query($sql)) {
	if ($result->num_rows < 1) {
		$returnObj->status = false;
		$returnObj->message = "no results found";
	}
	else {
		$returnObj->status = true;
		$returnObj->message = "";
		$returnObj->results = array();
		while ($row = $result->fetch_assoc()) {
			$tempObj = new stdClass();
			foreach ($row as $key => $value) {
				$tempObj->$key = $value;
			}
			array_push($returnObj->results, $tempObj);
		}
	}
    $result->close();	
}
else {
	$returnObj->status = false;
	$returnObj->message = "query fail";
}

echo json_encode($returnObj, JSON_PRETTY_PRINT);


?>