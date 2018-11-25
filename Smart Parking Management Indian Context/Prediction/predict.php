<?php

/**
* Prediction Algorithm Class
* -------------------------------------------------------------------------------------------------------------
* This algorithm aims to predict a particular parking lot's availability in the near future when the booking is
* required.
*
* Parameters Used
* -------------------------------------------------------------------------------------------------------------
* 1. Capacity of parking lot
* 2. Day of week when the booking is required
* 3. Time of day when the booking is required
* 4. Current availability of parking lot
* 5. Time left till when the booking is required
* 6. Number of booking requests till the time when the booking is required
*
* Equation Followed
* -------------------------------------------------------------------------------------------------------------
* Predicted Availability = weightCapacity - weightTimeofDay * TimeofDayValue - weightDayofWeek * DayofWeekValue
* 						- weightCurrentAvailability * currentAvailability - weightFuturePredictionDistance * 
*						futurePredictionDistance - weightB * weightBVal
* 
* Least Mean Square Rule
* -------------------------------------------------------------------------------------------------------------
* weight = weight + n (TrainingAvailability - CalculatedAvailability) value
* 
*/
class Prediction
{
	
	/*
	* Weights
	*/
	private $weightCapacity;
	private $weightTimeofDay;
	private $weightDayofWeek;
	private $weightCurrentAvailability;
	private $weightBookingDistance;
	private $weightRequestCounts;

	/*
	* Values
	*/
	private $valueTimeofDay;
	private $valueDayofWeek;
	private $valueCurrentAvailability;
	private $valueBookingDistance;
	private $valueRequestCounts;

	private $calculatedAvailability;

	/*
	* For Prediction Only
	*/
	private $parkingLotId;
	private $typeOfVehicle;

	function __construct($parkingLotId, $typeOfVehicle)
	{
		$this->parkingLotId = $parkingLotId;
		$this->typeOfVehicle = $typeOfVehicle;

		$this->getWeights();
	}

	function __toString()
	{
		$val = "";
		$val .= "weightCapacity: ".$this->weightCapacity."<br>";
		$val .= "weightTimeofDay: ".$this->weightTimeofDay."<br>";
		$val .= "weightDayofWeek: ".$this->weightDayofWeek."<br>";
		$val .= "weightCurrentAvailability: ".$this->weightCurrentAvailability."<br>";
		$val .= "weightBookingDistance: ".$this->weightBookingDistance."<br>";
		$val .= "weightRequestCounts: ".$this->weightRequestCounts."<br>";

		$val .= "valueTimeofDay: ".$this->valueTimeofDay."<br>";
		$val .= "valueDayofWeek: ".$this->valueDayofWeek."<br>";
		$val .= "valueCurrentAvailability: ".$this->valueCurrentAvailability."<br>";
		$val .= "valueBookingDistance: ".$this->valueBookingDistance."<br>";
		$val .= "valueRequestCounts: ".$this->valueRequestCounts."<br>";

		$val .= "calculatedAvailability: ". $this->calculatedAvailability;

		return $val;
	}

	public function predict($timestamp)
	{
		$this->valueTimeofDay = $this->getHourOfDayValue($timestamp);
		$this->valueDayofWeek = $this->getDayOfWeekValue($timestamp);
		$this->valueCurrentAvailability = $this->getCurrentAvailabilityValue();
		$this->valueBookingDistance = $this->getBookingDistanceValue($timestamp);
		$this->valueRequestCounts = $this->getRequestCountsValue($timestamp);

		if ($this->valueBookingDistance == -1) die("past timestamp given");

		$this->calculatedAvailability = $this->weightCapacity - 
						$this->weightTimeofDay * $this->valueTimeofDay - 
						$this->weightDayofWeek * $this->valueDayofWeek -
						$this->weightCurrentAvailability * $this->valueCurrentAvailability -
						$this->weightBookingDistance * $this->valueBookingDistance - 
						$this->weightRequestCounts * $this->valueRequestCounts;

		require 'dbconfig.php';

		$sql = "INSERT INTO `prediction_training_data`
			(`ParkingPlace_Id`, `BookedTime`, `CalculatedAvailability`, `valueTimeofDay`, `valueDayofWeek`, `valueCurrentAvailability`, `valueBookingDistance`, `valueRequestCounts`) 
			VALUES ('".$this->parkingLotId."', '".$timestamp."', '".$this->calculatedAvailability."', '".$this->valueTimeofDay."', '".$this->valueDayofWeek."', '".$this->valueCurrentAvailability."', '".$this->valueBookingDistance."', '".$this->valueRequestCounts."')";

		$database->query($sql);

		return $this->calculatedAvailability;
	}	

	public function train()
	{
		require 'dbconfig.php';	

		$sql = "SELECT * FROM `prediction_training_data` WHERE `TrainingComplete` = '0' AND `BookedTime`< '".time()."' ORDER BY `BookedTime`";
		$result = $database->query($sql);

		while ($row = $result->fetch_assoc())
		{
			$trainingAvailability = $this->calculateActualAvailability($row['BookedTime']);
			$this->leastMeanSquare($row, $trainingAvailability);

			$newsql = "UPDATE `prediction_training_data` SET `TrainingAvailability`=".$trainingAvailability.",`TrainingComplete`='1' WHERE `rowId` = '".$row['rowId']."'";
			$database->query($newsql);
		}

		$updatesql = "UPDATE `parking_place_weights` SET `weightTimeofDay`=".$this->weightTimeofDay.",`weightDayofWeek`=".$this->weightDayofWeek.",`weightCurrentAvailability`=".$this->weightCurrentAvailability.",`weightBookingDistance`=".$this->weightBookingDistance.",`weightRequestCounts`=".$this->weightRequestCounts." WHERE `ParkingPlace_Id` = '".$this->parkingLotId."';";
		$database->query($updatesql);
	}

