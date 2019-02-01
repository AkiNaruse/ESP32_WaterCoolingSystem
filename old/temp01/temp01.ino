//#include <memorysaver.h>

// UTFT_ViewFont (C)2014 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the included fonts.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
//240374PQ
//65K カラー
//320*240
//3.2 inch
//広視野角
//SSD1289:240 RGB x 320 TFT　ドライバー
//
// This program requires the UTFT library.
//
#include <SPI.h>
#include "printf.h"
//#include <Ethernet.h>
#include <UTFT.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#define BME280_ADDRESS 0x76
#include <OneWire.h>
#include "U8glib.h"

/*
byte mac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 }; //ほとんどの場合macアドレスはこのままで大丈夫です
IPAddress ip( 172,17,1,200 );            //イーサネットシールドのIPアドレス:LAN環境に合わせて設定変更が必要です
IPAddress gateway( 172,17,1,1 );          //デフォルトゲートウエイアドレス
IPAddress subnet(255, 255, 255, 0);      //サブネットマスク
IPAddress Logging_server(172,17,1,100);    //logを記録するwebサーバのIPアドレス
EthernetClient client;                    // イーサーネットクライアントライブラリ をイニシャライズ
*/

byte command;
float B=3435,T0=298.15,R0=10.0,R1=3.25,rr0,rr1,rr2,rr3,t0,t1,t2,t3;
int x=0,n0,n1,n2,n3,Fan1=1,Fan2=1;
//リレー温度判断
int td=0,td2=0;

/* Create an instance of the library for the 12864 LCD in SPI mode */
U8GLIB_ST7920_128X64_4X u8g(A15, A14, A13);  //128x64

//ファン制御用変数
unsigned long lastPulseTime;
unsigned long pulseInterval;

//リレーピン初期化
int relay_0 = 13;
int relay_1 = 12;
int relay_2 = 11;
int relay_3 = 10;
int relay_4 = 9;
int relay_5 = 8;

//温度計DS18B20
OneWire  ds(4);  // on pin 10 (a 4.7K resistor is necessary)
byte i,present = 0,type_s = 0,data[12],addr[8];
float celsius, fahrenheit;
char c_celsius[6];

//BME2890
double temp_act = 0.0, press_act = 0.0,hum_act=0.0;
signed long int temp_cal;
unsigned long int press_cal,hum_cal;
unsigned long int hum_raw,temp_raw,pres_raw;
signed long int t_fine;

 uint16_t dig_T1;
 int16_t dig_T2;
 int16_t dig_T3;
 uint16_t dig_P1;
 int16_t dig_P2;
 int16_t dig_P3;
 int16_t dig_P4;
 int16_t dig_P5;
 int16_t dig_P6;
 int16_t dig_P7;
 int16_t dig_P8;
 int16_t dig_P9;
 int8_t  dig_H1;
 int16_t dig_H2;
 int8_t  dig_H3;
 int16_t dig_H4;
 int16_t dig_H5;
 int8_t  dig_H6;

// Declare which fonts we will be using
//extern uint8_t SmallFont[];
//extern uint8_t BigFont[];
extern uint8_t hallfetica_normal[];
//extern uint8_t SevenSegNumFont[];
extern uint8_t SevenSegmentFull[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ITDB32S,38,39,40,41);

//流量計
volatile int num_pulse;
int rotate_value,rotate_value_before;
float flow_sum,flow_rate;
char c_flow_rate[6];
int ir_port = 3;
void sum_pulse (){ num_pulse++; }

