/*Farbendefinitionen für die Ausgabe zu Turm
Definition der Pixel:
Adresse 0-10 ohne Funktion, standard Gelb
Adresse 11-20 Sekunden 1" Stelle
.....
*/
int emptyR =15; //Leere Segmente
int emptyG =15;
int emptyB =0;

int spaceR =20; // Trenner
int spaceG =0;
int spaceB =0;

int countR =0; //Zaehler
int countG =0;
int countB =30;

int posR =50; //Positionslichter
int posG =0;
int posB =0;

int restR =00; //Restaurant
int restG =10;
int restB =10;

/* 
_______________________________________________
// AB HIER KEINE AENDERNGEN MEHR VORNEHMEN //
_______________________________________________
*/ 

#include <Adafruit_NeoPixel.h>


#define NeoPixelPin   2 // 
#define NUMPIXELS 86 // how many neopixels where attached

unsigned long PosMillisP = 0; // time counter for position lights with millis() function
const long interval = 900; // light intervall for the position lights
int tempNPX = 0; // temporary variable

Adafruit_NeoPixel pixels(NUMPIXELS, NeoPixelPin, NEO_GRB + NEO_KHZ800);

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

void pos_light(int startpos, int bits, int color_red, int color_green, int color_blue) //Anzeige der Positionsleuchten Blinkend
{
unsigned long currentMillis = millis(); // Aktuelle Zeit wird in currentMillis gespeichert

  if (currentMillis - PosMillisP >= interval) { // Falls mehr als 1000 ms vergangen sind
      shiftout_light (startpos, bits, color_red, color_green, color_blue);
      digitalWrite(26, HIGH); 
      digitalWrite(27, LOW); 
  }
  if (currentMillis - PosMillisP >= interval*2) { // Falls mehr als 1000 ms vergangen sind
      shiftout_light (startpos, bits, 0, 0, 0);
      digitalWrite(27, HIGH); 
      digitalWrite(26, LOW);
     PosMillisP = currentMillis;} // Zeitpunkt der letzten Schaltung wird festgehalten 
    
  pixels.show();   // Send the updated pixel colors to the hardware.
}


void show_out(int sec, int min, int hrs) //Ausgabe der Zeitanzeige zum Turm
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
  shiftout_light(62,12,restR, restG, restB); // Resttaurantbeleuchtung
  
  pos_light (75,8,restR, restG, restB); 
}
