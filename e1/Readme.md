# 第一次实验：进程、信号量
## 进程实验
### `make menuconfig` ；回车选择process experiment；save 保存
### `make` 进行编译
### `./exper1.out 0` （数字0表示父进程sleep的时间，可以换为1、2...）
## 信号量实验
### `make menuconfig` ；回车选择mutex experiment；save 保存
### `make` 进行编译
### `./exper2.out` （向命令行输入字符或字符串观察结果；如果仅输入字符'q',则程序退出）
## 清除编译输出
### `make clean`即可