#include <Arduino.h>
#include <WiFiManager.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>
#include <led_stripe.h>

const char *apSsid = "Rheinturm-V6-0-2"; // SSID for AP if WLAN failed and first setup
const char *apPwd = "123456789"; // SSID for AP if WLAN failed and first setup


int lastMilis;
int temp;
unsigned long CycleMillis = 0; // milli secounds conuter for cyclic delay funtions
const char *NTP_SERVER = "de.pool.ntp.org";
const char *TZ_INFO = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";

char wochentage[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
String monat[12] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};

time_t now;
tm tm;
