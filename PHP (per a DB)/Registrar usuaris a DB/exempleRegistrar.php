<?php
// include ('conectarConDB.php')
// conectarDB();
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "frigorifico";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}



// capturem les dades de register.html en variables
// ho fem mitjançant POST i l'atribut "name" (dels inputs de register.html)
$user = strip_tags($_POST['user']);
// strip_tags elimina qualsevol etiqueta html que algú pugui introduir a l'hora d'omplir el formulari (evita problemes)
$mail = strip_tags($_POST['mail']);
$password = sha1(strip_tags($_POST['password']));
// sha1 encripta la contrasenya
$r_password = sha1(strip_tags($_POST['r_password']));

// guardo en una variable el checkbox de "De acuerdo"
$acuerdo = isset($_POST['terms']);
// isset fa que la variable existeixi quan té contingut dins la funció, sinó no es guarda

// medeixo la contrasenya
$pass_para_medir = strip_tags($_POST['password']);
$longitud_password = strlen($pass_para_medir);
if ($longitud_password < 8) {
	echo "La contraseña debe tener al menos 8 caracteres";
	die();
}

// guardo la data en una variable
$hoy = date("Y-m-d H:i:s"); 
// no és 100% necessari pero serveix per saber com enviar-li manualment la data a la DB


// si algun camp queda buit
if ($user == NULL || $mail == NULL || $password == NULL || $r_password == NULL) {
	echo "Rellena todos los campos";
	die();
}
if ($acuerdo == NULL) {
	echo "Acepta los terminos...";
	die();
}

// comprovacions (noms i emails ja existents, contrasenyes no coincidents...)
$query = "SELECT `user` FROM usuarios WHERE user='$user'";
$result = $conn->query($query);
$row = $result->fetch_array();

if ($row[0] == $user) {
	echo "Ese nombre de usuario ya existe";
	die();
} else {
	$query = "SELECT `mail` FROM usuarios WHERE mail='$mail'";
	$result = $conn->query($query);
	$row = $result->fetch_array();

	if ($row[0] == $mail) {
		echo "Ese mail ya existe";
		die();
	} else {
		if ($password != $r_password) {
			echo "Las contraseñas no coinciden";
			die();
		} else {
			//per comprovar errors faig els echo
			echo $hoy;
			echo "<br>";
			echo $user;
			echo "<br>";
			echo $password;
			echo "<br>";
			echo $mail;

			$query = "INSERT INTO `usuarios` (`id`, `fecha`, `user`, `password`, `mail`) VALUES (NULL, '$hoy', '$user','$password', '$mail')";
			$conn->query($query);


			// enviar correu de confirmació (només funciona fora de XAMMP, en un servidor real)
			$para_quien = $mail;
			$titulo = "Bienvenido a Prueba";
			$mensaje = "Hola, ".$user." tu usuario es: ".$user.", ya puedes entrar en el sistema.";
			$cabecera = "From: pdpau@hotmail.com"."\r\n"."Reply-To: pdpau@hotmail.com"."\r\n"."X-Mailer: PHP/".phpversion();

			mail($para_quien, $titulo, $mensaje, $cabecera); // mail($para_quien, $titulo, $mensaje, $cabecera)

			echo "registrado";
			echo "<br>";
			echo "mail enviado";
		}
	}
}
?>