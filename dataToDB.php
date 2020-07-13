<?php

include ('conectarConDB.php');
global $varconexion;


// declaro les variables on guardaré les dades que m'arribin amb el mètode POST
// entre [] es posa l'atribut
$temperatura = $_POST ['temp'];
$humedad = $_POST ['hum'];


// inserto dades a la db
mysqli_query($varconexion, "INSERT INTO `dades` (`id`, `fecha`, `temperatura`, `humedad`) 
	VALUES (NULL, current_timestamp(), '$temperatura', '$humedad');");


// tanco la conexió amb la db
mysqli_close($varconexion);
// indicamos que variable (conexion con db) queremos cerrar


echo "Datos ingresados correctamente";

?>