/*PWM制御--------------------------------------------------------------------------*/
static uint8_t power = 0;
static int diff = 5;
const int PWMPin = 32;
/*PWM制御--------------------------------------------------------------------------*/


/*BME280　室内温・気圧--------------------------------------------------------------*/
#include <ESP32_BME280_I2C.h>
ESP32_BME280_I2C bme280i2c(0x76, 16, 17, 400000); //address, SCK, SDA, frequency
char temp_c[10], hum_c[10], pres_c[10];
double temperature, pressure, humidity;

/*BME280　室内温・気圧--------------------------------------------------------------*/


/*TFT_eSPI-------------------------------------------------------------------------*/
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
/*TFT_eSPI-------------------------------------------------------------------------*/


/*NeoPixel-------------------------------------------------------------------------*/
#include <NeoPixelBus.h>
#define PIXEL_PIN 22
#define PIXEL_COUNT 143
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT, PIXEL_PIN);

#include "FastLED.h"
#define NUM_LEDS 6
#define DATA_PIN 4
CRGBArray<NUM_LEDS> leds;

int RR=0,GG=0,BB=0,RRGGBB=0;

/*NeoPixel-------------------------------------------------------------------------*/


/*DS18B20(1-Wire)------------------------------------------------------------------*/
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is connected to GPIO19
#define ONE_WIRE_BUS 19
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

char temp1_c[10], temp2_c[10], temp3_c[10], temp4_c[10];
float temp1, temp2, temp3, temp4,Flow=300;

DeviceAddress sensor1 = { 0x28, 0xAA, 0xDE, 0x9, 0x38, 0x14, 0x1, 0x90 };
DeviceAddress sensor2 = { 0x28, 0xAA, 0xB5, 0xFC, 0x37, 0x14, 0x1, 0x5E };
DeviceAddress sensor3 = { 0x28, 0xAA, 0x97, 0xA5, 0x1A, 0x13, 0x2, 0x66 };
DeviceAddress sensor4 = { 0x28, 0xAA, 0xDD, 0x7D, 0x1A, 0x13, 0x2, 0x2F };
DeviceAddress sensor5 = { 0x28, 0xFF, 0x36, 0x46, 0x69, 0x14, 0x4, 0x24 };
/*DS18B20(1-Wire)------------------------------------------------------------------*/


/*WiFi-----------------------------------------------------------------------------*/
#include <WiFi.h>
#include <WiFiClient.h>


const char *ssid = "";
const char *password = "";
const char* server = "";


WiFiClient client;
//関数の宣言
void connectServer(float,float,float,float,float,float,float,float);

/*WiFi-----------------------------------------------------------------------------*/


/*NTP------------------------------------------------------------------------------*/
#include <time.h>
#define JST     3600* 9
/*NTP-----------------------------------------------------------------------------*/


void setup() {

 /*シリアル初期化-----------------------------*/
  Serial.begin(115200);

/*WiFi---------------------------------------
  Serial.print("\n\nStart\n");

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.printf("Connected, IP address: ");
  Serial.println(WiFi.localIP());
*/
/*NTP----------------------------------------*/
  //configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");


 /*NeoPixel----------------------------------*/
  //NeoPixelBusのLEDの初期化
  strip.Begin();
  strip.Show();

  // FastLEDの設定
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);


 /*PWM制御-----------------------------------*/
  // PWM制御の初期化
  // put your setup code here, to run once:
  ledcSetup(0, 60, 8);
  ledcAttachPin(PWMPin, 0);


  /*TFT_eSPI---------------------------------*/
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);


  /*BME280-----------------------------------*/
  delay(1000); //Take some time to open up the Serial Monitor
  uint8_t t_sb = 0; //stanby 0.5ms
  uint8_t filter = 4; //IIR filter = 16
  uint8_t osrs_t = 2; //OverSampling Temperature x2
  uint8_t osrs_p = 5; //OverSampling Pressure x16
  uint8_t osrs_h = 1; //OverSampling Humidity x1
  uint8_t Mode = 3; //Normal mode
  bme280i2c.ESP32_BME280_I2C_Init(t_sb, filter, osrs_t, osrs_p, osrs_h, Mode);


  //DS18B20(1-Wire)
  delay(1000);

}


