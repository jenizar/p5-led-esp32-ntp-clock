//digital clock esp32 NTP
//Part: esp32(1pcs), Panel P5 RGB (1pcs), Kabel jumper female to female dufon (14pcs), kabel power panel p5 modif dufon female (2pcs), charger hp 5volt(1pcs) 
//www.youtube.com/AsepRizal

#include "WiFi.h"
#include "RGBmatrixPanel.h"
#include "Adafruit_GFX.h"
#include "Fonts/FreeSansBold9pt7b.h"
#include "Fonts/BigNumber.h"
uint8_t rgbpins[] = { 19, 13, 18, 5, 12, 17 };
RGBmatrixPanel matrix(16, 14, 4, 27, 2, 26, 15, false, 64, rgbpins);

//setting wifi hotspot
char ssid[] = "al-husna"; // Nama Wifi Anda
char pass[] = "sekolahdasar"; // Password Wifi Anda

//setting time zone
#define TZ (+7*60*60) //Timezone          //+7 Jakarta indonesia
//#define TZ (-5*60*60) //Timezone       //-5 New york
//#define TZ (+5.50*60*60) //Timezone          //+5.30 hindi . (+5.50 same as actual display in time zone) 

void setup() {
  Serial.begin(115200);

  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  configTime(TZ, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); 

  matrix.begin();                      
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();

  time_t t;
  static time_t last_t;
  struct tm *tm;
  //setting day name -
  //static const char* const wd[7] = {"Ahad","Senin","Selasa","Rabu","Kamis","Jum'at","Sabtu"};   // bahasa indonesia
  static const char* const wd[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};   //English
  //static const char* const wd[7] = {"Ravivaar","Somavaar","Mangalavar","Budhavaar","Guroovaar","Shukravaar","Shanivaar"};   //Hindi. (sorry if i wrote wrong. translated google reference)
  t = time(NULL);
  if (last_t == t) return;
  last_t = t;
  tm = localtime(&t);
  matrix.fillScreen(0);
  
//display hour minute
  matrix.setTextColor(matrix.Color333(15, 15, 0));
  matrix.setTextSize(2);
  matrix.setCursor(1, 0);
  //matrix.printf("00");
  matrix.printf("%02d", tm->tm_hour);
  matrix.drawCircle(25, 3, 1, matrix.Color333(15, 0, 0));
  matrix.drawCircle(25, 10, 1, matrix.Color333(15, 0, 0));
  matrix.drawCircle(25, 3, 0, matrix.Color333(0, 15, 0));
  matrix.drawCircle(25, 10, 0, matrix.Color333(0, 15, 0));
  matrix.setCursor(28, 0);
  //matrix.printf("00");
  matrix.printf("%02d",  tm->tm_min);

  //display second - detik
  matrix.setTextColor(matrix.Color333(0, 15, 15));
  matrix.setTextSize(1);
  matrix.setCursor(52, 7);
  matrix.printf("%02d", tm->tm_sec);
  
// display day - hari
  matrix.setCursor(15, 16);           ///cursor for hindi 3,16
  matrix.setTextColor(tm->tm_wday == 0 ? matrix.Color333(15, 0, 0) :
                      tm->tm_wday == 6 ? matrix.Color333(0, 15, 15) :
                                         matrix.Color333(15, 0, 0));
  matrix.printf("%s", wd[tm->tm_wday]);
  
//display year - tahun
  matrix.setCursor(3, 24);
  matrix.setTextColor(matrix.Color333(15, 0, 15));
  matrix.printf("%02d-%02d-%04d-n", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);

}
