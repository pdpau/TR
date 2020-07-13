<?php
$varconexion = mysqli_connect("localhost","root","","provasensors1");
$varconexion -> set_charset("utf8");
if (mysqli_connect_errno())
  {
  echo "Fallo en la conexion a MySQL: " . mysqli_connect_error();
  }
 return $varconexion;
?>