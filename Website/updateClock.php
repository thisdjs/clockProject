<!doctype html>
<html lang="pt-PT">
<head>
	<!-- Required meta tags -->
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

	<link rel="stylesheet" type="text/css" href="../../styles/style.css">

	<!-- Font Awesome -->
	<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.3.1/css/all.css" integrity="sha384-mzrmE5qonljUremFsqc01SB46JvROS7bZs3IO2EmfFsd15uHvIt+Y8vEf7N7fWAU" crossorigin="anonymous">

	<!-- Bootstrap CSS -->
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

	<title>Clock Project</title>
</head>
<body>
	<?php
	$servername = "localhost";
	$username = "id6986029_admin";
	$password = "admin";
	$dbname = "id6986029_clockdb";

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 

	$dia = $_POST["data"];
	$hora = $_POST["hora"];
	//$modeDespertar = $_POST["modoDespertar"];
	$toque = $_POST["toque"];
	$modeDesligar = $_POST["modoDesligar"];
	$lumiMode = isset($_POST["modoLumi"]) ? 1 : 0;
	$repeat = isset($_POST["repetir"]) ? 1 : 0;

	$sql = "UPDATE despertar SET dia='$dia', hora='$hora', modo_desligar='$modeDesligar', toque='$toque', 
	modo_lumi='$lumiMode', repetir='$repeat'";		

	/*echo $_POST["data"] . " "; 
	echo $_POST["hora"] . ":00 ";
	echo $_POST["modoDesligar"] . " ";
	echo $_POST["toque"];
	echo $_POST["modoLumi"];
	echo $_POST["repetir"];*/

	if ($conn->query($sql) === TRUE) { ?>

		<div class="container">
			<div class="alert alert-success mt-4" role="alert">
				<h4 class="alert-heading">
					Alarme definido com sucesso <span class="far fa-check-circle"></span>
				</h4>				
			</div>
			<hr>
			<div style="text-align: center;">
				<a href="../index.php"><button class="btn btn-outline-secondary">Página Inicial</button></a>	
			</div>
		</div>

	<?php } else { ?>

		<div class="container">
			<div class="alert alert-danger mt-4" role="alert">
				<h4 class="alert-heading">
					Ocorreu um erro <span class="far fa-frown"></span>
				</h4>				
			</div>
			<hr>
			<div style="text-align: center;">
				<a href="../index.php"><button class="btn btn-outline-secondary">Página Inicial</button></a>	
			</div>
		</div>

	<?php }

	$conn->close();
	?>

	<!-- Optional JavaScript -->
	<!-- jQuery first, then Popper.js, then Bootstrap JS -->
	<script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
</body>
</html>