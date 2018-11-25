<?php
set_time_limit(0);
require 'radarsearch.php';

$radius = "37010";

$latlong = "17.384125,78.470517";

$datacollection = new DataCollection(17.433959, 78.457611, 26);

echo $datacollection;

// $datacollection->radarsearch($latlong, $radius);

// print_r($datacollection->getNewCoordinates(17.384125,78.470517,1, 90));
// print_r($datacollection->getNewCoordinates(17.164235,87.876115,270));

?>