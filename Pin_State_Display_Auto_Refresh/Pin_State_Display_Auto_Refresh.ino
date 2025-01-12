/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : 3_2_4_Pin_State_Display_Auto_Refresh
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200128
程序目的/Purpose          : 使用NodeMCU建立基本服务器。该网页将显示引脚D3状态。同时状态会
                           每隔5秒钟更新一次。
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
 
***********************************************************************/
 
#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
 
#define buttonPin D3            // 按钮引脚D3
 
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
 
bool pinState;                      // 存储引脚状态用变量
 
void setup(){
  Serial.begin(9600);          // 启动串口通讯
  delay(10);
  Serial.println("");
 
  pinMode(buttonPin, INPUT_PULLUP); // 将按键引脚设置为输入上拉模式
 
  wifiMulti.addAP("511", "511511511"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU在启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
                                                               // 另外这里只存储了3个WiFi信息，您可以存储更多
                                                               // 的WiFi信息在此处。
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
  
  esp8266_server.begin();                  
  esp8266_server.on("/", handleRoot);      
  esp8266_server.onNotFound(handleNotFound);        
 
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}
 
void loop(){
  esp8266_server.handleClient();     // 处理http服务器访问
  pinState = digitalRead(buttonPin); // 获取引脚状态
}                                                                   
 
/* 以下函数处理网站首页的访问请求。此函数为本示例程序重点1
详细讲解请参见太极创客网站《零基础入门学用物联网》
第3章-第2节“通过网络服务将开发板引脚状态显示在网页中”的说明讲解。*/    
void handleRoot() {   //处理网站目录“/”的访问请求 
  esp8266_server.send(200, "text/html", sendHTML(pinState));  
}
 
/*
建立用于发送给客户端浏览器的HTML代码。此代码将会每隔5秒刷新页面。
通过页面刷新，引脚的最新状态也会显示于页面中
*/
String sendHTML(bool buttonState) {
  // 声明文档类型为HTML5，并开始HTML文档
  String htmlCode = "<!DOCTYPE html> <html>\n";

  // 开始Head部分，设置页面每5秒自动刷新一次
  htmlCode += "<head><meta http-equiv='refresh' content='5'/>\n";

  // 设置页面标题
  htmlCode += "<title>ESP8266 Button State</title>\n";

  // 开始CSS样式定义，设置HTML元素的字体、布局和对齐方式
  htmlCode += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  // 设置HTML元素的样式：
  // - 字体为Helvetica
  // - 显示方式为内联块级元素（通常使用block或inline）
  // - 上下外边距为0，左右外边距自动，使内容水平居中
  // - 文本居中对齐

  // 继续CSS样式定义，设置Body、H1和H3元素的样式
  htmlCode += "body{margin-top: 50px;} h1 {color: #FF0000;margin: 50px auto 30px;} h3 {color: #0000FF;margin-bottom: 50px;}\n";
  // 设置Body、H1和H3元素的样式：
  // - Body元素的上外边距为50像素
  // - H1元素的文本颜色为深灰色，上下外边距分别为50像素和30像素，左右外边距自动，使内容水平居中
  // - H3元素的文本颜色为深灰色，下外边距为50像素

  // 结束CSS样式定义
  htmlCode += "</style>\n";

  // 结束Head部分
  htmlCode += "</head>\n";

  // 开始Body部分
  htmlCode += "<body>\n";

  // 添加一个H1标题，显示“ESP8266 BUTTON STATE”
  htmlCode += "<h1>ESP8266 BUTTON STATE</h1>\n";
  htmlCode += "<h3>Test</h3>\n";//果然可行！！！
  

  // 根据按钮状态添加内容
  if (buttonState) {
    // 如果按钮状态为HIGH，添加一个段落显示“Button Status: HIGH”
    htmlCode += "<p>Button Status: HIGH</p>\n";//<p>：用于创建一个段落，包含文本内容。
  } else {
    // 如果按钮状态为LOW，添加一个段落显示“Button Status: LOW”
    htmlCode += "<p>Button Status: LOW</p>\n";
  }

  // 结束Body部分
  htmlCode += "</body>\n";

  // 结束HTML文档
  htmlCode += "</html>\n";

  // 返回生成的HTML字符串
  return htmlCode;
}
 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
