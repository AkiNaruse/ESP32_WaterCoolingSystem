/*リレー制御------------------------------------------------------------------------*/
int Relay_1=26,Relay_2=25,Relay_3=32,Relay_4=33,Relay_5=15,Relay_6=21;
int td=0,td2=0;//リレー温度判断
/*リレー制御------------------------------------------------------------------------*/


/*ボリューム制御------------------------------------------------------------------------*/
int vol_hist[8];   //直近8回分のAD値変換値の履歴
int vol_new;       //最新のAD変換値
int vol_sum = 0;   //加算値格納用
int i_oldest = 0;    //一番古いデータが格納されている配列index

int valbk[] = {0,0,0}; // 今回,前回,前々回
double val = 0;    //移動平均結果
int channel;
/*ボリューム制御------------------------------------------------------------------------*/


/*ボタン制御------------------------------------------------------------------------*/
#include <EasyButton.h>

// Arduino pin where the buttons are connected to
#define BUTTON_ONE_PIN 35
#define BUTTON_TWO_PIN 34

// Button1
EasyButton button1(BUTTON_ONE_PIN);
// Button2
EasyButton button2(BUTTON_TWO_PIN);
/*ボタン制御------------------------------------------------------------------------*/


/*PWM制御--------------------------------------------------------------------------*/
static uint8_t power = 240 ,FAN1_p = 0, FAN2_p = 0;
static int diff = 5;
const int FAN1Pin = 27 ,FAN2Pin = 14 ,PumpPin = 12;
/*PWM制御--------------------------------------------------------------------------*/


/*流量計----------------------------------------------------------------------------*/
volatile int num_pulse;
int rotate_value,rotate_value_before;
float flow_sum,flow_rate;
char c_flow_rate[6];
int ir_port = 39;
/*流量計----------------------------------------------------------------------------*/


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
#define NUM_LEDS 14
#define DATA_PIN 4
CRGBArray<NUM_LEDS> leds;

double RR=0,GG=0,BB=0,RRGGBB=0,BRT=1;
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

#define SENSER_BIT    10      // 精度の設定bit

char temp1_c[10], temp2_c[10], temp3_c[10], temp4_c[10], temp5_c[10];
float temp1, temp2, temp3, temp4, temp5, wt1, wt2, wt3, wt4, wt5;

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
const char* server_url = "";
IPAddress subnet(255, 255, 255, 0);      //
IPAddress DNS(129, 250, 35, 250);          //
IPAddress ip(1, 1, 1, 1);           // for fixed IP Address
IPAddress gateway(1, 1, 1, 1);        //


WiFiServer server(80);
WiFiClient client;

//関数の宣言
void connectServer(float,float,float,float,float,float,float,float);
void httpServer(float,float,float,float,float,float,float,float);

int value = 0;
/*WiFi-----------------------------------------------------------------------------*/


/*NTP------------------------------------------------------------------------------*/
#include <time.h>
#define JST     3600* 9
/*NTP-----------------------------------------------------------------------------*/


void setup() {

 /*シリアル初期化-----------------------------*/
  Serial.begin(115200);

/*WiFi---------------------------------------*/
  WiFi.config(ip, gateway, subnet, DNS);   // Set fixed IP address

  Serial.print("\n\nStart\n");

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.printf("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //
  server.begin();


/*NTP----------------------------------------*/
  configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");


 /*NeoPixel----------------------------------*/
  //NeoPixelBusのLEDの初期化
  strip.Begin();
  strip.Show();

  // FastLEDの設定
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);


 /*PWM制御-----------------------------------*/
  // PWM制御の初期化
  // put your setup code here, to run once:
  //FAN1
  ledcSetup(0, 25000, 8);
  ledcAttachPin(FAN1Pin, 0);
  //FAN2
  ledcSetup(1, 25000, 8);
  ledcAttachPin(FAN2Pin, 1);
  //Pump
  ledcSetup(2, 25000, 8);
  ledcAttachPin(PumpPin, 2);


  /*TFT_eSPI---------------------------------*/
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);


  /*DS18B20(1-Wire)--------------------------*/
  sensors.setResolution(SENSER_BIT);

  /*BME280-----------------------------------*/
  //delay(1000); //Take some time to open up the Serial Monitor
  uint8_t t_sb = 0; //stanby 0.5ms
  uint8_t filter = 4; //IIR filter = 16
  uint8_t osrs_t = 2; //OverSampling Temperature x2
  uint8_t osrs_p = 5; //OverSampling Pressure x16
  uint8_t osrs_h = 1; //OverSampling Humidity x1
  uint8_t Mode = 3; //Normal mode
  bme280i2c.ESP32_BME280_I2C_Init(t_sb, filter, osrs_t, osrs_p, osrs_h, Mode);

  //流量計初期処理
  pinMode(ir_port, INPUT);
  attachInterrupt(ir_port, sum_pulse, RISING);

  //ボタン制御
  button1.begin();
  // Initialize the button2
  button2.begin();
  // Add the callback function to be called when the button1 is pressed.
  button1.onPressed(onButton1Pressed);
  // Add the callback function to be called when the button2 is pressed.
  button2.onPressed(onButton2Pressed);

  //リレー
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  pinMode(Relay_5, OUTPUT);
  pinMode(Relay_6, OUTPUT);
  digitalWrite(Relay_1, HIGH);
  digitalWrite(Relay_2, HIGH);
  digitalWrite(Relay_3, HIGH);
  digitalWrite(Relay_4, HIGH);
  digitalWrite(Relay_5, HIGH);
  digitalWrite(Relay_6, HIGH);
  //delay(1000);

}


