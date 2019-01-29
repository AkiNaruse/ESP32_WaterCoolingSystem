/*PWM制御--------------------------------------------------------------------------*/
static uint8_t brightness = 0;
static int diff = 5;
const int ledPin = 32;
/*PWM制御--------------------------------------------------------------------------*/


/*BME280　室内温・気圧--------------------------------------------------------------*/
#include "ESP32_BME280_I2C.h"
ESP32_BME280_I2C bme280i2c(0x76, 16, 17, 400000); //address, SCK, SDA, frequency
char temp_c[10], hum_c[10], pres_c[10];
/*BME280　室内温・気圧--------------------------------------------------------------*/


/*TFT_eSPI-------------------------------------------------------------------------*/
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
/*TFT_eSPI-------------------------------------------------------------------------*/


/*NeoPixel-------------------------------------------------------------------------*/
//#include <Adafruit_NeoPixel.h>
#include <NeoPixelBus.h>
//#include <EmotionalBlink.h>
//#define PIN 22 // LEDへの信号線をD8に繋ぐ
//#define NUMLED 8 // LEDの個数は1。数珠つなぎに複数個のLEDをつなげることも可能
#define PIXEL_PIN 22
#define PIXEL_COUNT 3
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMLED, PIN, NEO_RGB + NEO_KHZ800); //おまじない
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT, PIXEL_PIN);
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

DeviceAddress sensor1 = { 0x28, 0xAA, 0xDE, 0x9, 0x38, 0x14, 0x1, 0x90 };
DeviceAddress sensor2 = { 0x28, 0xAA, 0xB5, 0xFC, 0x37, 0x14, 0x1, 0x5E };
DeviceAddress sensor3 = { 0x28, 0xAA, 0x97, 0xA5, 0x1A, 0x13, 0x2, 0x66 };
DeviceAddress sensor4 = { 0x28, 0xAA, 0xDD, 0x7D, 0x1A, 0x13, 0x2, 0x2F };
DeviceAddress sensor5 = { 0x28, 0xFF, 0x36, 0x46, 0x69, 0x14, 0x4, 0x24 };
/*DS18B20(1-Wire)------------------------------------------------------------------*/


int NN=0;


void setup() {

 /*シリアル初期化----------------------------*/
  Serial.begin(115200);


 /*NeoPixel----------------------------------*/
  // NeoPixelのLEDの初期化
  //pixels.begin();
  strip.Begin();


 /*PWM制御*/
  // PWM制御の初期化
  // put your setup code here, to run once:
  ledcSetup(0, 60, 8);
  ledcAttachPin(ledPin, 0);


  /*TFT_eSPI*/
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);


  /*BME280*/
  delay(1000); //Take some time to open up the Serial Monitor
  uint8_t t_sb = 0; //stanby 0.5ms
  uint8_t filter = 4; //IIR filter = 16
  uint8_t osrs_t = 2; //OverSampling Temperature x2
  uint8_t osrs_p = 5; //OverSampling Pressure x16
  uint8_t osrs_h = 1; //OverSampling Humidity x1
  uint8_t Mode = 3; //Normal mode
  bme280i2c.ESP32_BME280_I2C_Init(t_sb, filter, osrs_t, osrs_p, osrs_h, Mode);


  //DS18B20(1-Wire)



  //delay(1000);



}


void loop() {

  /*TFT_eSPI*/
  int xpos;
  //Measure time to clear screen
  // Print numbers
  yield();
  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 10;
  xpos += tft.drawString("PWM = ", xpos, 10, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawNumber(brightness, xpos, 10, 4);

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


  /*PWM*/
  ledcWrite(0, brightness);

  if (brightness == 0) {
    diff = 5;
  } else if (brightness == 255) {
    diff = -5;
  }

  brightness += diff;



  /*BME280*/
  bme_get();

  /*NeoPixel
  // 2秒かけて、消灯→点灯(赤)→消灯をじんわりやる
  Blink.softly(&pixels, NUMLED, 0, 0, 55, 2000); //GRB
  // 1秒間パリピ点滅をする
  //Blink.likePartyPeople(&pixels, NUMLED, 1000);
  */
   for(int i=0;i<PIXEL_COUNT;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.SetPixelColor(i,RgbColor(0,0,10)); // Moderately bright green color.

    strip.Show(); // This sends the updated pixel color to the hardware.

    //delay(delayval); // Delay for a period of time (in milliseconds).

   }

  /*
  strip.SetPixelColor(0,RgbColor(0,0,10));
  strip.SetPixelColor(1,RgbColor(0,0,10));
  strip.SetPixelColor(2,RgbColor(0,0,10));
  strip.SetPixelColor(3,RgbColor(0,0,10));
  strip.SetPixelColor(4,RgbColor(0,0,10));
  strip.SetPixelColor(5,RgbColor(0,0,10));
  strip.SetPixelColor(6,RgbColor(0,0,10));
  strip.SetPixelColor(7,RgbColor(0,0,10));
  //6.LEDセルを更新するための関数っぽい。
  //pixels.show();
  strip.Show();
 */

  //DS18B20(1-Wire)
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");

  Serial.print("Sensor 1(*C): ");
  Serial.println(sensors.getTempC(sensor1));

  Serial.print("Sensor 2(*C): ");
  Serial.println(sensors.getTempC(sensor2));

  Serial.print("Sensor 3(*C): ");
  Serial.println(sensors.getTempC(sensor3));

  Serial.print("Sensor 4(*C): ");
  Serial.println(sensors.getTempC(sensor4));


 //Serial.println("Hello World!"); Serial.print("PWM = "); Serial.println(brightness);
 //Serial.println(NN);


 delay(1000);
}


/************** BME280 測定 *************************/
void bme_get(){
  byte temperature = (byte)round(bme280i2c.Read_Temperature());
  byte humidity = (byte)round(bme280i2c.Read_Humidity());
  uint16_t pressure = (uint16_t)round(bme280i2c.Read_Pressure());

  sprintf(temp_c, "%2d ℃", temperature);
  sprintf(hum_c, "%2d ％", humidity);
  sprintf(pres_c, "%4d hPa", pressure);

  Serial.println("-----------------------");
  Serial.print("Temperature = "); Serial.println(temp_c);
  Serial.print("Humidity = "); Serial.println(hum_c);
  Serial.print("Pressure = "); Serial.println(pres_c);
  Serial.println("-----------------------");
  Serial.flush();
}
