<?php
$servername = "localhost";
$dBUsername = "root";
$dBPassword = "";
$dBName = "practice";

$conn = mysqli_connect($servername, $dBUsername, $dBPassword, $dBName);
if (!$conn) {
	die("Connection failed: ".mysqli_connect_error());
}

/* Handle CORS */
header('Access-Control-Allow-Origin: *');
header("Access-Control-Allow-Headers: *");

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

        $response = ''; 
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

    } else {
        echo 'Invalid JSON';
    }
}

// Handle all GET Requests
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    
    // Handle all GET Requests with query parameter "device_id" and "type" for last 10 records
    if (isset($_GET['device_id']) && isset($_GET['type']) && isset($_GET['limit'])) {
	    $device_id = $_GET['device_id'];
	    $type = $_GET['type'];
	    $limit = $_GET['limit'];

        $sql = "SELECT * FROM events WHERE device_id = '$device_id' and type = '$type' ORDER BY id DESC LIMIT $limit;";
        $result   = mysqli_query($conn, $sql);

        if($result) 
        {   
            if($limit > 1) {
                // Store the results in an array
                $response = array();
                while ($row = $result->fetch_assoc()) {
                    $response[] = $row;
                }
            } else {
                $sql = "SELECT * FROM events WHERE device_id = '$device_id' and type = '$type' ORDER BY id DESC LIMIT 1;";
                $result   = mysqli_query($conn, $sql);
                if (mysqli_num_rows($result) > 0)
                {
                    $row  = mysqli_fetch_assoc($result);
                    $response = [ 'id' => $row['id'] , 'device_id' => $row['device_id'], 'type' => $row['type'], 'value' => $row['value'], 'created' => $row['created'] ]; 
                } 
                else
                    $response = [];
            } 

            header('Content-Type: application/json; charset=utf-8');
            echo json_encode($response);
        }

    }  else if (isset($_GET['device_id']) && isset($_GET['type'])) { // Handle all GET Requests with query parameter "device_id" and "type" for last 10 records
	    $device_id = $_GET['device_id'];
	    $type = $_GET['type'];

        $sql = "SELECT * FROM events WHERE device_id = '$device_id' and type = '$type' ORDER BY id DESC LIMIT 10;";
        $result   = mysqli_query($conn, $sql);

        // Store the results in an array
        $response = array();
        while ($row = $result->fetch_assoc()) {
            $response[] = $row;
        }

        header('Content-Type: application/json; charset=utf-8');
        echo json_encode($response);

    }  else if (isset($_GET['device_id'])) { // Handle all GET Requests with query parameter "device_id"
	    $device_id = $_GET['device_id'];

        $sql1 = "SELECT * FROM events WHERE device_id = '$device_id' and type = 'Temperature' ORDER BY id DESC LIMIT 1;";
        $result1   = mysqli_query($conn, $sql1);
        if (mysqli_num_rows($result1) > 0)
        {
            $row1  = mysqli_fetch_assoc($result1);
            $response1 = [ 'id' => $row1['id'] , 'device_id' => $row1['device_id'], 'type' => $row1['type'], 'value' => $row1['value'], 'created' => $row1['created'] ]; 
        }

        $sql2 = "SELECT * FROM events WHERE device_id = '$device_id' and type = 'Humidity' ORDER BY id DESC LIMIT 1;";
        $result2   = mysqli_query($conn, $sql2);
        if (mysqli_num_rows($result2) > 0)
        {
            $row2  = mysqli_fetch_assoc($result2);
            $response2 = [ 'id' => $row2['id'] , 'device_id' => $row2['device_id'], 'type' => $row2['type'], 'value' => $row2['value'], 'created' => $row2['created'] ];
        }

        $sql3 = "SELECT * FROM events WHERE device_id = '$device_id' and type = 'Light' ORDER BY id DESC LIMIT 1;";
        $result3   = mysqli_query($conn, $sql3);
        if (mysqli_num_rows($result3) > 0)
        {
            $row3  = mysqli_fetch_assoc($result3);
            $response3 = [ 'id' => $row3['id'] , 'device_id' => $row3['device_id'], 'type' => $row3['type'], 'value' => $row3['value'], 'created' => $row3['created'] ];
        }

        
        header('Content-Type: application/json; charset=utf-8');
        $response = array($response1,$response2,$response3);
        echo json_encode($response);
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'PUT') {
        if (isset($_GET['device_id'])) {
        // Takes raw data from the request
        $json = file_get_contents('php://input');
        // echo $json;

        // // Converts it into a PHP object
        $data = json_decode($json);

        if (json_last_error() == JSON_ERROR_NONE) {
            $id = $data->id;
            $device_id = $data->device_id;
            $type = $data->type;
            $value = $data->value;
            $created = date('Y-m-d H:i:s', strtotime(' + 4 hours'));

            $response = ''; 
            $query = "UPDATE events SET created = '$created' WHERE id = $id;";

            // echo $query;
            if (mysqli_query($conn, $query)) {
                $response = [ 'response' => 'Data updated successfully :)' ];
            } else {
                $response = [ 'response' => "Error: " . $query . "<br>" . mysqli_error($conn) ];
                header("HTTP/1.1 500 NOT OK");
            }
            
            header('Content-Type: application/json; charset=utf-8');
            echo json_encode($response);

        } else {
            echo 'Invalid JSON';
        }
    }
}

?>