<?php

$servername = "localhost";
$dBUsername = "root";
$dBPassword = "";
$dBName = "practice";

$conn = mysqli_connect($servername, $dBUsername, $dBPassword, $dBName);
if (!$conn) {
	die("Connection failed: ".mysqli_connect_error());
}


if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Takes raw data from the request
    $json = file_get_contents('php://input');
    // echo $json;

    // // Converts it into a PHP object
    $data = json_decode($json);

    if (json_last_error() == JSON_ERROR_NONE) {
        $device_id = $data->device_id;
        $type = $data->type;
        $value = $data->value;

        $created = date('Y-m-d H:i:s', strtotime(' + 4 hours'));

        $query = "INSERT INTO events (device_id, type, value, created) VALUES ('$device_id','$type','$value','$created');";
        
        if (mysqli_query($conn, $query)) {
            $response = [ 'response' => 'New data created successfully :)' ];
            header("HTTP/1.1 201 OK");
        } else {
            $response = [ 'response' => "Error: " . $query . "<br>" . mysqli_error($conn) ];
            header("HTTP/1.1 500 NOT OK");
        }
        
        header('Content-Type: application/json; charset=utf-8');
        echo json_encode($response);

    } 
    else 
        echo "JSON Invalid";
}

?>