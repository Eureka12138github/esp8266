/**********************************************************************
项目名称/Project           : 零基础入门学用物联网
程序名称/Program name      : esp8266-flash-test
作者/Author               : Eureka 
日期/Date（YYYYMMDD）      : 20250105
程序目的/Purpose           : 此程序用于复习刚学到的闪存文件系统的相关操作
-----------------------------------------------------------------------
函数说明：
SPIFFS.open(file_name, "w"); 
以上函数有两个参数：
第一个参数是被操作的文件名称，本示例中该文件为/notes.txt
第二个参数"w" 代表写入文件信息。（如需了解如何读取信息，请参阅示例程序esp8266-flash-read）
***********************************************************************/
 
 
#include <FS.h>  
 
String file_name = "/taichi-maker/notes.txt"; //被读取的文件位置和名称
String file_name1 = "/taichi-maker/notes1.txt"; //被读取的文件位置和名称
String folder_name  = "/taichi-maker";
 
void setup() {
  Serial.begin(9600);
  Serial.println("");
  
  Serial.println("SPIFFS format start");
  SPIFFS.format();    // 格式化SPIFFS
  Serial.println("SPIFFS format finish");
  
  if(SPIFFS.begin()){ // 启动SPIFFS
    Serial.println("SPIFFS Started.");
  } else {
    Serial.println("SPIFFS Failed to Start.");
  }
  //创建两个文件，并分别添加内容
  File dataFile = SPIFFS.open(file_name, "w");// 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
  dataFile.println("Hello IOT World.");       // 向dataFile写入字符串信息
  dataFile.close();                           // 完成文件写入后关闭文件
  File dataFile1 = SPIFFS.open(file_name1, "w");// 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
  dataFile1.println("Hello IOT World.1");       // 向dataFile写入字符串信息
  dataFile1.close();  
  
//查询文件file_name是否存在
if (SPIFFS.exists(file_name)){
    Serial.print(file_name);
    Serial.println(" FOUND.");
  } else {
    Serial.print(file_name);
    Serial.print(" NOT FOUND.");
  }

  //读取文件file_name信息
  dataFile = SPIFFS.open(file_name, "r"); 
  //读取文件内容并且通过串口监视器输出文件信息
  for(int i=0; i<dataFile.size(); i++){
    Serial.print((char)dataFile.read());       
  }
  //完成文件读取后关闭文件
  dataFile.close(); 
  Serial.print("READ OK!"); 
  Serial.print("\n"); 

//确认闪存中是否有file_name文件，若有则添加信息
  if (SPIFFS.exists(file_name))
  {
    Serial.print(file_name);
    Serial.println(" FOUND.");
 
    dataFile = SPIFFS.open(file_name, "a");// 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
    dataFile.println("This is Appended Info."); // 向dataFile添加字符串信息
    dataFile.close();                           // 完成文件操作后关闭文件   
    Serial.println("Finished Appending data to SPIFFS");
    
  }
  else 
  {
    Serial.print(file_name);
    Serial.print(" NOT FOUND.");
  }

  //输出文件file_name信息
  Serial.print(file_name);
  Serial.print(":");
  Serial.print("\n"); 
  dataFile = SPIFFS.open(file_name, "r"); 
  //读取文件内容并且通过串口监视器输出文件信息
  for(int i=0; i<dataFile.size(); i++){
    Serial.print((char)dataFile.read());       
  }
  //完成文件读取后关闭文件
  dataFile.close();

 //输出根目录下文件名称
 Serial.print("florder:"); 
 Serial.print("\n"); 
 Dir dir = SPIFFS.openDir(folder_name);  // 建立“目录”对象
 while (dir.next()) 
 {// dir.next()用于检查目录中是否还有“下一个文件”
  Serial.println(dir.fileName()); // 输出文件名
 }
 
//从闪存中删除file_name文件
if (SPIFFS.remove(file_name))
{ 
    Serial.print(file_name);
    Serial.println(" remove sucess"); 
} 
else
{
    Serial.print(file_name);
    Serial.println(" remove fail");
} 

//再次输出根目录下文件名称
 dir = SPIFFS.openDir(folder_name);  // 建立“目录”对象
  while (dir.next()) 
  {  // dir.next()用于检查目录中是否还有“下一个文件”
    Serial.println(dir.fileName()); // 输出文件名
  }

//输出文件file_name1信息
  dataFile1 = SPIFFS.open(file_name1, "r"); 
  //读取文件内容并且通过串口监视器输出文件信息
  for(int i=0; i<dataFile1.size(); i++){
    Serial.print((char)dataFile1.read());       
  }
  //完成文件读取后关闭文件
  dataFile1.close();
  
}
 
void loop() {
}
