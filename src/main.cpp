#include <Arduino.h>
#include <WiFiManager.h>
#include <time.h>
#include "led_stripe.h"


int lastMilis;
int temp;
unsigned long CycleMillis = 0; // speichert den Zeitpunkt des Letzten Zyklus
const char *NTP_SERVER = "de.pool.ntp.org";
const char *TZ_INFO = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";

char wochentage[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
String monat[12] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};

time_t now;
tm tm;

void setup()
{
  Serial.begin(115200);
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("RheinturmUhr-V6-0-1", "123456789"); // password protected ap
  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TZ_INFO, 1);

  npx_start(); // Start Routine für die Neopixel

  pinMode(26, OUTPUT); // Setzt den Digitalpin 26 als Outputpin
  pinMode(27, OUTPUT); // Setzt den Digitalpin 27 als Outputpin
}

void loop()
{

  delay(50);
  CycleMillis = millis();
  show_out(tm.tm_sec, tm.tm_min, tm.tm_hour);
  CycleMillis = millis() - CycleMillis;
  /*
  Serial.print("CycleTime: ");
  Serial.println(CycleMillis);
  */

  time(&now);             // Liest die aktuelle Zeit
  localtime_r(&now, &tm); // Beschreibt tm mit der aktuelle Zeit
  Serial.printf("%02d-%02d-%04d \t", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  // Serial.printf("%02d %s %04d \t", tm.tm_mday, monat[tm.tm_mon], tm.tm_year + 1900);  // Monat ausgeschrieben

  Serial.print(wochentage[tm.tm_wday]);
  // Serial.print(tm.tm_wday);         // Wochentag als Zahl, 0 = Sonntag
  Serial.print("\tder ");
  Serial.print(tm.tm_mday);
  Serial.print(" " + monat[tm.tm_mon] + " ");
  Serial.print(tm.tm_year + 1900);

  Serial.printf(" \tUhrzeit: %02d:%02d:%02d \n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}
