#include <main.h> // move all definitions to separate header-include

void npx_start() 
{ 
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();   // Send the updated pixel colors to the hardware.
} ;



// Funktion zur Anzeige Starrer Lichter
void shiftout_light (int startpos, int bits, int color_red, int color_green, int color_blue)   // Parameter übernehmen
{ for (int i = 0; i < bits; i++) {
    pixels.setPixelColor(startpos + i, pixels.Color(color_red, color_green, color_blue));
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

// Funktion zur Anzeige Stellenzaehler
void shiftout_bits (int startpos, int bits, int value, int color_red, int color_green, int color_blue)   // Parameter übernehmen
{ for (int i = 0; i < bits; i++) // Hauptschleifenzähler, Anzahl der auszugebenden Bits
  { if (value > i) //Wert ist größer als Schleifenzähler
    { 
      #ifdef FIRE
      color_red = random(cnt_rf_min, cnt_rf_max); // Feuerflackern
      color_green = 30;
      #endif
      pixels.setPixelColor(startpos + i, pixels.Color(color_red, color_green, color_blue)); //send Bit out
      //pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
    }
    else // Aktion bei Low Bit
    { pixels.setPixelColor(startpos + i, pixels.Color(0, 0, 0)); //send Bit out
      //pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
    }
  }
  pixels.show();
  return; // Funtion wieder verlassen
}


// Funktion zur Anzeige der Restaurantlichter
void rest_light(int startpos, int bits, int color_red, int color_green, int color_blue) //Anzeige der Restaurant LichterWechselnd
{
unsigned long currentMillis = millis(); // Aktuelle Zeit wird in currentMillis gespeichert

  if (currentMillis - RestMillis >= intervalRestMillis) { // Falls mehr als 1000 ms vergangen sind
      shiftout_light (startpos, bits, color_red, color_green, color_blue);

  }
  if (currentMillis - RestMillis >= intervalRestMillis*2) { // Falls mehr als 1000 ms vergangen sind
      shiftout_light (startpos, bits, color_red/2, color_green/2, color_blue/2);
     RestMillis = currentMillis;} // Zeitpunkt der letzten Schaltung wird festgehalten 
    
  pixels.show();   // Send the updated pixel colors to the hardware.
}

// Funktion zur Anzeige der Positionslichter
void pos_light(int startpos, int bits, int color_red, int color_green, int color_blue) //Anzeige der Positionsleuchten Blinkend
{
unsigned long currentMillis = millis(); // Aktuelle Zeit wird in currentMillis gespeichert

  if (currentMillis - PosMillis >= intervalPosMillis) { // Falls mehr als 1000 ms vergangen sind
      shiftout_light (startpos, bits, color_red, color_green, color_blue);
      digitalWrite(26, HIGH); 
      digitalWrite(27, LOW); 
  }
  if (currentMillis - PosMillis >= intervalPosMillis*2) { // Falls mehr als 1000 ms vergangen sind
      shiftout_light (startpos, bits, 0, 0, 0);
      digitalWrite(27, HIGH); 
      digitalWrite(26, LOW);
     PosMillis = currentMillis;} // Zeitpunkt der letzten Schaltung wird festgehalten 
    
  pixels.show();   // Send the updated pixel colors to the hardware.
}

// Funktion zur Ausgabe der gesamten Anzeige zum Turm
void show_out(int sec, int min, int hrs) 
{
  shiftout_light (0, 11, emptyR, emptyG, emptyB); // die ersten 11 Stellen Trenner in Gelb
  
  //sekunden Einerstelle ausgeben
  tempNPX = sec % 10 ;
  shiftout_bits(11, 9, tempNPX, countR, countG, countB); // Funktion Bitszählen und Muster ausgeben aufrufen
  // Trennstelle ausgeben
  shiftout_light(20, 1, spaceR, spaceG, spaceB); // einzelne Lampe
  //sekunden Zehnerstelle ausgeben
  tempNPX =  sec/ 10 ;
  shiftout_bits(21, 5, tempNPX, countR, countG, countB); // Funktion Bitszählen und Muster ausgeben aufrufen

  // Trennstellen ausgeben
  shiftout_light(26, 2, spaceR, spaceG, spaceB); // einzelne

  //minutenen Einerstelle ausgeben
  tempNPX = min % 10 ;
  shiftout_bits(28, 9, tempNPX, countR, countG, countB); // Funktion Bitszählen und Muster ausgeben aufrufen
  // Trennstelle ausgeben
  shiftout_light(37, 1, spaceR, spaceG, spaceB); // einzelne Lampe
  //minuten Zehnerstelle ausgeben
  tempNPX = min / 10 ;
  shiftout_bits(38, 5, tempNPX, countR, countG, countB); // Funktion Bitszählen und Muster ausgeben aufrufen

  // Trennstelle ausgeben
  shiftout_light(43, 2, spaceR, spaceG, spaceB); // einzelne
  //stunden Einerstelle ausgeben
  tempNPX = hrs % 10 ;
  shiftout_bits(45, 9, tempNPX, countR, countG, countB); // Funktion Bitszählen und Muster ausgeben aufrufen
  // Trennstelle ausgeben
  shiftout_light(54, 1, spaceR, spaceG, spaceB); // einzelne Lampe
  //stunden Zehnerstelle ausgeben
  tempNPX = hrs / 10 ;
  shiftout_bits(55, 2, tempNPX, countR, countG, countB); // Funktion Bitszählen und Muster ausgeben aufrufen
  // Leere Stellen ausgeben
  shiftout_light(57, 5, emptyR, emptyG, emptyB); // fuenf Trennstellen
  // Restuarantbeleuchtung
  rest_light(62,18,restR, restG, restB); // Restaurantbeleuchtung
  
  // pos_light (75,8,restR, restG, restB); 
}


void setup()
{
  Serial.begin(115200);
  npx_start(); // Start Routine für die Neopixel
    shiftout_light(0, 2,spaceR, spaceG, spaceB); // zwei rote für start
    delay(500); // wait a wihle
  
  WiFiManager wm;
  bool res;
  res = wm.autoConnect(apSsid, apPwd); // password protected ap
  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
    shiftout_light(2, 2,spaceR, spaceG, spaceB); // zwei rote für start
    delay(500); // wait a wihle
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    shiftout_light(0, 4,spaceR, spaceG, spaceB); // vier rote OK
    delay(500); // wait a wihle
  }

  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TZ_INFO, 1);
  shiftout_light(0, 6,spaceR, spaceG, spaceB); // sechs rote für start fertig
delay(1000); // wait a wihle
  

  pinMode(26, OUTPUT); // Setzt den Digitalpin 26 als Outputpin
  pinMode(27, OUTPUT); // Setzt den Digitalpin 27 als Outputpin
}

void loop()
{

  delay(100);
  CycleMillis = millis();
  show_out(tm.tm_sec, tm.tm_min, tm.tm_hour); // send lights to the NPX Stripe
  CycleMillis = millis() - CycleMillis;
  
  Serial.print("CycleTime: ");
  Serial.print(CycleMillis);
  Serial.print("\t");
  
  
  time(&now);             // Liest die aktuelle Zeit
  localtime_r(&now, &tm); // Beschreibt tm mit der aktuelle Zeit
  // Serial.printf("%02d-%02d-%04d \t", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  // Serial.printf("%02d %s %04d \t", tm.tm_mday, monat[tm.tm_mon], tm.tm_year + 1900);  // Monat ausgeschrieben

  Serial.print(wochentage[tm.tm_wday]);
  // Serial.print(tm.tm_wday);         // Wochentag als Zahl, 0 = Sonntag
  Serial.print("\tder ");
  Serial.print(tm.tm_mday);
  Serial.print(" " + monat[tm.tm_mon] + " ");
  Serial.print(tm.tm_year + 1900);

  Serial.printf(" \tUhrzeit: %02d:%02d:%02d \n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}