void setup()
{
  Serial.begin(9600);
  
  myGLCD.InitLCD();
  myGLCD.clrScr();
  //myGLCD.setBrightness(0);

  myGLCD.setFont(hallfetica_normal);
  
  myGLCD.setColor(255, 200, 200);
  myGLCD.fillRect(0, 0, 319, 40);
  
  myGLCD.setColor(200, 200, 255);  
  myGLCD.fillRect(0, 120, 159, 160);
  
  myGLCD.setColor(200, 255, 200);  
  myGLCD.fillRect(160, 120, 319, 160); 
  
  myGLCD.setColor(70, 70, 255);
  myGLCD.setBackColor(255, 200, 200);
  myGLCD.print("MainInput", 0, 1);
  myGLCD.print("Water Temp", 0, 22);
  myGLCD.print("MainOutput", 160, 1);
  myGLCD.print("Water Temp", 160, 22);
  myGLCD.setBackColor(200, 200, 255);
  myGLCD.print("SubOutput", 0, 121);
  myGLCD.print("Water Temp", 0,142);
  myGLCD.setBackColor(200, 255, 200); 
  myGLCD.print("Room Temp", 168, 131);
  
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("`", 132, 103);
  myGLCD.print("c", 143, 103);
  myGLCD.print("`", 292, 103);
  myGLCD.print("c", 303, 103); 
  myGLCD.print("`", 132, 223);
  myGLCD.print("c", 143, 223);
  myGLCD.print("`", 292, 223);
  myGLCD.print("c", 303, 223); 
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawLine( 0, 119, 319, 119);
  myGLCD.drawLine( 159, 0, 159, 239);   
  
  //シリアル転送用インクリメント初期化
  //x=0;
  
  /* Set the font */
  u8g.setFont(u8g_font_8x13);  
  
  //リレーピンセット
  pinMode(relay_0,OUTPUT);
  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
  pinMode(relay_3,OUTPUT);
  pinMode(relay_4,OUTPUT);
  pinMode(relay_5,OUTPUT);
  
  digitalWrite(relay_0,HIGH);
  digitalWrite(relay_1,HIGH);  
  digitalWrite(relay_2,HIGH);
  digitalWrite(relay_3,HIGH);
  digitalWrite(relay_4,HIGH);
  digitalWrite(relay_5,HIGH);
  
  
  //パルス
  lastPulseTime = 0;
  pulseInterval = 1;
  //外部割り込みが発生したときに実行する関数を指定します。すでに指定されていた関数は置き換えられます。
  //ほとんどのArduinoボードは割り込み0(ピン2)と割り込み1(ピン3)と呼ばれる2つの外部割り込みに対応しています。
  //Arduino Megaはさらに、割り込み2(ピン21)、割り込み3(ピン20)、割り込み4(ピン19)、割り込み5(ピン18)が使用可能です。
  //呼び出せる関数は引数と戻り値が不要なものだけです。 
  //pinMode(2, INPUT);
  //attachInterrupt(0, senseRotation, FALLING);
  analogWrite(1, 0);
  analogWrite(2, 0);
  
  //流量計初期処理
  pinMode(ir_port, INPUT); 
  attachInterrupt(1, sum_pulse, RISING); 
  
  
  //BME280
    uint8_t osrs_t = 1;             //Temperature oversampling x 1
    uint8_t osrs_p = 1;             //Pressure oversampling x 1
    uint8_t osrs_h = 1;             //Humidity oversampling x 1
    uint8_t mode = 3;               //Normal mode
    uint8_t t_sb = 5;               //Tstandby 1000ms
    uint8_t filter = 0;             //Filter off 
    uint8_t spi3w_en = 0;           //3-wire SPI Disable
    
    uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
    uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
    uint8_t ctrl_hum_reg  = osrs_h;
    
    Wire.begin();
    
    writeReg(0xF2,ctrl_hum_reg);
    writeReg(0xF4,ctrl_meas_reg);
    writeReg(0xF5,config_reg);
    readTrim();                    //
  
}

