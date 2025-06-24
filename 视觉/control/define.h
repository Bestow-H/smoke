#ifndef DEFINE_H
#define DEFINE_H

// 相机选择:
#define MIDVISION //英雄，飞机
//#define DAHENG1 //全向步兵
// #define DAHENG2 //串口版步兵
//#define HK //哨兵
// #define GENERIC //通过OpenCV默认api调用通用相机,无畸变校正，仅用于无工业相机时临时测试

// 功能选择:
#define Img_Show //程序图形界面，正式上车时禁用以提升运行速度
//#define Save_Video //保存视频内录，关闭Img_Show时即可进入采集模式，用于比赛时进行素材采集（此时不会采集UI）
//#define ARRMOR //绿灯上部装甲板的识别

//#define Nova_Get_Img //新版获取相机图像方法（海康相机必须启用，全向需禁用，其他相机无所谓） 
#define Get_Debug //在终端中显示所有接收到的16进制数据，仅用于串口通信模式

//#define CPU_Thread_ID_Lock //为每个线程锁定固定的cpu核心（视控一体通信模式必须启用）


#endif
