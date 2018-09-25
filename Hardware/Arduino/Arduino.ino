#include <Wire.h>
#include <SimpleDHT.h>
#include <lcd.h>
#include <LCD_C0220BiZ.h>
#include <ST7036.h>
#include <Time.h>
#include <TimeAlarms.h>

// mp3 + SD libraries
#include <SPI.h>
#include <SdFat.h>
#include <vs1053_SdFat.h>
#include <Bounce2.h>

#define touchPin A0
#define B_NEXT  A1
#define B_PREV  3
//#define BUTTON_DEBOUNCE_PERIOD 10 //ms

#define I2C_Address 0x3C
ST7036 lcd = ST7036 ( 2, 16, I2C_Address * 2 );

SdFat sd;
vs1053 MP3player;

//Bounce b_Next  = Bounce();
//Bounce b_Prev  = Bounce();

int8_t current_track = 1;
unsigned long nowTime, afterTime;

int horas, minutos, dia, mes;
int horasDespertar, minutosDespertar, diaDespertar, mesDespertar;
int modoDesligar, alarmTone, modoLumi, repetirAlarm;
int repeatDay, alarmDay, alarmMinute;

//Estados
bool alarmModeOn = false;
bool mediaPlayerModeOn = false;
bool pauseMedia = false;
bool verifHoraDespertar = false;
bool lcdLimpo = false;
bool despertouUmaVez = false;
bool repeating = false;
bool lumiAtivo = false;
bool lumiFoiDesligado = false;
bool verifModoLumi, repeat, verifRepeatDespertar;

//LM35
const int LM35 = A2;
float temperatura;

//LDR
const int ldrPin = A3;
float photocellReading;// A leitura analógica do divisor de resistência analógico

// HC-SR04
const int trigPin = 4;
const int echoPin = 5;
long duration, cm;

int a = 1, b = 1, c = 1;