	private function calculateActualAvailability($timestamp)
	{
		require 'dbconfig.php';

		$sql = "SELECT COUNT(*) as cnt
				FROM `transaction_table` 
				WHERE `Arrival_Time` < '$timestamp' AND (`Departure_Time` = '-1' OR `Departure_Time` > '$timestamp')";

		$result = $database->query($sql);
		$count = $this->weightCapacity - (int)$result->fetch_assoc()['cnt'];
		return $count;
	}

	private function leastMeanSquare($row, $trainingAvailability)
	{
		$n = 0.001;
		$val = $this->weightTimeofDay + $n * ($trainingAvailability - $row['CalculatedAvailability']) * $row['valueTimeofDay'];
		$this->weightTimeofDay = $val;
		$this->weightDayofWeek = $this->weightDayofWeek + $n * ($trainingAvailability - $row['CalculatedAvailability']) * $row['valueDayofWeek'];
		$this->weightCurrentAvailability = $this->weightCurrentAvailability + $n * ($trainingAvailability - $row['CalculatedAvailability']) * $row['valueCurrentAvailability'];
		$this->weightBookingDistance = $this->weightBookingDistance + $n * ($trainingAvailability - $row['CalculatedAvailability']) * $row['valueBookingDistance'];
		$this->weightRequestCounts = $this->weightRequestCounts + $n * ($trainingAvailability - $row['CalculatedAvailability']) * $row['valueRequestCounts'];
	}

	private function getWeights()
	{
		require 'dbconfig.php';

		$sql = "SELECT * FROM `parking_place_weights` WHERE `ParkingPlace_Id` = '".$this->parkingLotId."'";
		$result = $database->query($sql);

		$row = $result->fetch_assoc();
		$this->weightTimeofDay = (double) $row['weightTimeofDay'];
		$this->weightDayofWeek = (double) $row['weightDayofWeek'];
		$this->weightCurrentAvailability = (double) $row['weightCurrentAvailability'];
		$this->weightBookingDistance = (double) $row['weightBookingDistance'];
		$this->weightRequestCounts = (double) $row['weightRequestCounts'];

		$newsql = "";
		if ($this->typeOfVehicle == '2') {
			$newsql = "SELECT `Two_Wheeler_Capacity` as cap FROM `registered_parking_places` WHERE `ParkingPlace_Id` = '".$this->parkingLotId."'";
		}
		else {
			$newsql = "SELECT `Four_Wheeler_Capacity` as cap FROM `registered_parking_places` WHERE `ParkingPlace_Id` = '".$this->parkingLotId."'";
		}

		$newresult = $database->query($newsql);
		$this->weightCapacity = (double) $newresult->fetch_assoc()['cap'];
	}

	private function getCurrentAvailabilityValue()
	{
		require 'dbconfig.php';

		$sql = "SELECT count(*) as cnt 
			FROM `transaction_table` 
			WHERE `Arrival_Time`!= '-1' 
				AND `Departure_Time`='-1' 
				AND `ParkingPlace_Id` = '".$this->parkingLotId."' 
				AND `Type_of_Vehicle` = '".$this->typeOfVehicle."'";

		$result = $database->query($sql);

		return $this->weightCapacity - (double) $result->fetch_assoc()['cnt'];
	}

	private function getRequestCountsValue($timestamp)
	{
		require 'dbconfig.php';

		$sql = "SELECT count(*) as cnt 
			FROM `transaction_table` 
			WHERE `Booked_Time`> '".time()."'
				AND `Booked_Time`< '$timestamp' 
				AND `ParkingPlace_Id` = '".$this->parkingLotId."' 
				AND `Type_of_Vehicle` = '".$this->typeOfVehicle."'";

		$result = $database->query($sql);

		return (double) $result->fetch_assoc()['cnt'];
	}

	private function getBookingDistanceValue($timestamp)
	{
		$dist = $timestamp - time();

		if ($dist < 0) return -1;

		return $dist/36000.0;
	}

	private function getDayOfWeekValue($timestamp)
	{
		$day = date('N', $timestamp);

		switch ($day) {
			case '1':
			case '2':
			case '3':
			case '4':
				return 5;
				break;
			case '5':
				return 10;
				break;
			case '6':
			case '7':
				return 20;
				break;
			default:
				return -1;
				break;
		}
	}

	private function getHourOfDayValue($timestamp)
	{
		$hour = date('G', $timestamp);

		if ($hour < 0) return -1;
		if ($hour >= 1 && $hour<=11) return 5;
		if ($hour >= 12 && $hour<=16) return 10;
		if ($hour >= 17 && $hour <= 22) return 20;
		if ($hour == 23 || $hour == 0) return 15;
		if ($hour > 23) return -1;
	}
}

?>