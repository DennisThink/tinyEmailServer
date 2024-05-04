# tinyEmailServer
tinyEmailServer


# 中文版

tinyEmailServer是一个用C++编写的邮件服务器,实行邮件协议解析处理和网络通信相分离的设计，支持不同的网络框架。目前计划先支持SMTP和POP3，在这两个功能完善以后，再考虑支持IMAP协议。


# 使用的第三方库

1. 网络库: 
asio
socket-cpp
2. 日志库:
 spdlog
3. 数据库: 
sqlite
SQLiteCPP
4. 测试库: 
doctest 

# English

tinyEmailServer is an Email server written by C++,the parse of protocal and network are designed in a cross way,that means it can support different network framework.It will support SMTP and POP3 at the beginning,and after the two features is finished,I will add IMAP protocal to this project.

# thirdparty lib

1. network lib:
asio:
socket-cpp:

2. log lib:
spdlog

3. database lib:
sqlite
SQLiteCPP

4. test lib:
doctest


