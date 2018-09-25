#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <TimeLib.h>

const char *ssid = "F366";  //ENTER YOUR WIFI SETTINGS
const char *password = "";

//Web/Server address to read/write from
const char *host = "clockproject.000webhostapp.com";

String link;
String wakeUpTime, turnOffMode, alarmTone, modoLumi, repetir;
int horasAtuais, minutosAtuais, diaAtual, mesAtual;
int horasDespertar, minutosDespertar, diaDespertar, mesDespertar;

//=======================================================================
//                    Power on setup
//=======================================================================
void setup() {
  delay(1000);
  Serial.begin(9600);         // start serial

  /*************************** CONECCAO A INTERNET *********************/
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password); //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to ESP
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  Wire.begin(D1, D2); // join i2c bus (address optional for master)

}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  /*************************** CURRENT TIME ******************/
  link = "http://clockproject.000webhostapp.com/currentTime.php";

  http.begin(link);                     //Specify request destination
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
  http.end();                           //Close connection

  payload.replace(" ", "-");
  String currentTime = payload;
  Serial.println("time: " + currentTime);

  currentTime.remove(0, currentTime.indexOf(':') + 1);
  minutosAtuais = currentTime.toInt();

  currentTime = payload;
  currentTime.remove(0, currentTime.indexOf('-') + 1);
  horasAtuais = currentTime.toInt();

  currentTime = payload;
  currentTime.remove(0, currentTime.indexOf('/') + 1);
  diaAtual = currentTime.toInt();

  currentTime = payload;
  currentTime.remove(currentTime.indexOf('/'));
  mesAtual = currentTime.toInt();

  //Serial.println(minutosAtuais);
  //Serial.println(horasAtuais);
  //Serial.println(diaAtual);
  //Serial.println(mesAtual);

  /*************************** GET VALUES ********************/
  link = "http://clockproject.000webhostapp.com/horasDespertar.php";

  http.begin(link);                     //Specify request destination
  httpCode = http.GET();                //Send the request
  payload = http.getString();           //Get the response payload
  http.end();                           //Close connection

  payload.replace(" ", "_");
  Serial.println("Data Despertar: " + payload);

  wakeUpTime = payload;
  wakeUpTime.remove(0, wakeUpTime.indexOf(':') + 1);
  minutosDespertar = wakeUpTime.toInt();

  wakeUpTime = payload;
  wakeUpTime.remove(0, wakeUpTime.indexOf('_') + 1);
  horasDespertar = wakeUpTime.toInt();

  wakeUpTime = payload;
  wakeUpTime.remove(0, wakeUpTime.indexOf('-') + 1);
  wakeUpTime.remove(0, wakeUpTime.indexOf('-') + 1);
  diaDespertar = wakeUpTime.toInt();

  wakeUpTime = payload;
  wakeUpTime.remove(0, wakeUpTime.indexOf('-') + 1);
  mesDespertar = wakeUpTime.toInt();

  turnOffMode = payload;
  turnOffMode.remove(0, turnOffMode.indexOf('_') + 1);
  turnOffMode.remove(0, turnOffMode.indexOf('_') + 1);
  turnOffMode.remove(turnOffMode.indexOf('_'));

  alarmTone = payload;
  alarmTone.remove(0, alarmTone.indexOf('_') + 1);
  alarmTone.remove(0, alarmTone.indexOf('_') + 1);
  alarmTone.remove(0, alarmTone.indexOf('_') + 1);
  alarmTone.remove(alarmTone.indexOf('_'));

  modoLumi = payload;
  modoLumi.remove(0, modoLumi.indexOf('_') + 1);
  modoLumi.remove(0, modoLumi.indexOf('_') + 1);
  modoLumi.remove(0, modoLumi.indexOf('_') + 1);
  modoLumi.remove(0, modoLumi.indexOf('_') + 1);
  modoLumi.remove(modoLumi.indexOf('_'));

  repetir = payload;
  repetir.remove(0, repetir.indexOf('_') + 1);
  repetir.remove(0, repetir.indexOf('_') + 1);
  repetir.remove(0, repetir.indexOf('_') + 1);
  repetir.remove(0, repetir.indexOf('_') + 1);
  repetir.remove(0, repetir.indexOf('_') + 1);
  repetir.remove(repetir.indexOf('_'));

  Serial.println(minutosDespertar);
  Serial.println(horasDespertar);
  Serial.println(diaDespertar);
  Serial.println(mesDespertar);
  Serial.println(turnOffMode);
  Serial.println(alarmTone);
  Serial.println(modoLumi);
  Serial.println(repetir);


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*************************** Conexão com arduino (I2C) *****/
  Wire.beginTransmission(8); // transmit to device #8
  
  Wire.write(horasAtuais);
  Wire.write(minutosAtuais);
  Wire.write(diaAtual);
  Wire.write(mesAtual);
  Wire.write(horasDespertar);
  Wire.write(minutosDespertar);
  Wire.write(diaDespertar);
  Wire.write(mesDespertar);
  if (turnOffMode == "button") {
    Wire.write(91);
  } else if (turnOffMode == "distance") {
    Wire.write(92);
  }
  if (alarmTone == "ringtone") {
    Wire.write(93);
  } else if (alarmTone == "mediaPlayer") {
    Wire.write(94);
  }
  if (modoLumi == "0") {
    Wire.write(95);
  } else if (modoLumi == "1") {
    Wire.write(96);
  }
  if (repetir == "0") {
    Wire.write(97);
  } else if (repetir == "1") {
    Wire.write(98);
  }

  Wire.endTransmission();    // stop transmitting

  delay(25000);

}
