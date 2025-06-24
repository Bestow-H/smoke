#ifndef _THREAD_H_
#define _THREAD_H_
#include "../control/define.h"
#include "../include/constant.h"
#include "../drivers/DaHeng/DaHengCamera.h"
#include "serial.h"
#include "../drivers/Mindvision/MidCamera.h"
#include "../drivers/Generic/GenericCamera.h"
#include "../drivers/HikVision/include/hikvision_camera.h"
#include <mutex>
#include <chrono>
#include <future>

using namespace std;
using namespace Horizon;

enum BufferSize
{

    IMGAE_BUFFER = 10


}; //lqq ：10 wty：5
    
class Factory
{
public:

    Factory() //by lqq
    {

    }


public:
    //wty
     cv::Mat image_buffer_[BufferSize::IMGAE_BUFFER];
     double timer_buffer_[IMGAE_BUFFER]; 
    
    //lqq
    //cv::Mat image_buffer_[50];
    //double timer_buffer_[50]; 
    
    volatile unsigned int image_buffer_front_ = 0;   // the produce index
    volatile unsigned int image_buffer_rear_ = 0;    // the comsum index 
    void producer();
    void consumer();

    void sr_serial();


public:

    cv::Mat img;
    std::vector<cv::Mat> frames; 

    mutex serial_mutex_; // 数据上🔓

    int fd;

    Horizon::DataControler data_controler_;
    Horizon::DataControler::VisionData visiondata; // 视觉向电控传数据
    Horizon::DataControler::Stm32Data stm32data;   // 电控向视觉发数据
    Horizon::DataControler::Stm32Data last_stm32_;

};
#endif

