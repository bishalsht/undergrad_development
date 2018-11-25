<?php

/**
* Class to automatically get all the public parking lots in a city
*/
class DataCollection
{

	private $server_key;

	//circle details
	private $lat;
	private $long;
	private $radius;

	//hexagon details
	private $hexagonRadius;
	private $lat1, $long1;
	private $lat2, $long2;
	private $lat3, $long3;
	private $lat4, $long4;
	private $lat5, $long5;
	private $lat6, $long6;

	//smaller hexagon details
	private $subRadius = 10;

	private $count = 0;


	/*

	   1********2
	   *		*
	  *			 *
	6*		******3
	  *		  ^  *
	   *	  ^ *
	   5******^*4
			  ^
			  ^
			  ^
		hexagonRadius
	*/
	
	function __construct($lat, $long, $radius)
	{
		// $this->server_key = "AIzaSyAzAY-FIjcSuI6-wv7GtmvwMYWFLe5FB-Q";
		$this->server_key = "AIzaSyCNnlQQoEkci99LfRzso1NHXDmVMlVx2No";

		$this->lat = $lat;
		$this->long = $long;
		$this->radius = $radius;

		$this->outerHexagonConstruction();
		$this->hexagonalTessellation();
	}

	function __toString()
	{
		$value = "Hexagon vertices' Latlong values<br>"
				."-----------------------------------------------<br>";
		$value .= $this->lat1.", ".$this->long1."<br>"
				.$this->lat2.", ".$this->long2."<br>"
				.$this->lat3.", ".$this->long3."<br>"
				.$this->lat4.", ".$this->long4."<br>"
				.$this->lat5.", ".$this->long5."<br>"
				.$this->lat6.", ".$this->long6."<br>";

		$value .= "Total insertions : $this->count";

		return $value;
	}

	private function outerHexagonConstruction()
	{
		$this->hexagonRadius = (2*$this->radius)/(sqrt(3));

		$mark = $this->getNewCoordinates($this->lat, $this->long, $this->radius, 0);

		$latlong = $this->getNewCoordinates($mark[0], $mark[1], $this->hexagonRadius/2, 270);
		$this->lat1 = $latlong[0];
		$this->long1 = $latlong[1];

		$latlong = $this->getNewCoordinates($mark[0], $mark[1], $this->hexagonRadius/2, 90);
		$this->lat2 = $latlong[0];
		$this->long2 = $latlong[1];

		$latlong = $this->getNewCoordinates($this->lat, $this->long, $this->hexagonRadius, 90);
		$this->lat3 = $latlong[0];
		$this->long3 = $latlong[1];

		$latlong = $this->getNewCoordinates($this->lat2, $this->long2, 2*$this->radius, 180);
		$this->lat4 = $latlong[0];
		$this->long4 = $latlong[1];

		$latlong = $this->getNewCoordinates($this->lat4, $this->long4, $this->hexagonRadius, 270);
		$this->lat5 = $latlong[0];
		$this->long5 = $latlong[1];

		$latlong = $this->getNewCoordinates($this->lat, $this->long, $this->hexagonRadius, 270);
		$this->lat6 = $latlong[0];
		$this->long6 = $latlong[1];
	}

	private function hexagonalTessellation()
	{
		$lat = $this->lat1;
		$long = $this->long1;

		$latitude;
		$longitude;

		$dist = (sqrt(3)*$this->subRadius)/2;

		$smallestIteration = ceil($this->HaversineDistance($this->lat1, $this->long1, $this->lat2, $this->long2)/(2*$this->subRadius));
		$largestIteration = ceil($this->HaversineDistance($this->lat3, $this->long3, $this->lat6, $this->long6)/(2*$this->subRadius));

		$flipped = false;
		$currentIteration = $smallestIteration;

		while (true) 
		{
			$latitude = $lat;
			$longitude = $long;
			$this->radarSearch($latitude.",".$longitude, $this->subRadius*1000);

			for ($i=0; $i < $currentIteration; $i++) 
			{ 
				$latlong = $this->getNewCoordinates($lat, $long, $dist*2, 90);
				$lat = $latlong[0];
				$long = $latlong[1];
				$this->radarSearch($lat.",".$long, $this->subRadius*1000);
			}

			if (!$flipped) 
			{
				$currentIteration++;
				$temp1 = $this->getNewCoordinates($latitude, $longitude, (3*$this->subRadius)/2, 180);
				$temp2 = $this->getNewCoordinates($temp1[0], $temp1[1], $dist, 270);
				$lat = $temp2[0];
				$long = $temp2[1];
			}
			else
			{
				$currentIteration--;	
				$temp1 = $this->getNewCoordinates($latitude, $longitude, (3*$this->subRadius)/2, 180);
				$temp2 = $this->getNewCoordinates($temp1[0], $temp1[1], $dist, 90);
				$lat = $temp2[0];
				$long = $temp2[1];
			} 

			if ($largestIteration<$currentIteration)
			{
				$flipped = true;
				$currentIteration -= 2;
				$temp1 = $this->getNewCoordinates($latitude, $longitude, (3*$this->subRadius)/2, 180);
				$temp2 = $this->getNewCoordinates($temp1[0], $temp1[1], $dist, 90);
				$lat = $temp2[0];
				$long = $temp2[1];
			}

			if ($flipped && $currentIteration<$smallestIteration) break;
		}
	}

