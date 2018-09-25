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
        echo $row["dia"]. " ";
        echo $row["hora"]. " ";
        echo $row["modo_desligar"]. " ";
        echo $row["toque"]. " ";
        echo $row["modo_lumi"]. " ";
        echo $row["repetir"];
    }
} else {
    echo "0 results";
}

$conn->close();

?> 