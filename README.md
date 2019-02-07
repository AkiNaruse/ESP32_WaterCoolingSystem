# ESP32_WaterCoolingSystem

使用パーツ・参考サイトまとめ  

* * *

### //TFT_eSPI

-   WINGONEER 3.5インチTFTタッチスクリーン、320x480解像度LCDディスプレイラズベリーパイ用  
      <https://www.amazon.co.jp/gp/product/B077XGK9MY/>  
 -   参考サイト  
      <https://macsbug.wordpress.com/2017/08/12/how-to-use-a-tft-display-for-raspberry-pie-with-esp32/>  

* * *

### //BME280　室内温・気圧

-   BME280搭載　温湿度・気圧センサモジュール  
      <https://www.switch-science.com/catalog/2236/>  
 -   参考サイト  
      <https://www.mgo-tec.com/blog-entry-esp32-bme280-sensor-library.html>  

* * *

### //DS18B20(1-Wire)　外気温・水温

-   DS18B20(Techcell)  
      <https://www.amazon.co.jp/gp/product/B07M886SBK/>  
 -   参考サイト  
      <https://randomnerdtutorials.com/esp32-with-multiple-ds18b20-temperature-sensors/>  

* * *

### //NeoPixel

-   ALITOVE WS2812B LEDテープ1m 144連 NeoPixel RGB TAPE LED 5050 SMD LEDテープライトピクセル 防水 黒いベース 5V DC  
    https://www.amazon.co.jp/gp/product/B01MYT6JH9/
 -   参考サイト  
      <http://ogimotokin.hatenablog.com/entry/2018/12/25/005300>  

-   NeoPixel スルーホールLED(半透明 5mm 5個入りパック)  
    https://www.switch-science.com/catalog/1863/
 -   参考サイト  
      <http://cammy.co.jp/technical/2016/01/23/arduino-007/>  

* * *

### //ラジエータファンPWM制御

-  PC用ファンの制御

 -   参考サイト  
      <https://garretlab.web.fc2.com/arduino/esp32/lab/ledc/index.html>  

* * *
### //水流量センサ

-  SEN0217 - WaterFlowSensor 水流量センサ
   http://akizukidenshi.com/catalog/g/gM-12548/

 -   参考サイト  
     http://www.eleki-jack.com/sengokufan/2011/12/seeed-studio2.html  
     (インターネットアーカイブは神)

* * *
### //FAN-LED制御

-  Riing 12 LED RGB 256  
   https://www.thermaltake.com/products-model.aspx?id=C_00002731&o=1  

  -   参考サイト  
      制御法思考中...

* * *
### //PHP

-  値を取得してPHPへ送信
  -   参考サイト  
      http://projectsbiotope.blogspot.com/2010/02/arduinoweb.html

* * *
### //WiFiClient & NTP

-  NTPで現在時間を取得
  -   参考サイト  
      http://www.autumn-color.com/archives/839
-  NTPで現在時間を取得
  -   参考サイト  
      http://projectsbiotope.blogspot.com/2010/02/arduinoweb.html
  -   参考サイト  
      http://www.shangtian.tokyo/entry/2017/06/19/232014
* * *
### //使用中のピン

<table>
  <tbody>
    <tr>
      <td colspan="5" align="center">HiLetgo ESP-32S</td>
    </tr>
    <tr>
      <td bgcolor="#ff0000" align="center"></td>
      <td align="center" bgcolor="#ff0000"><font COLOR="#ffffff"><b>3.3v</b></font></td>
      <td rowspan="18" align="center" bgcolor="#00cccc"></td>
      <td align="center" bgcolor="#000000"><font COLOR="#ffffff"><b>GND</b></font></td>
      <td align="center" bgcolor="#000000"></td>
    </tr>
    <tr>
      <td align="center">TFT_eSPI</td>
      <td align="center">RST</td>
      <td align="center">23</td>
      <td align="center">TFT_eSPI</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">36</td>
      <td align="center">22</td>
      <td align="center">NeoPixel-144連</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">39</td>
      <td align="center">1</td>
      <td align="center">NeoPixel-6連</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">34</td>
      <td align="center">3</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">35</td>
      <td align="center">21</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center">PWM制御</td>
      <td align="center">32</td>
      <td align="center" bgcolor="#000000"><font COLOR="#ffffff"><b>GND</b></font></td>
      <td align="center" bgcolor="#000000"></td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">33</td>
      <td align="center">19</td>
      <td align="center">DS18B20</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">25</td>
      <td align="center">18</td>
      <td align="center">TFT_eSPI</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">26</td>
      <td align="center">5</td>
      <td align="center">TFT_eSPI</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">27</td>
      <td align="center">17</td>
      <td align="center">BME280</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">14</td>
      <td align="center">16</td>
      <td align="center">BME280</td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">12</td>
      <td align="center">4</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#000000"></td>
      <td align="center" bgcolor="#000000"><font COLOR="#ffffff"><b>GND</b></font></td>
      <td align="center">0</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center">TFT_eSPI</td>
      <td align="center">13</td>
      <td align="center">2</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">9</td>
      <td align="center">15</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">10</td>
      <td align="center">8</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center"></td>
      <td align="center">11</td>
      <td align="center">7</td>
      <td align="center"></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#ff0000"></td>
      <td align="center" bgcolor="#ff0000"><font COLOR="#ffff00"><b>5V</b></font></td>
      <td align="center" bgcolor="#666666"><b>　 USB　</b></td>
      <td align="center">6</td>
      <td align="center"></td>
    </tr>
  </tbody>
</table>