void setup() {
  lcd.init(); delay(50);    // Inicializa o display
  lcd.clear(); delay(50);   // Limpa do Display

  pinMode(touchPin, INPUT);
  pinMode(B_NEXT, INPUT);
  pinMode(B_PREV, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //pinMode(B_NEXT, INPUT_PULLUP);
  //pinMode(B_PREV, INPUT_PULLUP);

  /*b_Next.attach(B_NEXT);
    b_Next.interval(BUTTON_DEBOUNCE_PERIOD);
    b_Prev.attach(B_PREV);
    b_Prev.interval(BUTTON_DEBOUNCE_PERIOD);*/

  Serial.begin(9600);

  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event

  sd.begin(SD_SEL, SPI_HALF_SPEED);
  //sd.begin(SD_SEL, SPI_FULL_SPEED);
  MP3player.begin();
}

void loop() {
  // Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif

  nowTime = millis();
  //setTime(horas, minutos, 0, dia, mes, 18);

  // Despertar primeira vez
  if (dia != 0 && verifHoraDespertar == false && /*lumiFoiDesligado == false &&*/
      mes == mesDespertar && dia == diaDespertar && horas == horasDespertar && minutos == minutosDespertar) {
    if (MP3player.isPlaying()) {
      MP3player.stopTrack();
    }
    MP3player.setVolume(20, 20);
    delay(100);

    current_track = 0;

    if (repetirAlarm == 98) {
      repeat = true;
      repeatDay = diaDespertar + 1;
      alarmDay = diaDespertar;
      alarmMinute = minutosDespertar;
    } else {
      repeat = false;
    }

    verifHoraDespertar = true;
    alarmModeOn = true;
    mediaPlayerModeOn = false;
    lcdLimpo = false;
    lumiAtivo = false;
    lumiFoiDesligado = false;

    despertouUmaVez = false;
    Serial.print(a++); Serial.println("tocarpravez");
  }

  if (repeat == true && despertouUmaVez == true) {
    if (alarmDay != diaDespertar && alarmMinute != minutosDespertar) {
      repeat = false; // quer dizer que há um novo alarme e que tem de começar a repetir noutro dia
    }
  }
  //Para Testes:
  //repeatDay = dia;
  //despertouUmaVez == true;
  //repetir Alarm
  if (repetirAlarm == 98 && repeat == true && despertouUmaVez == true && verifRepeatDespertar == false /*&& lumiFoiDesligado == false*/) {
    repeating = true;    
    if (dia != 0 && horas == horasDespertar && minutos == minutosDespertar && repeatDay == dia) {
      if (MP3player.isPlaying()) {
        MP3player.stopTrack();
      }
      MP3player.setVolume(20, 20);
      delay(100);

      repeatDay = repeatDay + 1;

      current_track = 0;

      verifHoraDespertar = true;
      mediaPlayerModeOn = false;
      lcdLimpo = false;
      lumiAtivo = false;
      alarmModeOn = true;
      lumiFoiDesligado = false;
      Serial.print(b++); Serial.println("repetirAlarme");
    }
  }
  else {
    repeating = false;
  }

  //Modo Luminosidade
  if (modoLumi == 96 && verifModoLumi == false && dia == diaDespertar && horas >= 15 && horas <= 16 ||
      modoLumi == 96 && verifModoLumi == false && repeating == true && horas >= 15 && horas <= 16) {
    if (MP3player.isPlaying()) {
      MP3player.stopTrack();
    }
    current_track = 0;

    lumiAtivo = true;
    lcdLimpo = false;
    mediaPlayerModeOn = false;
    //lumiFoiDesligado = false;
    Serial.print(c++); Serial.println("modolumi");
  }
  if (lumiAtivo == true) {
    photocellReading = analogRead(ldrPin); delay(50);

    verifModoLumi = true;

    //if (photocellReading > 5) {
    alarmModeOn = true;
    if (photocellReading <= 100) {
      MP3player.setVolume(28, 28);
    } else if (photocellReading > 100 && photocellReading < 200) {
      MP3player.setVolume(14, 14);
    } else if (photocellReading >= 200) {
      MP3player.setVolume(2, 2);
    }
    //}
  }
  if (horas < 15 || horas > 16) {
    verifModoLumi == false;
  }

  delay(100);

  //tocarAlarm
  if (alarmModeOn == true) {
    alarmModes();
  }

  //para não ligar o alarme again depois de ter sido desligado na hora do alarme
  if (minutos != minutosDespertar) {
    verifHoraDespertar = false; //é usado para poder ser lida a hora de alarme outra vez
    if (repeat == true) {
      verifRepeatDespertar = false;
    }
  }

  delay(50);

  //ligar media player - time usado para mediaPlayer ñ ligar logo depois de premir botao desligar
  if (alarmModeOn == false && digitalRead(touchPin) == HIGH && mediaPlayerModeOn == false && lumiAtivo == false && nowTime > afterTime + 5000) {
    mediaPlayerModeOn = true;

    lcd.clear(); delay(50);
    lcd.setCursor (0, 1);
    lcd.print("Media Player ON");
    delay(3000);
    lcd.clear(); delay(50);

    lcd.setCursor (1, 0);
    lcd.print("#");
    lcd.print(current_track);

    MP3player.playTrack(current_track);
  }

  delay(50);

  //media player buttons
  if (mediaPlayerModeOn == true) {
    mediaPlayerControls();
  }

  //turn off mediaPlayer
  if (alarmModeOn == false && mediaPlayerModeOn == true && digitalRead(touchPin) == HIGH && lumiAtivo == false) {
    mediaPlayerModeOn = false;

    lcd.clear(); delay(50);
    lcd.setCursor (0, 0);
    lcd.print("Media Player OFF");
    delay(3000);
    lcd.clear(); delay(50);

    MP3player.stopTrack();

    current_track = 1;
  }

  //continuar a dar musica no media Player
  if (mediaPlayerModeOn == true && !MP3player.isPlaying() && pauseMedia == false) {
    MP3player.playTrack(++current_track);
    lcd.setCursor (1, 0);
    lcd.print("#"); lcd.print(current_track);
    // if current_track == null stop music!!
  }

  delay(100);
  if (alarmModeOn == false && lumiAtivo == false) {
    printLcd(); // horas + sensores
  }
  delay(100);

}


//=======================================================================
//                    FUNÇÕES
//=======================================================================

void alarmModes() {
  // Button
  if (modoDesligar == 91) {
    if (digitalRead(touchPin) == LOW) {
      if (!MP3player.isPlaying()) {
        MP3player.playTrack(++current_track);
      }

      if (lcdLimpo == false) {
        lcd.clear(); delay(50);
        lcdLimpo = true;
      }
      printHoras();
      lcd.setCursor (1, 2);
      lcd.print("ALARM ON!");

      if (lumiAtivo == true) {
        //LCD_LUMINOSIDADE
        lcd.setCursor (1, 12);
        photocellReading = (photocellReading * 100) / 1024;
        if ( photocellReading < 10 ) {
          lcd.print(' ');
        }
        lcd.print((int)photocellReading); lcd.print("%");
      }

    }
    else {
      MP3player.stopTrack();

      lcd.clear(); delay(50);
      lcd.setCursor (0, 4);
      lcd.print("ALARM OFF");
      delay(3000);
      lcd.clear(); delay(50);

      alarmModeOn = false;
      lcdLimpo = false;
      lumiAtivo = false;
      /*if (lumiAtivo == true) {
        lumiFoiDesligado = true;
      }
      if (repeat == true) {
        despertouUmaVez = true;
      }*/
      afterTime = millis();

      current_track = 1;
    }
  }

  // Distancia
  if (modoDesligar == 92) {
    if (!MP3player.isPlaying()) {
      MP3player.playTrack(++current_track);
    }

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    cm = (duration / 2) / 29.1;

    delay(2000);

    if (lcdLimpo == false) {
      lcd.clear(); delay(50);
      lcdLimpo = true;
    }
    printHoras();
    lcd.setCursor (1, 1);
    lcd.print("distancia: ");
    lcd.print(cm); lcd.print("cm");

    if (lumiAtivo == true) {
      //LCD_LUMINOSIDADE
      lcd.setCursor (0, 12);
      photocellReading = (photocellReading * 100) / 1024;
      if ( photocellReading < 10 ) {
        lcd.print(' ');
      }
      lcd.print((int)photocellReading); lcd.print("%");
    }

    if (cm != 0 && cm <= 5) {
      MP3player.stopTrack();

      lcd.clear(); delay(50);
      lcd.setCursor (0, 4);
      lcd.print("ALARM OFF");
      delay(3000);
      lcd.clear(); delay(50);

      alarmModeOn = false;
      lcdLimpo = false;
      lumiAtivo = false;
      if (lumiAtivo == true) {
        lumiFoiDesligado = true;
      }
      if (repeat == true) {
        despertouUmaVez = true;
      }

      current_track = 1;
    }
  }
}

void mediaPlayerControls() {
  //Play
  if (digitalRead(touchPin) == HIGH && pauseMedia == true)  {
    Serial.print(F("B_PLAY pressed, Start Playing Track # "));
    Serial.println(current_track);

    MP3player.resumeMusic();
    pauseMedia = false;

    //time = millis(); // experimentar com diferença de tempo
    /*&& time + 1000 > time*/
  }

  //Pause
  if (digitalRead(touchPin) == HIGH && MP3player.isPlaying() ) {
    Serial.print(F("pause pressed, pausing Track #"));
    MP3player.pauseMusic();
    pauseMedia = true;
  }

  if (digitalRead(B_NEXT) == HIGH) {

    MP3player.stopTrack();
    MP3player.playTrack(++current_track);

    lcd.setCursor (1, 0);
    lcd.print("#"); lcd.print(current_track);

    pauseMedia = false;
  }
  if (digitalRead(B_PREV) == HIGH)  {

    MP3player.stopTrack();
    MP3player.playTrack(--current_track);

    lcd.setCursor (1, 0);
    lcd.print("#"); lcd.print(current_track);

    pauseMedia = false;
  }  
}

void printHoras() {
  lcd.setCursor (0, 1);
  if ( horas < 10 ) {
    lcd.print('0'); // In the first 10 minutes of each hour, we'll want a leading '0'
  }
  lcd.print(horas);
  lcd.print(":");
  if ( minutos < 10 ) {
    lcd.print('0');
  }
  lcd.print(minutos);
}

void printLcd() {
  if (lcdLimpo == false) {
    lcd.clear(); delay(50);
    lcdLimpo = true;
  }

  photocellReading = analogRead(ldrPin); delay(50);
  temperatura = (float(analogRead(LM35)) * 5 / (1024)) / 0.01; delay(50);
  //temperatura = (analogRead(LM35)/1024.0)*(5000)/10;

  //LCD_TEMPERATURE
  lcd.setCursor (1, 7);
  lcd.print(int(temperatura)); lcd.print("*C");

  //LCD_LUMINOSIDADE
  lcd.setCursor (1, 12);
  photocellReading = (photocellReading * 100) / 1024;
  if ( photocellReading < 10 ) {
    lcd.print(' ');
  }
  lcd.print((int)photocellReading); lcd.print("%");

  //LCD_DATA
  lcd.setCursor (0, 1);
  if ( horas < 10 ) {
    lcd.print('0'); // In the first 10 minutes of each hour, we'll want a leading '0'
  }
  lcd.print(horas);
  lcd.print(":");
  if ( minutos < 10 ) {
    lcd.print('0');
  }
  lcd.print(minutos);

  lcd.setCursor (0, 7);
  if ( dia < 10 ) {
    lcd.print('0');
  }
  lcd.print(dia);
  lcd.print('/');
  if ( mes < 10 ) {
    lcd.print('0');
  }
  lcd.print(mes);
  lcd.print("/18");
}

void receiveEvent(int howMany) {
  horas = Wire.read();
  minutos = Wire.read();
  dia = Wire.read();
  mes = Wire.read();
  horasDespertar = Wire.read();
  minutosDespertar = Wire.read();
  diaDespertar = Wire.read();
  mesDespertar = Wire.read();
  modoDesligar = Wire.read();
  alarmTone = Wire.read();
  modoLumi = Wire.read();
  repetirAlarm = Wire.read();

  /*Serial.println("*****");
    Serial.println(horas);
    Serial.println(minutos);
    Serial.println(dia);
    Serial.println(mes);
    Serial.println(horasDespertar);
    Serial.println(minutosDespertar);
    Serial.println(diaDespertar);
    Serial.println(mesDespertar);
    Serial.println(modoDesligar);
    Serial.println(alarmTone);
    Serial.println(modoLumi);
    Serial.println(repetirAlarm);*/
}
