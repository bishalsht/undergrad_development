<?php

require 'predict.php';

$prediction = new Prediction("1", "2");

$prediction->train();

echo $prediction;

?>