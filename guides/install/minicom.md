---
title: 'Guide: Install Minicom'
---

## MacOS
On mac os you can simply run 
```console
$ brew install minicom
```

## Ubuntu/WSL
To install on WSL/Ubuntu ou can run 
```console 
$ apt install minicom 
```

## recommended settings

You can launch minicom with flags, such as below 

```console 
$ mincom -D /dev/your-tty-device-here -w -c on -b 115200 
```

This line specifices the device you are listening on with minicom, `-w` for line wrapping
`-c on` to turn on color, and `-b 115200` sets the baud rate to 115200 baud. 

You can alternatively set these in minicom itself so you do not have to type all these flags every time. 
To open minicom settings type 
```console
$ mincom -s
```

You will see a small configurations screen, select 'Serial port setup' and hit enter. Here you can hit 'A' to start entering your serial device, enter your device and hit enter twice to confirm. To make sure this saves select 'save setup as dfl' to make this the default configuration. You can enter the rest of the settings in 'Screen and keyboard'. Feel free to play around here and see what works for you. Make sure to hit 'Save setup as dfl' to keep your changes next time you use minicom. Now you can just type `minicom` in terminal and it will connect automatically with no flags needed! 



