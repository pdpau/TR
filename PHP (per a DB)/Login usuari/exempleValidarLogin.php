<?php 
// inicio de sesión
// las variables de sesión serán las mismas en todos los archivos PHP de mi programa durante 30 min
// las puedo recuperar desde cualquier archivo PHP del programa
session_start();
// conectar a la DB
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "frigorifico";
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
   	die("Connection failed: " . $conn->connect_error);
}

// guardamos en variables los datos recibidos por POST
$user = strip_tags($_POST['user']);
$password = strip_tags(sha1($_POST['password']));
$user = $conn->real_escape_string($user);
$password = $conn->real_escape_string($password);

$sqlquery = "SELECT * FROM usuarios WHERE `user` = '$user' AND `password` = '$password';";
$resultquery = $conn->query($sqlquery);


// si la consulta anterior devuelve un resultado significa que el usuario esta registrado y ha ingresado correctamente
if ($existe = $resultquery->fetch_object()) {
    $hoy = date("Y-m-d H:i:s");

    /* $sqlquery = "UPDATE `usuarios` SET `ultimo_ingreso` = '$hoy' WHERE `user` = '$user';"
    $resultquery = $conn->query($sqlquery) */;

    $_SESSION['logged'] = "yes";

    // extraemos todas las columnas de ese usuario en particular i las metemos en un array
    $queryExtraer = "SELECT * FROM usuarios WHERE `user` = '$user'";
    $resultExtraer = $conn->query($queryExtraer);
    $row = $resultExtraer->fetch_array();

    // echo es solo para aprovechar e imprimirlo en pantalla
    echo $id = $row[0];
    echo "<br>";
    echo $fecha = $row[1];
    echo "<br>";
    echo $user = $row[2];
    echo "<br>";
    echo $mail = $row[4];
    echo "<br>";


    // guardamos todo en variables de sesion para que el usuario
    // pueda estar logueado en nuestra web por un maximo de 30 min
    // sin que se tenga que volver a loguear
    $_SESSION['user_id'] = $id;
    $_SESSION['user'] = $user;
    $_SESSION['mail'] = $mail;
    // utilitzar id com a identificador d'usuari en comptes del nom d'usuari


    $conn->close();

    echo "true";

    // redirigimos a otra pagina si el usuario se ha logueado correctamente
    /* echo '<meta http-equiv="Refresh" content="3(son los segundos de delay);url=http://www.miweb.com">'; */

} else {
    $_SESSION['logged'] = "no";
    echo "false";
}
?>