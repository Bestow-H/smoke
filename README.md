# smoke

smoking detect

***
## 这是吸烟识别项目的源代码
---
### 视觉模块 python版本
**使用设备：jetson Orin nano** \
**系统   ：Ubuntu22.04**    \
**文件   ：smoke_detect.py**

#### 需要的环境
    python 3.10
    YOLO v11

    此外，需要引入python的serial库

---

##### 特别提醒，ubuntu22.04的串口可能会有占用问题，需要重新配置。

> sudo dmesg | grep brltty 

如果你能收到这样的结果 

>[ 7033.078452] usb 1-13: usbfs: interface 0 claimed by ch341 while 'brltty' sets config #1

问题就很明显了，是驱动占用问题

>sudo apt remove brltty

然后重新拔插即可。

---

### 视觉模块 C++版本

**使用设备：jetson NX** \
**系统   ：Ubuntu20.04**\
**文件   ：视觉**

#### 需要的环境
    cuda     : 11.8 
    TensorRT : 8.5.1.7
    openCV   : 4.8.0
    
    需要自行配置相机驱动


---


