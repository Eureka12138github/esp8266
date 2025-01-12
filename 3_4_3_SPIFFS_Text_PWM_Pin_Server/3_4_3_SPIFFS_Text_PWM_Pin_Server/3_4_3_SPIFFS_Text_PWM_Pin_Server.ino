/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : 3_4_3_SPIFFS_Text_PWM_Server
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20190305
程序目的/Purpose          : 
使用ESP8266-NodeMCU建立一个有多个页面的网站。通过LED页面
的文本输入可以控制板上LED的亮度。
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200211     CYNO朔            0.01       一致性调整
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/
***********************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FS.h>  
 
ESP8266WiFiMulti wifiMulti;         // 建立ESP8266WiFiMulti对象
 
ESP8266WebServer esp8266_server(80);// 建立ESP8266WebServer对象，该对象用于响应HTTP请求。监听端口（80）

void setup(void){
  Serial.begin(9600);        
  Serial.println("");
  
  pinMode(LED_BUILTIN, OUTPUT);      // 初始化NodeMCU控制板载LED引脚为OUTPUT

  wifiMulti.addAP("511", "511511511"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  int i = 0;  
  while (wifiMulti.run() != WL_CONNECTED) { // 尝试进行wifi连接。
    delay(1000);
    Serial.print(i++); Serial.print(' ');
  }
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP

  if(SPIFFS.begin()){                       // 启动闪存文件系统
    Serial.println("SPIFFS Started.");
  } else {
    Serial.println("SPIFFS Failed to Start.");
  }                      
                 
  //初始化网络服务器
  esp8266_server.on("/LED-Control", handleLEDControl);   
  esp8266_server.onNotFound(handleUserRequest); // 处理其它网络请求

  // 启动网站服务
  esp8266_server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  esp8266_server.handleClient();  //处理网络请求
}                                
                                                                         
void handleLEDControl(){
  // 从浏览器发送的信息中获取PWM控制数值（字符串格式）
  String ledPwm = esp8266_server.arg("My_ledPwm"); 

  // 将字符串格式的PWM控制数值转换为整数
  int ledPwmVal = ledPwm.toInt();

  // 实施引脚PWM设置
  analogWrite(LED_BUILTIN, ledPwmVal);

  // 建立基本网页信息显示当前数值以及返回链接
  String httpBody = "Led PWM: " + ledPwm + "<p><a href=\"/LED.html\"><-LED Page</a></p>";           
  esp8266_server.send(200, "text/html", httpBody);
}

// 处理用户浏览器的HTTP访问
void handleUserRequest() {         
     
  // 获取用户请求资源(Request Resource）
  String reqResource = esp8266_server.uri();
  Serial.print("reqResource: ");
  Serial.println(reqResource);
  
  // 通过handleFileRead函数处处理用户请求资源
  bool fileReadOK = handleFileRead(reqResource);

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK){                                                 
    esp8266_server.send(404, "text/plain", "404 Not Found"); 
  }
}

bool handleFileRead(String resource) {            //处理浏览器HTTP访问

  if (resource.endsWith("/")) {                   // 如果访问地址以"/"为结尾
    resource = "/index.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  } 
  
  String contentType = getContentType(resource);  // 获取文件类型
  
  if (SPIFFS.exists(resource)) {                     // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(resource, "r");          // 则尝试打开该文件
    esp8266_server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}

// 获取文件类型
String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
