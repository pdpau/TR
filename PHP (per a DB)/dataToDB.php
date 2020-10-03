<?php
// conexió amb la DB
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "name_DB";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}



// declaro les variables on guardaré les dades que m'arribin amb el mètode POST
// entre [] es posa l'atribut
$dispositivo = $_POST ['disp'];
$temperatura = $_POST ['temp'];
$humedad = $_POST ['hum'];


// inserto dades a la db
$sql = "INSERT INTO `dades` (`id`, `fecha`, `dispositivo`, `temperatura`, `humedad`) 
VALUES (NULL, current_timestamp(), '$dispositivo', '$temperatura', '$humedad');";

$result = $conn->query($sql);

// tanco la conexió amb la db
$conn->close();

echo "Datos ingresados correctamente";

?>