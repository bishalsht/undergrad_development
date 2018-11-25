<?php

require 'predict.php';

$prediction = new Prediction("1", "2");

// echo "calculated availability: ".$prediction->predict(1461699684)."<br><br>";

$prediction->train();

echo $prediction;

// require 'dbconfig.php';

// $sql = "select ParkingPlace_Id as id from registered_parking_places";

// $res = $database->query($sql);

// while ($val = $res->fetch_assoc()['id']){
// 	$newsql = "INSERT INTO `parking_place_weights`(`ParkingPlace_Id`) VALUES ('$val')";
// 	$database->query($newsql);
// }

?>