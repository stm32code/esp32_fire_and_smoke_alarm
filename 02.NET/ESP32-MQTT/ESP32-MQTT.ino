#include <WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
//DHT11
#include <Bonezegei_DHT11.h>
//OLED
#include <U8g2lib.h>
#include <Wire.h>
//JSON
#include <ArduinoJson.h>

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "NET";
const char* password = "12345678";
const char* mqttServer = "iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com";
// 如以上MQTT服务器无法正常连接，请前往以下页面寻找解决方案
// http://www.taichi-maker.com/public-mqtt-broker/

Ticker ticker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
int count;  // Ticker计数用变量
int count_2;
// ****************************************************
// 注意！以下需要用户根据然也物联平台信息进行修改！否则无法工作!
// ****************************************************

const char* subTopic = "/broadcast/k1apcUUJqzs/test1";                                                                       // 订阅主题(需要修改)
const char* pubTopic = "/broadcast/k1apcUUJqzs/test2";                                                                       // 订阅主题(需要修改)
const char* willTopic = "/broadcast/k1apcUUJqzs/test2";                                                                      // 遗嘱主题名称(需要修改)
// ****************************************************

//遗嘱相关信息
const char* willMsg = "esp8266 offline";  // 遗嘱主题信息
const int willQos = 0;                    // 遗嘱QoS
const int willRetain = false;             // 遗嘱保留

const int subQoS = 1;            // 客户端订阅主题时使用的QoS级别（截止2020-10-07，仅支持QoS = 1，不支持QoS = 2）
const bool cleanSession = true;  // 清除会话（如QoS>0必须要设为false）

// 模式选择
int Flage = 0;


// 火焰传感器 配置
#define FIRE 23
int fire = 0;

//DHT11
Bonezegei_DHT11 dht(4);
int humidity = 0;
float temperature = 0;
//MQ2
#define MQ2_PIN 32
int mq2 = 0;
 

#define SCL 22
#define SDA 21
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, /*reset=*/U8X8_PIN_NONE);
char str[50];  //拼接字符使用

void setup() {

  Serial.begin(9600);  // 启动串口通讯
  // Dht11
  dht.begin();
  //OELD
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_16_tf);  //设定字体  u8g2_font_ncenB08_tr 
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
 
  // 连接MQTT服务器
  connectMQTTserver();
  // Ticker定时对象
  ticker.attach(1, tickerCount);
  
  // 火焰传感器
  pinMode(FIRE, INPUT_PULLUP);                // 引脚为输入模式

  digitalWrite(LED_BUILTIN, ledStatus);  // 启动后关闭板上LED
  digitalWrite(BEEP, ledStatus);  // 启动后关闭板上LED
}
void loop() {
  // 如果开发板未能成功连接服务器，则尝试连接服务器
  if (!mqttClient.connected()) {
    connectMQTTserver();
  } else {
    //1秒更新一次
    if (count_2 >= 1) {
    
      u8g2.clearBuffer();
      sprintf(str, "Temp: %.2f  C", temperature);
      u8g2.drawStr(0, 16, str);
      sprintf(str, "Humi: %02d  %%", humidity);
      u8g2.drawStr(0, 32, str);
      sprintf(str, "Smog:  %03d  ppm", mq2);
      u8g2.drawStr(0, 48, str);
      u8g2.sendBuffer();  //显示
      Serial.printf("Temperature: %0.1lf °C  Humidity:%d %\n", temperature, humidity);

      // 实现LED闪烁
      if (ledStatus == LOW) {
        ledStatus = HIGH;
        digitalWrite(LED_BUILTIN, ledStatus);  // 则点亮LED。

      } else {
        ledStatus = LOW;
        digitalWrite(LED_BUILTIN, ledStatus);  // 则点亮LED。
      }
    
      count_2 = 0;
    }
  
  }

  // 处理信息以及心跳
  mqttClient.loop();

}

//计时器
void tickerCount() {
  count++;
  count_2++;
}
// 连接MQTT服务器并订阅信息
void connectMQTTserver() {
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  /* 连接MQTT服务器
  boolean connect(const char* id, const char* user, 
                  const char* pass, const char* willTopic, 
                  uint8_t willQos, boolean willRetain, 
                  const char* willMessage, boolean cleanSession); 
  若让设备在离线时仍然能够让qos1工作，则connect时的cleanSession需要设置为false                
                  */
  if (mqttClient.connect(clientId, mqttUserName,
                         mqttPassword, willTopic,
                         willQos, willRetain, willMsg, cleanSession)) {
   
    subscribeTopic();  // 订阅指定主题

  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(5000);
  }
}

// 收到信息后的回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Received [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  pubMQTTmsg();
}

// 订阅指定主题
void subscribeTopic() {
  // 通过串口监视器输出是否成功订阅主题以及订阅的主题名称
  // 请注意subscribe函数第二个参数数字为QoS级别。这里为QoS = 1
  if (mqttClient.subscribe(subTopic, subQoS)) {
    Serial.println(subTopic);
  } else {
    Serial.print("Subscribe Fail...");
    connectMQTTserver();  // 则尝试连接服务器
  }
}

// 发布信息
void pubMQTTmsg() {
  char pubMessage[254];
  // 数据流
  MQTT_FillBuf(pubMessage);
 
}

// ESP8266连接wifi
void connectWifi() {
 
  //等待WiFi连接,成功连接后输出成功信息
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.println("");
}
// 读取DHT11数值
void Read_Dht11() {

  if (dht.getData()) {                   // get All data from DHT11
    temperature = dht.getTemperature();  // return temperature in celsius
    humidity = dht.getHumidity();        // return humidity
  }
}
 
// 数据封装
unsigned char MQTT_FillBuf(char* buf) {

  char text[256];
  memset(text, 0, sizeof(text));

  strcpy(buf, "{");

  memset(text, 0, sizeof(text));
  sprintf(text, "\"led\":\"%d\",", led_1_state);
  strcat(buf, text);
 
  memset(text, 0, sizeof(text));
  sprintf(text, "}");
  strcat(buf, text);

  return strlen(buf);
}

// 解析json数据
void massage_parse_json(char* message) {
  // 声明一个 JSON 文档对象
  StaticJsonDocument<200> doc;

  // 解析 JSON 数据
  DeserializationError error = deserializeJson(doc, (const char*)message);

  // 检查解析是否成功
  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }
 
}
