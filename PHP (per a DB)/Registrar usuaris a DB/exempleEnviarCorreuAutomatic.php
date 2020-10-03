<?php 
// enviar correu de confirmació (només funciona fora de XAMMP, en un servidor real)
$para_quien = $mail;
$titulo = "Bienvenido a Prueba";
$mensaje = "Hola, ".$user." tu usuario es: ".$user.", ya puedes entrar en el sistema.";
$cabecera = "From: pdpau@hotmail.com"."\r\n"."Reply-To: pdpau@hotmail.com"."\r\n"."X-Mailer: PHP/".phpversion();

mail(to, subject, message); // mail($para_quien, $titulo, $mensaje, $cabecera)

echo "registrado";
echo "<br>";
echo "mail enviado";
?>