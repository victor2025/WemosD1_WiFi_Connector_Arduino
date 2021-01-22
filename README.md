# 基于ArduinoIDE的Wemos D1 WiFi连接驱动

## 简介
1.用于 Wemos D1 的WiFi搜索以及连接；
2.工程文件为ArduinoIDE工程文件；
3.单一功能的实现主要来自官方示例程序；
4.所有头文件均为ArduinoIDE中ESP8266的库；
5.编号在1.0.0以下的版本无法完整完成所有功能；
6.testCode文件夹为测试文件，对于主程序无影响。

## 基本实现功能
1.scan命令：WiFi搜索以及SSID、信号强度等的显示；
2.connect命令：通过串口输入命令进行WiFi的选择和连接(包括加密WiFi的密码输入);
3.disconnect命令：关闭WiFi;
4.status命令：当前网络状态的显示，获取当前IP地址;
5.acesspointon命令：通过串口输入命令开启AP移动热点(包括设置SSID和密码);
6.acesspointoff命令：AP的关闭;