void loop() {

  /*NTP
  time_t t;
  struct tm *tm;
  static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};

  t = time(NULL);
  tm = localtime(&t);
  Serial.printf(" %04d/%02d/%02d(%s) %02d:%02d:%02d\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,wd[tm->tm_wday],tm->tm_hour, tm->tm_min, tm->tm_sec);
*/

  /*PWM制御*/
  ledcWrite(0, power);

  if (power == 0) {
    diff = 5;
  } else if (power == 255) {
    diff = -5;
  }

  power += diff;



  /*BME280*/
  bme_get();


  /*NeoPixel*/
  //NeoPixelBus
  for(int i=0;i<PIXEL_COUNT;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.SetPixelColor(i,RgbColor(RR,GG,BB)); // Moderately bright green color.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }
  strip.Show(); // This sends the updated pixel color to the hardware.


  /*FastLED*/

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(GG,RR,BB);
  }

  FastLED.show();


  //
  if(RRGGBB == 0){
    RR=5;GG=0;BB=0;
    RRGGBB++;
  }else if (RRGGBB == 1) {
    RR=0;GG=5;BB=0;
    RRGGBB++;
  }else if (RRGGBB >= 2) {
    RR=0;GG=0;BB=5;
    RRGGBB=0;
  }


  //DS18B20(1-Wire)
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  //Serial.print("Sensor 1(*C): ");  Serial.println(sensors.getTempC(sensor1));
  temp1 = sensors.getTempC(sensor1);
  //Serial.print("Sensor 2(*C): ");  Serial.println(sensors.getTempC(sensor2));
  temp2 = sensors.getTempC(sensor2);
  //Serial.print("Sensor 3(*C): ");  Serial.println(sensors.getTempC(sensor3));
  temp3 = sensors.getTempC(sensor3);
  //Serial.print("Sensor 4(*C): ");  Serial.println(sensors.getTempC(sensor4));
  temp4 = sensors.getTempC(sensor4);


 //Serial.println("Hello World!"); Serial.print("PWM = "); Serial.println(power);
 //Serial.println(NN);
  Serial.println(RRGGBB);

  //PHPへ送信
  //関数に値を代入して実行
  connectServer(temp1,temp2,temp3,temp4,temperature,humidity,pressure,Flow);
  Serial.print(sensors.getTempC(sensor1));
  Serial.print(",");
  Serial.print(sensors.getTempC(sensor2));
  Serial.print(",");
  Serial.print(sensors.getTempC(sensor3));
  Serial.print(",");
  Serial.print(sensors.getTempC(sensor4));
  Serial.print(",");
  Serial.print(temp_c);
  Serial.print(",");
  Serial.print(hum_c);
  Serial.print(",");
  Serial.print(pres_c);
  Serial.print(",");
  Serial.println(Flow);


  /*TFT_eSPI*/
  int xpos;
  //Measure time to clear screen
  // Print numbers
  yield();
  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("PWM = ", xpos, 10, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawNumber(power, xpos, 10, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("Temperature = ", xpos, 40, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(temp_c, xpos, 40, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("Humidity = ", xpos, 80, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(hum_c, xpos, 80, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("Pressure =  ", xpos, 120, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(pres_c, xpos, 120, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("temp1 =  ", xpos, 160, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  snprintf(temp1_c, 10, "%.2f", temp1);
  tft.drawString(temp1_c, xpos, 160, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("temp2 =  ", xpos, 200, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  snprintf(temp2_c, 10, "%.2f", temp2);
  tft.drawString(temp2_c, xpos, 200, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("temp3 =  ", xpos, 240, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  snprintf(temp3_c, 10, "%.2f", temp3);
  tft.drawString(temp3_c, xpos, 240, 4);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("temp4 =  ", xpos, 280, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  snprintf(temp4_c, 10, "%.2f", temp4);
  tft.drawString(temp4_c, xpos, 280, 4);

 delay(1000);
}


/************** BME280 測定 *************************/
void bme_get(){
  bme280i2c.Read_All(&temperature, &pressure, &humidity);

  sprintf(temp_c, "%2.2lf", temperature);
  sprintf(hum_c, "%2.2lf", humidity);
  sprintf(pres_c, "%4.2lf", pressure);

  Serial.println(temperature);

/*
  Serial.println("-----------------------");
  Serial.print("Temperature = "); Serial.println(temp_c);
  Serial.print("Humidity = "); Serial.println(hum_c);
  Serial.print("Pressure = "); Serial.println(pres_c);
  Serial.println("-----------------------");
  Serial.flush();
*/
}


/************** PHPへ送信 *************************/
//サーバーに接続し値を送る
void connectServer(float t1,float t2,float  t3,float t4,float t5, float h,float p, float f) {
  if(client.connect(server, 80)){
    Serial.println("connected to server");
    // 指定のwebサーバのPHPスクリプトにGET送信
    client.print("GET /monitor/receive.php?temp0=");
    client.print(t1);
    client.print("&temp1=");
    client.print(t2);
    client.print("&temp2=");
    client.print(t3);
    client.print("&temp3=");
    client.print(t4);
    client.print("&temp4=");
    client.print(t5);
    client.print("&Humi=");
    client.print(h);
    client.print("&APress=");
    client.print(p);
    client.print("&Flow=");
    client.print(f);
    client.print(" HTTP/1.1\r\n");
    client.print("HOST: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  }
}