void loop() {

  /*NTP*/
  time_t t;
  struct tm *tm;
  static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};

  t = time(NULL);
  tm = localtime(&t);
  Serial.printf(" %04d/%02d/%02d(%s) %02d:%02d:%02d\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,wd[tm->tm_wday],tm->tm_hour, tm->tm_min, tm->tm_sec);

  int hour =(t / 3600 + 9) % 24;
  //Serial.println(hour);
  int min =t / 60 % 60;
  //Serial.println(min);
  int sec =t % 60;
  //Serial.println(sec);



  /*ボタン--------------------------------*/
  // Continuously read the status of the buttons
  button1.read();
  button2.read();


  /*PWM制御*/
  ledcWrite(0, FAN1_p);
  ledcWrite(1, FAN2_p);
  /*
    if (FAN2_p == 0) {
      diff = 5;
    } else if (FAN2_p == 255) {
    diff = -5;
    }
    FAN2_p += diff;
    */
  ledcWrite(2, power);
  //Serial.print("FAN2_p = ");
  //Serial.println(FAN2_p);


  /*BME280*/
  bme_get();


  //DS18B20(1-Wire)
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  //Serial.print("Sensor 1(*C): ");  Serial.println(sensors.getTempC(sensor1));
  temp1 = sensors.getTempC(sensor1);
  wt1 = temp1;
  //Serial.print("Sensor 2(*C): ");  Serial.println(sensors.getTempC(sensor2));
  temp2 = sensors.getTempC(sensor2);
  wt2 = temp2;
  //Serial.print("Sensor 3(*C): ");  Serial.println(sensors.getTempC(sensor3));
  temp3 = sensors.getTempC(sensor3);
  wt3 = temp3;
  //Serial.print("Sensor 4(*C): ");  Serial.println(sensors.getTempC(sensor4));
  temp4 = sensors.getTempC(sensor4);
  wt4 = temp4;
  //Serial.print("Sensor 5(*C): ");  Serial.println(sensors.getTempC(sensor5));
  temp5 = sensors.getTempC(sensor5);
  wt5 = temp5;

  /*ボリューム--------------------------------*/
  // 移動平均処理
  vol_new = analogRead(A0);  // A0ピンをAnalog入力とする
  Serial.print("analogRead = ");
  Serial.println(vol_new);

  vol_sum = vol_sum + vol_new;            // 合計値に最新の値を足す
  vol_sum = vol_sum - vol_hist[i_oldest]; //合計値から最古の値を引く
  vol_hist[i_oldest] = vol_new;           //最古の履歴を最新の値で上書き
  i_oldest++;                             //古いデータのインデックス値を１つ増やす
  if(i_oldest>=8){i_oldest=0;}            //配列のインデックスが範囲外なら0に戻す
  val = vol_sum/8;             //

  /*ボリューム全開との変化を比較
  if(chkchange(channel,val)){
    valbk[2] = valbk[1];  // 「前々回」を「前々前回」へ
    valbk[1] = valbk[0];  // 「前回」を「前々回」へ
    valbk[0] = val;       // 「今回」を「前回」へ
    //delay(10);        // 変化検知したら、少し間をあける
  }*/

  Serial.print("val = ");
  Serial.println(val);
  /*
  // read the input on analog pin 0:
  double sensorValue = analogRead(A0);
  sensorValue = map(sensorValue, def_MIN, def_MAX, val_MIN, val_MAX);
  BRT=(double)sensorValue/(double)10;
  //256=20*12.8
  if (BRT < 1) {BRT=1;}
  // print out the value you read:
  Serial.print("sensorValue = ");
  Serial.println(sensorValue);
  */


  //hour
  if (hour >= 18 ) {
    //LED ON
    BRT = val/320;
    if (BRT <= 1) {
      BRT = 1;
    }
  }else if(hour <= 2 ){
    //LED ON
    BRT = val/320;
    if (BRT <= 1) {
      BRT = 1;
    }
  }else{
    //LED OFF
    BRT = 0;
  }


  Serial.print("BRT = ");
  Serial.println(BRT);



  //LED配色設定
  if (temp1 <= 20) {
    BB=(20*BRT)-1;
    RR=0;
  }else if (temp1 >= 35) {
    BB=0;
    RR=(20*BRT)-1;
  }else{
    BB=0;
    RR=0;
    BB=((35-temp1)*1.33)*BRT;
    RR=((temp1-20)*1.33)*BRT;
  }
  Serial.print("BB = ");
  Serial.println(BB);
  Serial.print("RR = ");
  Serial.println(RR);

  /*NeoPixel*/
  //NeoPixelBus
  for(int i=0;i<PIXEL_COUNT;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.SetPixelColor(i,RgbColor(RR,GG,BB)); // Moderately bright green color.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }
  strip.Show(); // This sends the updated pixel color to the hardware.


  /*FastLED*/
  //リザーバ
  for(int i = 0; i < (NUM_LEDS-8); i++) {
    leds[i] = CRGB(RR,GG,BB);
  }
  //FAN
  for(int i = 6; i < NUM_LEDS; i++) {
    leds[i] = CRGB(RR,GG,BB);
  }

  FastLED.show();

  //Serial.println(RRGGBB);

  //PHPへ送信
  //関数に値を代入して実行
  Serial.println("temp1 |temp2 |temp3 |temp4 |tempe | hum  | press  |flow_r |");
  Serial.print(sensors.getTempC(sensor1));
  Serial.print(" |");
  Serial.print(sensors.getTempC(sensor2));
  Serial.print(" |");
  Serial.print(sensors.getTempC(sensor3));
  Serial.print(" |");
  Serial.print(sensors.getTempC(sensor4));
  Serial.print(" |");
  Serial.print(sensors.getTempC(sensor5));
  Serial.print(" |");
  Serial.print(hum_c);
  Serial.print(" |");
  Serial.print(pres_c);
  Serial.print(" |");
  Serial.println(flow_rate);

  //
  connectServer(wt1,wt2,wt3,wt4,wt5,humidity,pressure,flow_rate);
  //
  httpServer(wt1,wt2,wt3,wt4,wt5,humidity,pressure,flow_rate);


  /*TFT_eSPI*/
  int xpos;
  //Measure time to clear screen
  // Print numbers
  yield();

  tft.fillScreen(TFT_BLACK);

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

//--- Right -----------------------------------------------

  tft.setTextColor(TFT_RED, TFT_BLACK);
  xpos = 250;
  xpos += tft.drawString("Flow = ", xpos, 10, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(c_flow_rate, xpos-250, 10, 4);


  /*リレー制御_温度判断*/
  if(td > 0){
    if(temp1 > 35){//35度以上
      //FAN
      digitalWrite(Relay_1,LOW);//FAN1_ON
      digitalWrite(Relay_2,HIGH);//FAN2_ON
      FAN1_p = 127;
      FAN2_p = 255;
    }
    //温度ディレイタイムディクリメント
    td--;
  }else if(temp1 < 25){//25度以下
    digitalWrite(Relay_1,HIGH);//FAN1_OFF
    digitalWrite(Relay_2,LOW);//FAN2_OFF
    FAN1_p = 0;
    FAN2_p = 0;
  }else if(temp1 > 25 && temp1 < 30){//25度以上かつ30度以下
    digitalWrite(Relay_1,HIGH);//FAN1_OFF
    digitalWrite(Relay_2,HIGH);//FAN2_ON
    FAN1_p = 0;
    FAN2_p = 0;
    //温度ディレイタイムインクリメント
    td=600;
  }else if(temp1 > 30 && temp1 < 35){//30度以上かつ35度以下
    digitalWrite(Relay_1,HIGH);//FAN1_OFF
    digitalWrite(Relay_2,HIGH);//FAN2_ON
    FAN1_p = 0;
    FAN2_p = 127;
    //温度ディレイタイムインクリメント
    td=600;
  }else if(temp1 > 35){//35度以上
    digitalWrite(Relay_1,LOW);//FAN1_ON
    digitalWrite(Relay_2,HIGH);//FAN2_ON
    FAN1_p = 127;
    FAN2_p = 255;
    //温度ディレイタイムインクリメント
    td=600;
  }


  //hour
  if (hour >= 18 ) {
    //リレー6 ON
    digitalWrite(Relay_6, HIGH);
  }else if(hour <= 2 ){
    //リレー6 ON
    digitalWrite(Relay_6, HIGH);
  }else{
    //リレー6 OFF
    digitalWrite(Relay_6, LOW);
  }

  if (td2 <= 0) {
    //Outside temp4 temp5
    if (temp4 > temp5) {
      //リレー3-4 OFF
      digitalWrite(Relay_3,LOW);//Pump_OFF
      digitalWrite(Relay_4,LOW);//FAN_OUT_OFF
      //温度ディレイタイムインクリメント
      td2=600;
    }else{
      //リレー3-4 ON
      digitalWrite(Relay_3,HIGH);//Pump_ON
      digitalWrite(Relay_4,HIGH);//Pump_ON
      //温度ディレイタイムディクリメント
      td--;
    }
  }else{
    //リレー3-4 OFF
    digitalWrite(Relay_3,HIGH);//Pump_ON
    digitalWrite(Relay_4,HIGH);//FAN_OUT_ON
    //温度ディレイタイムディクリメント
    td--;
  }


  //流量計
  rotate_value_before=num_pulse;


  delay(1000);


  //流量計
  rotate_value=num_pulse;
  flow_rate = (rotate_value-rotate_value_before)* 60  / 7.5;
  //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour
  dtostrf(flow_rate, 4, 1, c_flow_rate);
  //dtostrf(celsius, 4, 1, c_celsius);
  //Serial.print (flow_rate, DEC);
  //Serial.println (" L/h ");

}


/************** 流量計 *************************/
void sum_pulse (){
   num_pulse++;
}
/************** 流量計 *************************/


/************** BME280 測定 *************************/
void bme_get(){
  bme280i2c.Read_All(&temperature, &pressure, &humidity);

  sprintf(temp_c, "%2.2lf", temperature);
  sprintf(hum_c, "%2.2lf", humidity);
  sprintf(pres_c, "%4.2lf", pressure);

  //Serial.print("BME280_temperature = ");
  //Serial.println(temperature);

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
void connectServer(float ct1,float ct2,float  ct3,float ct4,float ct5, float ch,float cp, float cf) {
  if (ct1 < 50 && ct2 < 50 && ct3 < 50 && ct4 < 50 && ct5 < 50 && ct1 > -10 && ct2 > -10 && ct3 > -10 && ct4 > -10 && ct5 > -10) {
    if(client.connect(server_url, 80)){
      Serial.println("connected to server");
      // 指定のwebサーバのPHPスクリプトにGET送信
      client.print("GET /monitor/receive.php?temp0=");
      client.print(ct1);
      client.print("&temp1=");
      client.print(ct2);
      client.print("&temp2=");
      client.print(ct3);
      client.print("&temp3=");
      client.print(ct4);
      client.print("&temp4=");
      client.print(ct5);
      client.print("&Humi=");
      client.print(ch);
      client.print("&APress=");
      client.print(cp);
      client.print("&Flow=");
      client.print(cf);
      client.print(" HTTP/1.1\r\n");
      client.print("HOST: ");
      client.println(server_url);
      client.println("Connection: close");
      client.println();
    }
  }
}

void httpServer(float ht1,float ht2,float  ht3,float ht4,float ht5, float hh,float hp, float hf) {
  WiFiClient client = server.available();
  if (client) {
    String blank_line = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
//        Serial.print(c);
        if (c == '\n') {
          if (blank_line.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html; charset=utf-8;");
            client.println();

            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><meta http-equiv=\"refresh\" content=\"60\"></head>");
            client.println("<body>");
            client.println("ESP32 - 温湿度センサ<br>");
            client.println("温度1: ");
            client.print(ht1);
            client.println("度 <br>");
            client.println("温度2: ");
            client.print(ht2);
            client.println("度 <br>");
            client.println("温度3: ");
            client.print(ht3);
            client.println("度 <br>");
            client.println("温度4: ");
            client.print(ht4);
            client.println("度 <br>");
            client.println("温度5: ");
            client.print(ht5);
            client.println("度 <br>");
            client.println("湿度: ");
            client.print(hh);
            client.println("％ <br>");
            client.println("気圧: ");
            client.print(hp);
            client.println("hPa <br>");
            client.println("流量: ");
            client.print(hf);
            client.println("h/L <br>");
            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          } else {
            blank_line = "";
          }
        } else if (c != '\r') {
          blank_line += c;
        }
      }
    }
    client.stop();
  }
}

// Callback function to be called when button1 is pressed
void onButton1Pressed() {
  Serial.println("Button1 has been pressed!");
  if (BRT > 12) {
    BRT = BRT+2.95;
  }else{
    BRT = 1;
  }
}

// Callback function to be called when button2 is pressed
void onButton2Pressed() {
  Serial.println("Button2 has been pressed!");
}
/*ボリューム全開との変化を比較
boolean chkchange(int channel,int val){
  byte chk1 = valbk[0];
  byte chk2 = valbk[1];
  byte chk3 = valbk[2];
  if((val != chk1)&&(val != chk2)&&(val != chk3)&&(chk1 != chk3)){
    return true;
  }
  if((val != chk1)&&(chk1 == chk2)&&(chk1 == chk3)&&(chk1 == 0)){
    return true;
  }
  return false;
}
*/