void loop(){

  //温度計算 
  //temp0(計測範囲0℃まで)
  n0=analogRead(0);
  rr0=R1*n0/(1024.0-n0);
  t0=1/(log(rr0/R0)/B+(1/T0));
  if (t0 <= 273.15){ t0=273.15; }
  //temp2(計測範囲0℃まで)
  n1=analogRead(1);
  rr1=R1*n1/(1024.0-n1);
  t1=1/(log(rr1/R0)/B+(1/T0));
  if (t1 <= 273.15){ t1=273.15; }  
  //temp2(計測範囲0℃まで)
  n2=analogRead(2);
  rr2=R1*n2/(1024.0-n2);
  t2=1/(log(rr2/R0)/B+(1/T0));
  if (t2 <= 273.15){ t2=273.15; }
  //temp3(計測範囲-10℃まで)
  n3=analogRead(3);
  rr3=R1*n3/(1024.0-n3);
  t3=1/(log(rr3/R0)/B+(1/T0));
  if (t3 <= 263.15){ t3=273.15; }
  
  //温度計DS18B20
  if ( !ds.search(addr)) {
    ds.reset_search();
    //delay(250);
    return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);

 //温度判断
if(td > 0){
  if(t0-273.15 > 35){//35度以上
    //FAN
    digitalWrite(relay_0,LOW);//FAN_ON
    digitalWrite(relay_1,LOW);//FAN_12v
    //LED
    digitalWrite(relay_4,LOW);//BLUE_OFF
    digitalWrite(relay_5,LOW);//RED_ON
    Fan1=3;
    }
    //温度ディレイタイムディクリメント
    td--;
    
  }else if(t0-273.15 < 30){//30度以下
    digitalWrite(relay_0,HIGH);//FAN_OFF
    digitalWrite(relay_1,HIGH);//FAN_5v
    //LED
    digitalWrite(relay_4,HIGH);//BLUE_ON
    digitalWrite(relay_5,HIGH);//RED_OFF
    Fan1=1;
    
  }else if(t0-273.15 > 30 && t0-273.15 < 35){//30度以上かつ35度以下
    digitalWrite(relay_0,LOW);//FAN_ON
    digitalWrite(relay_1,HIGH);//FAN_5v
    //LED
    digitalWrite(relay_4,LOW);//BLUE_ON
    digitalWrite(relay_5,HIGH);//RED_ON
    Fan1=2;
    //温度ディレイタイムインクリメント
    td=600;

  }else if(t0-273.15 > 35){//35度以上
    digitalWrite(relay_0,LOW);//FAN_ON
    digitalWrite(relay_1,LOW);//FAN_12v
    //LED
    digitalWrite(relay_4,LOW);//BLUE_OFF
    digitalWrite(relay_5,LOW);//RED_ON
    Fan1=3;
    //温度ディレイタイムインクリメント
    td=600;
  }
  
  if(td2 > 0){
   td2--; 
  }else if(temp_act > celsius){
    //リレー3 OFF
    digitalWrite(relay_2,HIGH);
    //リレー4 OFF
    digitalWrite(relay_3,HIGH);
    Fan2=1;
    td2=3000;
  }else if(temp_act < celsius){
    //リレー3 ON
    digitalWrite(relay_2,LOW);
    //リレー4 ON
    digitalWrite(relay_3,LOW);
    Fan2=2;
    td2=3000;
  }
  
  
  //Pump(PWM)
  //int sensorValue;
  //int outputValue;
  //sensorValue = analogRead(0);
  //outputValue = map(sensorValue, 0, 1023, 0, 255);
  //analogWrite(4, outputValue);
  //↑A1にライトだめ！
  //lcd.clear();
  //lcd.setCursor(0,1);
  //lcd.print(outputValue);
  //lcd.setCursor(7,1);
  //lcd.print(sensorValue);         
  //unsigned long rpm = 60000000 / (pulseInterval * 2);
  //Serial.print (rpm);
  //Serial.println ("rpm");
  //lcd.setCursor(0,0);
  //lcd.print(rpm);
  //lcd.print("rpm");
 
 
  //液晶へ温度を表示
  myGLCD.setFont(SevenSegmentFull);
  myGLCD.setBackColor (0, 0, 0);
  myGLCD.setColor(70, 70, 255);
  myGLCD.printNumF(t0-273.15, 1, 15, 55);
  myGLCD.printNumF(t1-273.15, 1, 175, 55);
  myGLCD.printNumF(t2-273.15, 1, 15, 175);
  myGLCD.printNumF(temp_act, 1, 175, 175);

  /*
  //イーサ
  if (client.connect(Logging_server, 80)) {
  //Serial.println("connected to Logging server");
    client.print("GET /tempera_log.php?tempera=");   //   /cgi-bin/tempera_log.phpへ HTTP GETリクエストです:
    client.print( t0-273.15 );    //temp0
    client.print(",");
    client.print( t1-273.15 );    //temp1
    client.print(",");
    client.print( t2-273.15 );    //temp2
    client.print(",");
    client.print( t3-273.15 );    //temp3
    client.print(",");
    client.print( 0.0 );    //temp4
    client.print(",");
    client.print( 0.0 );    //hum0
    client.print(",");
    client.print( 0.0 );    //press0
    client.print(",");
    client.print( 0.0 );    //pump0
    client.print(",");
    client.print( 0.0 );    //pump1
    client.print(",");
    client.print (flow_rate);    //Flow0
    client.println(" HTTP/1.0");
    client.println();
  } else {
    // Serial.println("Logging server connection failed");
  }
  client.stop();
  */
  
  //BME2890
    
    readData();   
    temp_cal = calibration_T(temp_raw);
    press_cal = calibration_P(pres_raw);
    hum_cal = calibration_H(hum_raw);
    temp_act = (double)temp_cal / 100.0;
    press_act = (double)press_cal / 100.0;
    hum_act = (double)hum_cal / 1024.0;
    //Serial.print("TEMP : ");
    //Serial.print(temp_act);
    //Serial.print(" DegC  PRESS : ");
    //Serial.print(press_act);
    //Serial.print(" hPa  HUM : ");
    //Serial.print(hum_act);
    //Serial.println(" %"); 
 
  //流量計
  rotate_value_before=num_pulse;

  delay(1000);
  
  //温度計DS18B20
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); 

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3;
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;
    else if (cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  //Serial.print("  Temperature = ");
  //Serial.println(celsius);
  
 //流量計
 rotate_value=num_pulse;
 flow_rate = (rotate_value-rotate_value_before)* 60  / 7.5;  
 //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour
 dtostrf(flow_rate, 4, 1, c_flow_rate);
 dtostrf(celsius, 4, 1, c_celsius);
 // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
 
 //lcd.clear();
 //lcd.setCursor(0,0);
 //lcd.print (flow_rate);
 //lcd.print(" L/h ");
  
 // ここから、Processing にデータを送るための追加コード
 if( Serial.available() > 0 ){
   //Serial.read();
   // ここまで
   Serial.print(t0-273.15 );  //temp0
   Serial.print(",");
   Serial.print(t1-273.15 );  //temp0
   Serial.print(",");
   Serial.print(t2-273.15 );  //temp0
   Serial.print(",");
   Serial.print(temp_act );  //temp0
   Serial.print(",");
   Serial.print(celsius);  //temp0
   Serial.print(",");
   Serial.print(hum_act);  //hum0
   Serial.print(",");
   Serial.print(press_act);  //press0
   Serial.print(",");
   Serial.print(Fan1);  //pump0
   Serial.print(",");
   Serial.print(Fan2);  //pump1
   Serial.print(",");
   Serial.println (flow_rate);  //Flow0
 }else{
  //Serial.println ("Processing no connecting...");
  //シリアルへ温度出力(1分に一回出力)60 
  //x++;
  //if(x == 60){
   Serial.print(t0-273.15 );  //temp0
   Serial.print(",");
   Serial.print(t1-273.15 );  //temp0
   Serial.print(",");
   Serial.print(t2-273.15 );  //temp0
   Serial.print(",");
   Serial.print(temp_act );  //temp0
   Serial.print(",");
   Serial.print(celsius);  //temp0
   Serial.print(",");
   Serial.print(hum_act);  //hum0
   Serial.print(",");
   Serial.print(press_act);  //press0
   Serial.print(",");
   Serial.print(Fan1);  //pump0
   Serial.print(",");
   Serial.print(Fan2);  //pump1
   Serial.print(",");
   Serial.println (flow_rate);  //Flow0
  //x=0;
  //}
 } 
}