	private function radarSearch($latlong, $radius)
	{
		$type = "parking"
		."|airport"
		."|hospital"
		."|shopping_mall"
		."|amusement_park"
		."|zoo"
		."|train_station"
		."|stadium"
		."|movie_theater";

		$url = "https://maps.googleapis.com/maps/api/place/radarsearch/json?location=".$latlong."&type=".$type."&radius=".$radius."&key=".$this->server_key;

		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		$response = curl_exec($ch);

		curl_close($ch);

		$responseObj = json_decode($response);

		$i = 0;
		foreach ($responseObj->results as $value) {
			try {
				$this->placeDetails($value->place_id);
			}
			catch (Exception $e) {
				echo $this;
				exit();
			}
		}

	}

	private function placeDetails($placeid)
	{
		$url = "https://maps.googleapis.com/maps/api/place/details/json?placeid=".$placeid."&key=".$this->server_key;

		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		$response = curl_exec($ch);

		curl_close($ch);

		if ($response == false) {
			throw new Exception("Google Server couldn't be contacted");
		}
		$responseObj = json_decode($response);
		
		if ($responseObj->status != "OK") {
			throw new Exception($responseObj->error_message);
		}

		$name = $responseObj->result->name;
		$address = $responseObj->result->formatted_address;
		$lat = $responseObj->result->geometry->location->lat;
		$long = $responseObj->result->geometry->location->lng;

		$this->insertData($name, $address, $lat, $long, $placeid);
	}

	private function insertData($name, $address, $lat, $long, $placeid)
	{
		require 'dbconfig.php';

		$sql = "INSERT INTO `parking_manager`.`registered_parking_places` (`ParkingPlace_Name`, `Address`, `Latitude`, `Longitude`, `Two_Wheeler_Capacity`, `Two_wheeler_Rate`, `Four_wheeler_Capacity`, `Four_wheeler_Rate`, `Verified`, `GooglePlaceId`) VALUES ('$name', '$address', '$lat', '$long', '', '', '', '', '0', '$placeid');";

		if ($database->query($sql) == false)
		file_put_contents ("collision_log.txt", $placeid." ".$name."\n", FILE_APPEND);
		else $this->count++;
	}

	private function HaversineDistance($lat1, $long1, $lat2, $long2)
	{
		$earthRadius = 6378.14; //km
		$latDist = deg2rad($lat2-$lat1);
		$longDist = deg2rad($long2-$long1);

		$a = sin($latDist/2) * sin($latDist/2) + 
			cos(deg2rad($lat1)) * cos(deg2rad($lat2)) *
			sin($longDist/2) * sin($longDist/2);

		$c = 2*atan2(sqrt($a), sqrt(1-$a));
		$d = $earthRadius*$c;
		return $d;
	}

	private function getNewCoordinates($lat1,$long1,$d,$angle)
	{
		/*
		0 => up
		90 => right
		180 => down
		270 => left
		*/

	    # Earth Radious in KM
	    $R = 6378.14;

	    # Degree to Radian
	    $latitude1 = deg2rad($lat1);
	    $longitude1 = deg2rad($long1);
	    $brng = deg2rad($angle);

	    $latitude2 = asin(sin($latitude1)*cos($d/$R) + cos($latitude1)*sin($d/$R)*cos($brng));
	    $longitude2 = $longitude1 + atan2(sin($brng)*sin($d/$R)*cos($latitude1),cos($d/$R)-sin($latitude1)*sin($latitude2));

	    # back to degrees
	    $latitude2 = rad2deg($latitude2);
	    $longitude2 = rad2deg($longitude2);

		# 6 decimal for Leaflet and other system compatibility
		$lat2 = round ($latitude2,6);
		$long2 = round ($longitude2,6);

		// Push in array and get back
		$tab[0] = $lat2;
		$tab[1] = $long2;
		return $tab;
	}
}

?>