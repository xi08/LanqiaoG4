# lanqiao_test_g431

蓝桥杯 嵌入式设计与开发 测试代码

基于CT117E-M4平台，STM32G431RBT6芯片

## 开发与下载环境

MDK-ARM(Keil) V5+, 测试使用V5.38a

ARMCC V5/6 编译工具链, 测试时使用V5.06u7(-O3)和V6.19(-Os或-O3, 启用LTO)

STM32CubeMX V5.6.1+

STM32Cube FW_G4 V1.4.0

## 硬件连接

扩展板跳线接通SER/RCK/SCK/TRAO/AKEY/TDQ/HDQ

即启用数码管, 光敏电阻, 分压按键, DS18B20和DHT11的连接

## 实现功能

定时中断

按键识别

8位LED

LCD显示

串口任意发送与任意接收

启动次数检测

AT24C02读写

MCP4017控制

模拟按键/旋钮模拟输入

8段数码管

DS18B20温度传感器

DHT11温湿度传感器

方波频率检测

## 等待实现功能

蜂鸣器(CT117E-M4没有蜂鸣器)

方波占空比检测(写了, 但无法检测)