// ファン回転パルス立ち下がりエッジ検出用
void senseRotation(void) {
  unsigned long cur = micros();
  unsigned long dif = cur - lastPulseTime;
  pulseInterval = dif;
  lastPulseTime = cur;
}

//LCD
void draw(void) {
  u8g.drawFrame( 0, 0, 128, 64);
  u8g.drawLine(0, 10, 128, 10);
  u8g.drawFrame( 118, 2, 7, 7);
  u8g.drawLine(118, 2, 124, 8);
  u8g.drawLine(124, 2, 118, 8);
  u8g.drawFrame( 110, 2, 7, 7);
  u8g.drawFrame( 112, 4, 3, 3);
  u8g.drawFrame( 102, 2, 7, 7);
  u8g.drawLine(104, 6, 106, 6);
  
  u8g.drawStr(5, 25, "Flow rate");
  u8g.drawStr( 80, 35, c_flow_rate);
  u8g.drawStr(5, 47, "Outside temp");
  u8g.drawStr( 80, 60, c_celsius);
 
}


void readTrim(){
    uint8_t data[32],i=0;                      // Fix 2014/04/06
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(0x88);
    Wire.endTransmission();
    Wire.requestFrom(BME280_ADDRESS,24);       // Fix 2014/04/06
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
    
    Wire.beginTransmission(BME280_ADDRESS);    // Add 2014/04/06
    Wire.write(0xA1);                          // Add 2014/04/06
    Wire.endTransmission();                    // Add 2014/04/06
    Wire.requestFrom(BME280_ADDRESS,1);        // Add 2014/04/06
    data[i] = Wire.read();                     // Add 2014/04/06
    i++;                                       // Add 2014/04/06
    
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(0xE1);
    Wire.endTransmission();
    Wire.requestFrom(BME280_ADDRESS,7);        // Fix 2014/04/06
    while(Wire.available()){
        data[i] = Wire.read();
        i++;    
    }
    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    dig_P1 = (data[7] << 8) | data[6];
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11]<< 8) | data[10];
    dig_P4 = (data[13]<< 8) | data[12];
    dig_P5 = (data[15]<< 8) | data[14];
    dig_P6 = (data[17]<< 8) | data[16];
    dig_P7 = (data[19]<< 8) | data[18];
    dig_P8 = (data[21]<< 8) | data[20];
    dig_P9 = (data[23]<< 8) | data[22];
    dig_H1 = data[24];
    dig_H2 = (data[26]<< 8) | data[25];
    dig_H3 = data[27];
    dig_H4 = (data[28]<< 4) | (0x0F & data[29]);
    dig_H5 = (data[30] << 4) | ((data[29] >> 4) & 0x0F); // Fix 2014/04/06
    dig_H6 = data[31];                                   // Fix 2014/04/06
}
void writeReg(uint8_t reg_address, uint8_t data){
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();    
}


