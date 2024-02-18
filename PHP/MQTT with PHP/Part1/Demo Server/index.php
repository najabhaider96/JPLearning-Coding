<?php

require('vendor/autoload.php');

use \PhpMqtt\Client\MqttClient;

// DB
$servername = "localhost";
$dBUsername = "root";
$dBPassword = "";
$dBName = "practice";

$conn = mysqli_connect($servername, $dBUsername, $dBPassword, $dBName);
if (!$conn) {
	die("Connection failed: ".mysqli_connect_error());
} else {
    printf("Database connected\n");
}

// MQTT Credentials
// EMQX Public
$server   = 'broker.emqx.io';
$port     = 1883;
$clientId = 'JPLearning_Application';
// $username = 'emqx_user';
// $password = 'password';
// $clean_session = false;
// $mqtt_version = MqttClient::MQTT_3_1_1;

// $connectionSettings = (new ConnectionSettings)
//   ->setUsername($username)
//   ->setPassword($password)
//   ->setKeepAliveInterval(60)
//   ->setLastWillTopic('last-will')
//   ->setLastWillMessage('client disconnect')
//   ->setLastWillQualityOfService(1);

// $mqtt = new MqttClient($server, $port, $clientId, $mqtt_version);
// $mqtt->connect($connectionSettings, $clean_session);


$mqtt = new MqttClient($server, $port, $clientId);
$mqtt->connect();
printf("MQTT connected\n");

$mqtt->subscribe('JPLearning_SensorData', function ($topic, $message) use ($conn) {
    printf("\n\n\nReceived message on topic [%s]: %s\n", $topic, $message);
    $data = json_decode($message);

    printf("device_id: %s\n", $data->device_id);
    printf("type: %s\n", $data->type);
    printf("value: %s\n", $data->value);

    $device_id = $data->device_id;
    $type = $data->type;
    $value = $data->value;
    $created = date('Y-m-d H:i:s', strtotime(' + 4 hours'));

    $query = "INSERT INTO events (device_id, type, value, created) VALUES ('$device_id','$type','$value','$created');";
    mysqli_query($conn, $query);
    printf("Data Stored Successfully :)");
}, 0);

$mqtt->loop(true);