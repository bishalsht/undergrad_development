<?php

require 'predict.php';

$prediction = new Prediction("1", "2");

echo "calculated availability: ".$prediction->predict(1461699684)."<br><br>";

echo $prediction;

?>