void readData(){
    int i = 0;
    uint32_t data[8];
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(0xF7);
    Wire.endTransmission();
    Wire.requestFrom(BME280_ADDRESS,8);
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
    pres_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    hum_raw  = (data[6] << 8) | data[7];
}


signed long int calibration_T(signed long int adc_T){
    
    signed long int var1, var2, T;
    var1 = ((((adc_T >> 3) - ((signed long int)dig_T1<<1))) * ((signed long int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int)dig_T1)) * ((adc_T>>4) - ((signed long int)dig_T1))) >> 12) * ((signed long int)dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T; 
}

unsigned long int calibration_P(signed long int adc_P){
    signed long int var1, var2;
    unsigned long int P;
    var1 = (((signed long int)t_fine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)dig_P6);
    var2 = var2 + ((var1*((signed long int)dig_P5))<<1);
    var2 = (var2>>2)+(((signed long int)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)dig_P1))>>15);
    if (var1 == 0){
        return 0;
    }    
    P = (((unsigned long int)(((signed long int)1048576)-adc_P)-(var2>>12)))*3125;
    if(P<0x80000000){
       P = (P << 1) / ((unsigned long int) var1);   
    }else{
        P = (P / (unsigned long int)var1) * 2;    
    }
    var1 = (((signed long int)dig_P9) * ((signed long int)(((P>>3) * (P>>3))>>13)))>>12;
    var2 = (((signed long int)(P>>2)) * ((signed long int)dig_P8))>>13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + dig_P7) >> 4));
    return P;
}

unsigned long int calibration_H(signed long int adc_H){
    signed long int v_x1;
    
    v_x1 = (t_fine - ((signed long int)76800));
    v_x1 = (((((adc_H << 14) -(((signed long int)dig_H4) << 20) - (((signed long int)dig_H5) * v_x1)) + 
              ((signed long int)16384)) >> 15) * (((((((v_x1 * ((signed long int)dig_H6)) >> 10) * 
              (((v_x1 * ((signed long int)dig_H3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) * 
              ((signed long int) dig_H2) + 8192) >> 14));
   v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)dig_H1)) >> 4));
   v_x1 = (v_x1 < 0 ? 0 : v_x1);
   v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
   return (unsigned long int)(v_x1 >> 12);   
}

