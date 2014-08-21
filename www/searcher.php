<?php

/* Receive GET paramenters */
$model = $_GET["model"];
$query = $_GET["query"];

/* Socket parameters */
$MSG_SIZE = 10000;
$SOCKET_OK = "220";
$server_addres = '127.0.0.1';
$service_port = 10001;

/* Get the IP address for the target host. */
$address = gethostbyname($server_addres);

/* Create a TCP/IP socket. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP); // SOL_TCP
if ($socket === false) {
     echo "error\n";
    //echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}

/* Try to connect to address on port. */
//echo "Attempting to connect to '$address' on port '$service_port'...";
$result = socket_connect($socket, $address, $service_port);
if ($result === false) {
	 echo "error\n";    
    //echo "socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
}

/* Send <model,query> to application */
//echo "Sending Request";
$data = $model . "," . $query . "\n";
socket_write($socket, $data , strlen($data));

/* Receive json response */
$json = "";

while ($out = socket_read($socket, $MSG_SIZE, PHP_NORMAL_READ)) {    
    if($out == "\n")
	break;
    $json .= $out;
}

//echo "Closing socket...";
socket_close($socket);

/* Send HTTP response */

echo $json;
?>
