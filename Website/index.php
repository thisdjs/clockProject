<!doctype html>
<html lang="pt-PT">
<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">  

  <!-- Font Awesome -->
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.3.1/css/all.css" integrity="sha384-mzrmE5qonljUremFsqc01SB46JvROS7bZs3IO2EmfFsd15uHvIt+Y8vEf7N7fWAU" crossorigin="anonymous">

  <!-- Bootstrap CSS -->
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

  <link rel="stylesheet" type="text/css" href="../styles/style.css">

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

  $despertarInfo = "SELECT * FROM despertar";
  $result = $conn->query($despertarInfo);

  if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
      $dia = new DateTime($row["dia"]);
      $hora = new DateTime($row["hora"]);
      $toque = $row["toque"];
      $modoDesligar = $row["modo_desligar"];
      $modoLumi = $row["modo_lumi"];
      $repetir = $row["repetir"];
    }
  }

  $notesNaBD = "SELECT nota FROM bloconotas";
  $resultb = $conn->query($notesNaBD);

  if ($resultb->num_rows > 0) {
    // output data of each row
    while($row = $resultb->fetch_assoc()) {
      $nota = $row["nota"];
    }
  }

  $conn->close();

  if($toque == "mediaPlayer") {
    $toque = "MP3";
  }

  if ($modoDesligar == "button") {
    $modoDesligar = "Botão";
  } else if ($modoDesligar == "distance") {
    $modoDesligar = "Distância";
  }

  if ($modoLumi == 0)  {
    $modoLumi = "Desligado";
  } else {
    $modoLumi = "Ativo";
  }

  if ($repetir == 0) {
    $repetir = "Não";
  } else {
    $repetir = "Sim";
  }
  $date=date_create("2013-03-15");

  ?> 
  <nav class="navbar navbar-light bg-light">
    <a class="navbar-brand" href="#">
      <span class="far fa-clock"></span>
      Clock Project
    </a>
  </nav>

  <div class="container">    

    <div class="row mt-2">

      <div class="col-sm-6">
        <div class="card mt-4">
          <!--<img class="card-img-top" src="..." alt="Card image cap">
          <div class="card-header">
            Alarme
          </div>-->
          <div class="card-header" style="background-image: url('../images/backfour.jpeg'); background-repeat: no-repeat;
          background-size: cover; background-position: center;">
          <div class="card-title">
            <h5 class="card-subtitle text-muted"> <?php echo $dia->format('d-m-Y'); ?>
            <span class="float-right"><img src="../images/baseline-alarm-24px.svg" alt="alarm"></span>
          </h5>
          <h1 class="mt-3"><?php echo $hora->format('H:i'); ?> 
          <a href="editClock.html" class="btn btn-outline-dark mt-4 rounded float-right" style="background-color: transparent;">
            Editar <span class="far fa-edit"></span>
          </a>
        </h1>
      </div>
    </div>
    <ul class="list-group list-group-flush">
      <li class="list-group-item"> Toque: <?php echo $toque; ?> </li>
      <li class="list-group-item"> Repetir: <?php echo $repetir; ?> </li>
      <li class="list-group-item"> Modo de desligar: <?php echo $modoDesligar; ?> </li>
      <li class="list-group-item"> Modo de luminosidade: <?php echo $modoLumi; ?> </li>
    </ul>
  </div>
</div>

<div class="col-sm-6">
  <div class="card mt-4 mb-3">
    <div class="card-header bg-warning">
      <h5> Bloco de notas <span class="far fa-sticky-note mt-1 float-right"></span></h5>
    </div>
    <div class="card-body"> 
      <form action="" method="post" id="formText">
        <div class="form-row">
          <textarea  class="noteTextArea" name="note" form="formText" rows="6" cols="100"><?php echo $nota ?></textarea>      
        </div>
        <button type="submit" name="submit_button" value="submit" class="btn btn-warning mt-3 rounded float-right">
          Guardar <span class="far fa-save"></span>
        </button>
      </form>        
    </div>
  </div>
</div>
</div>
</div>

<!-- Optional JavaScript -->
<!-- jQuery first, then Popper.js, then Bootstrap JS -->
<script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
</body>
</html>

<?php

if(isset($_POST["submit_button"])) {

  // Create connection
  $mysqli = new mysqli($servername, $username, $password, $dbname);

  // Check connection
  if($mysqli === false){
    die("ERROR: Could not connect. " . $mysqli->connect_error);
  }

  $thisNote = $_POST['note'];

  $sqlb = "UPDATE bloconotas SET nota='$thisNote'";
  $mysqli->query($sqlb);

  echo "<meta http-equiv='refresh' content='0'>";

  // Close connection
  $mysqli->close();
}

?>