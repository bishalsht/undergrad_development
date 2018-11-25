<?php

include 'dbconfig.php';

// sql to create table
$sqltable = "CREATE TABLE MyGuests (
id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, 
firstname VARCHAR(30) NOT NULL,
lastname VARCHAR(30) NOT NULL,
email VARCHAR(50),
reg_date TIMESTAMP
)";

if ($database->query($sqltable) === TRUE) {
    echo "Table MyGuests created successfully";
} else {
    echo "Error creating table: " . $database->error;
}

$database->close();
?>