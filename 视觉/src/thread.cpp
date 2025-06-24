#include "../include/thread.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
// #include "Camara/Camara.h"
#include "Serial/SerialTool.h"
#include "Serial/SerialPack.h"
#include "yolo11/yolov11.h"
#include <stdlib.h>
#include <vector>

//Global Variable
//Serial ComBuffer Thread Lock
pthread_mutex_t SERIALTOOL_THREAD_COMBUFFER_MUTEX=PTHREAD_MUTEX_INITIALIZER;

//mutex image_mutex_{}; // 数据上🔓


namespace GxCamera
{
	// 大恒相机滑动条调参
	int GX_exp_time = 10000;
	int GX_gain = 10;
	DaHengCamera *camera_ptr_ = nullptr;
	int GX_blance_r = 50; // rbg颜色通道
	int GX_blance_g = 32;
	int GX_blance_b = 44;

	int GX_gamma = 1;

	// DaHengCamera* camera_ptr_ = nullptr;

	void DaHengSetExpTime(int, void *)
	{
		camera_ptr_->SetExposureTime(GX_exp_time);
	}

	void DaHengSetGain(int, void *)
	{
		camera_ptr_->SetGain(3, GX_gain);
	}

}
namespace MidCamera
{
	int MV_exp_value = 0;
	MVCamera *camera_ptr_ = nullptr;

	int r = 152;
	int g = 125;
	int b = 100;

	int GAIN = 100;

	void MVSetExpTime(int, void *)
	{
		camera_ptr_->SetExpose(MV_exp_value);
		
	}

	void MVSetGain_R(int, void *)
	{
		camera_ptr_->SetGain(r,g,b);
	}

	void MVSetGain_G(int, void *)
	{
		camera_ptr_->SetGain(r,g,b);
	}

	void MVSetGain_B(int, void *)
	{
		camera_ptr_->SetGain(r,g,b);
	}
	
	void MVSetGain(int, void *)
	{
		camera_ptr_->SetGain(GAIN,GAIN,GAIN);
	}

}
namespace GenericCamera
{
	GNCamera *camera_ptr_ = nullptr;
}
using namespace camera;
namespace HKcamera
{
	HikCamera *MVS_cap = nullptr;												// 创建一个相机对象
	const string camera_config_path = "../drivers/HikVision/config/camera_config.yaml"; // 相机配置文件路径
	const string intrinsic_para_path = "../param/camera_HK.yaml";				// 相机内参文件路径											// 记录相机初始化时间戳
	bool debug_flag = true;														// 是否开启相机调参
}

//[[noreturn]] void Factory::producer() //by lsn 全向通信出问题时可以尝试启用
void Factory::producer()
{
#ifdef DAHENG1
#ifdef Save_Video
	cv::Mat image;
	//    GxCamera::camera_ptr_->GetMat(image);
	int frame_cnt = 0;
	const std::string &storage_location = "../video/";
	char now[64];
	std::time_t tt;
	struct tm *ttime;
	int width = 1280;
	int height = 1024;
	tt = time(nullptr);
	ttime = localtime(&tt);
	strftime(now, 64, "%Y-%m-%d_%H_%M_%S", ttime); // 以时间为名字
	std::string now_string(now);
	std::string path(std::string(storage_location + now_string).append(".avi"));
	auto writer = cv::VideoWriter(path, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25.0, cv::Size(width, height)); // Avi format
	std::future<void> write_video;
	if (!writer.isOpened())
	{
		cerr << "Could not open the output video file for write\n";
		return;
	}
#endif
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	while (true)
	{
		if (GxCamera::camera_ptr_ != nullptr) // 打印图片
		{
			while (image_buffer_front_ - image_buffer_rear_ > IMGAE_BUFFER)
			{
			};
			if (GxCamera::camera_ptr_->GetMat(image_buffer_[image_buffer_front_ % IMGAE_BUFFER]))
			{
				// 调整后，把这段注释掉
				std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
                std::chrono::duration<double> time_run = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t0);
				timer_buffer_[image_buffer_front_ % IMGAE_BUFFER] = time_run.count();
				++image_buffer_front_;
				// 收数开始
#ifdef Save_Video
				frame_cnt++;
				cv::Mat src = image_buffer_[image_buffer_front_ % IMGAE_BUFFER];
				if (frame_cnt % 10 == 0)
				{
					frame_cnt = 0;
					// 异步读写加速,避免阻塞生产者
					write_video = std::async(std::launch::async, [&, src]()
											 { writer.write(src); });
				}
#endif
			}
			else
			{
				delete GxCamera::camera_ptr_;
				GxCamera::camera_ptr_ = nullptr;
			}
		}
		else
		{
			GxCamera::camera_ptr_ = new DaHengCamera;
			while (!GxCamera::camera_ptr_->StartDevice())
				;
			GxCamera::camera_ptr_->SetResolution();
			while (!GxCamera::camera_ptr_->StreamOn())
				;
			// 设置是否自动白平衡
			GxCamera::camera_ptr_->Set_BALANCE_AUTO(1);
			// 手动设置白平衡通道及系数，此之前需关闭自动白平衡

			GxCamera::camera_ptr_->SetExposureTime(GxCamera::GX_exp_time);
			GxCamera::camera_ptr_->SetGain(3, GxCamera::GX_gain);

			double GX_Gamma = 2.85;
			GxCamera::camera_ptr_->setGamma(GX_Gamma);

			cv::namedWindow("DaHengCameraDebug", cv::WINDOW_AUTOSIZE);
			cv::createTrackbar("DaHengExpTime", "DaHengCameraDebug", &GxCamera::GX_exp_time, 10000, GxCamera::DaHengSetExpTime);
			GxCamera::DaHengSetExpTime(0, nullptr);
			cv::createTrackbar("DaHengGain", "DaHengCameraDebug", &GxCamera::GX_gain, 10, GxCamera::DaHengSetGain);
			GxCamera::DaHengSetGain(0, nullptr);
			// GxCamera::DaHengSetGain(0,nullptr);

			image_buffer_front_ = 0;
			image_buffer_rear_ = 0;
		}
	}
#endif
#ifdef DAHENG2
#ifdef Save_Video
	cv::Mat image;
	//    GxCamera::camera_ptr_->GetMat(image);
	int frame_cnt = 0;
	const std::string &storage_location = "../video/";
	char now[64];
	std::time_t tt;
	struct tm *ttime;
	int width = 1280;
	int height = 1024;
	tt = time(nullptr);
	ttime = localtime(&tt);
	strftime(now, 64, "%Y-%m-%d_%H_%M_%S", ttime); // 以时间为名字
	std::string now_string(now);
	std::string path(std::string(storage_location + now_string).append(".avi"));
	auto writer = cv::VideoWriter(path, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25.0, cv::Size(width, height)); // Avi format
	std::future<void> write_video;
	if (!writer.isOpened())
	{
		cerr << "Could not open the output video file for write\n";
		return;
	}
#endif
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	while (true)
	{
		if (GxCamera::camera_ptr_ != nullptr) // 打印图片
		{
			while (image_buffer_front_ - image_buffer_rear_ > IMGAE_BUFFER)
			{
			};
			if (GxCamera::camera_ptr_->GetMat(image_buffer_[image_buffer_front_ % IMGAE_BUFFER]))
			{
				// 调整后，把这段注释掉
				std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
                std::chrono::duration<double> time_run = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t0);
				timer_buffer_[image_buffer_front_ % IMGAE_BUFFER] = time_run.count();
				++image_buffer_front_;
				// 收数开始
#ifdef Save_Video
				frame_cnt++;
				cv::Mat src = image_buffer_[image_buffer_front_ % IMGAE_BUFFER];
				if (frame_cnt % 10 == 0)
				{
					frame_cnt = 0;
					// 异步读写加速,避免阻塞生产者
					write_video = std::async(std::launch::async, [&, src]()
											 { writer.write(src); });
				}
#endif
			}
			else
			{
				delete GxCamera::camera_ptr_;
				GxCamera::camera_ptr_ = nullptr;
			}
		}
		else
		{
			GxCamera::camera_ptr_ = new DaHengCamera;
			while (!GxCamera::camera_ptr_->StartDevice())
				;
			GxCamera::camera_ptr_->SetResolution();
			while (!GxCamera::camera_ptr_->StreamOn())
				;
			// 设置是否自动白平衡
			GxCamera::camera_ptr_->Set_BALANCE_AUTO(1);
			// 手动设置白平衡通道及系数，此之前需关闭自动白平衡

			GxCamera::camera_ptr_->SetExposureTime(GxCamera::GX_exp_time);
			GxCamera::camera_ptr_->SetGain(3, GxCamera::GX_gain);

			double GX_Gamma = 2.85;
			GxCamera::camera_ptr_->setGamma(GX_Gamma);

			cv::namedWindow("DaHengCameraDebug", cv::WINDOW_AUTOSIZE);
			cv::createTrackbar("DaHengExpTime", "DaHengCameraDebug", &GxCamera::GX_exp_time, 10000, GxCamera::DaHengSetExpTime);
			GxCamera::DaHengSetExpTime(0, nullptr);
			cv::createTrackbar("DaHengGain", "DaHengCameraDebug", &GxCamera::GX_gain, 10, GxCamera::DaHengSetGain);
			GxCamera::DaHengSetGain(0, nullptr);
			// GxCamera::DaHengSetGain(0,nullptr);

			image_buffer_front_ = 0;
			image_buffer_rear_ = 0;
		}
	}
#endif

#ifdef MIDVISION

#ifdef Save_Video
	cv::Mat image;
	//    GxCamera::camera_ptr_->GetMat(image);
	int frame_cnt = 0;
	const std::string &storage_location = "../video/";
	char now[64];
	std::time_t tt;
	struct tm *ttime;
	int width = 1280;
	int height = 1024;
	tt = time(nullptr);
	ttime = localtime(&tt);
	strftime(now, 64, "%Y-%m-%d_%H_%M_%S", ttime); // 以时间为名字
	std::string now_string(now);
	std::string path(std::string(storage_location + now_string).append(".avi"));
	auto writer = cv::VideoWriter(path, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25.0, cv::Size(width, height)); // Avi format
	std::future<void> write_video;
	if (!writer.isOpened())
	{
		cerr << "Could not open the output video file for write\n";
		return;
	}
#endif

	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	while (true)
	{
		if (MidCamera::camera_ptr_ != nullptr)
		{

			// std::cout << "enter producer" << std::endl;
			while (image_buffer_front_ - image_buffer_rear_ > IMGAE_BUFFER - 1)
			{
			};
			// bool is = image_buffer_.try_lock();
			std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

			if (MidCamera::camera_ptr_->GetMat(image_buffer_[image_buffer_front_ % IMGAE_BUFFER]))
			{
				std::chrono::duration<double> time_run = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
				// std::cout << "time :" << time_run.count() << std::endl;

				MidCamera::camera_ptr_->SetExpose(MidCamera::MV_exp_value);

				// if (!is)
				// {
				// 	std::cout << "try lock failed!!" << std::endl;
				// }
				// std::cout << "enter producer lock" << std::endl;
				timer_buffer_[image_buffer_front_ % IMGAE_BUFFER] = time_run.count();
				// cv::imshow("windowName", image_buffer_[image_buffer_front_ % IMGAE_BUFFER]);
				++image_buffer_front_;

				// std::cout << "out producer lock" << std::endl;
#ifdef Save_Video
				frame_cnt++;
				cv::Mat src = image_buffer_[image_buffer_front_ % IMGAE_BUFFER];
				if (frame_cnt % 10 == 0)
				{
					frame_cnt = 0;
					// 异步读写加速,避免阻塞生产者
					write_video = std::async(std::launch::async, [&, src]()
											 { writer.write(src); });
				}
#endif
			}
			else
			{
				delete MidCamera::camera_ptr_;
				MidCamera::camera_ptr_ = nullptr;
			}
			// image_mutex_.unlock();
		}
		else
		{
			MidCamera::camera_ptr_ = new MVCamera;

			MidCamera::camera_ptr_->SetExpose(5000);
#ifdef Img_Show
			cv::namedWindow("MVCameraDebug", cv::WINDOW_AUTOSIZE);
			cv::createTrackbar("MVExpTime", "MVCameraDebug", &MidCamera::MV_exp_value, 60000, MidCamera::MVSetExpTime);
#endif
			MidCamera::MVSetExpTime(0, nullptr);

#ifdef Img_Show
			cv::namedWindow("MVCameraDebug", cv::WINDOW_AUTOSIZE);
			cv::createTrackbar("r_gain", "MVCameraDebug", &MidCamera::r, 160, MidCamera::MVSetGain_R);

			cv::namedWindow("MVCameraDebug", cv::WINDOW_AUTOSIZE);
			cv::createTrackbar("g_gain", "MVCameraDebug", &MidCamera::g, 160, MidCamera::MVSetGain_G);
			
			cv::namedWindow("MVCameraDebug", cv::WINDOW_AUTOSIZE);
			cv::createTrackbar("b_gain", "MVCameraDebug", &MidCamera::b, 160, MidCamera::MVSetGain_B);
#endif

			image_buffer_front_ = 0;
			image_buffer_rear_ = 0;
		}
	}
#endif
#ifdef HK
#ifdef Save_Video
			frame_cnt++;
			src = image_buffer_[image_buffer_front_ % IMGAE_BUFFER];
			if (frame_cnt % 10 == 0)
			{
				frame_cnt = 0;
				// 异步读写加速,避免阻塞生产者
				write_video = std::async(std::launch::async, [&, src]()
										 { writer.write(src); });
			}
#endif
	auto t0 = std::chrono::steady_clock::now(); // 记录相机初始化时间戳
	while (true)
	{
		if (HKcamera::MVS_cap != nullptr)
		{
			
			while (image_buffer_front_ - image_buffer_rear_ > IMGAE_BUFFER)
			{
				// std::cout << image_buffer_front_ - image_buffer_rear_ << std::endl;
			};
			if (HKcamera::MVS_cap->ReadImg(image_buffer_[image_buffer_front_ % IMGAE_BUFFER])) // 相机取图
			{
				auto t2 = std::chrono::steady_clock::now();
				std::chrono::duration<double> time_run = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t0);
				// HKcamera::MVS_cap->undistProcess(image); // 相机畸变矫正示例(取消注释即可使用)
				timer_buffer_[image_buffer_front_ % IMGAE_BUFFER] = time_run.count();
				++image_buffer_front_;
				
			}
			else
			{
				delete HKcamera::MVS_cap;
				HKcamera::MVS_cap = nullptr;
			}

#ifdef Save_Video
			frame_cnt++;
			cv::Mat src = image_buffer_[image_buffer_front_ % IMGAE_BUFFER];
			if (frame_cnt % 10 == 0)
			{
				frame_cnt = 0;
				// 异步读写加速,避免阻塞生产者
				write_video = std::async(std::launch::async, [&, src]()
										 { writer.write(src); });
			}
#endif
		}
		else
		{
			HKcamera::MVS_cap = new HikCamera;
			HKcamera::MVS_cap->Init(HKcamera::debug_flag, HKcamera::camera_config_path, HKcamera::intrinsic_para_path, t0); // 初始化相机，第一个参数为 动态调节相机参数模式
			HKcamera::MVS_cap->CamInfoShow();																				// 显示图像参数信息
			image_buffer_front_ = 0;
			image_buffer_rear_ = 0;
			
		}
	}
#endif 
#ifdef GENERIC
std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
while(true)
{

	while (image_buffer_front_ - image_buffer_rear_ > IMGAE_BUFFER - 1)
	{
	};
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	if (GenericCamera::camera_ptr_->GetMat(image_buffer_[image_buffer_front_ % IMGAE_BUFFER]))
	{
		std::chrono::duration<double> time_run = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
		timer_buffer_[image_buffer_front_ % IMGAE_BUFFER] = time_run.count();
		++image_buffer_front_;
	}
}
#endif
}

void Factory::consumer()
{

std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();


    //IMPORT LOCK
    unsigned char databuffcom[18];//comhave space 
    cv::Mat ORIGINMAT; //帧图
    //打开摄像头
    std::cout << "[main.cpp::main] 正在打开相机\n";
    //cv::VideoCapture capture(0, cv::CAP_V4L2); ;


#ifdef GENERIC
    //cout << "Built with OpenCV " << CV_VERSION << endl;
    VideoCapture capture;
    capture.open(0);
    if(capture.isOpened())
    {
        cout << "Capture 0 is opened" << endl;
        //capture >> img;
    }
    else
    {
		capture.open(1);
		if(capture.isOpened())
		{
			cout << "Capture 1 is opened" << endl;
			//capture >> img;
		}
		else
		{
			cout << "No capture" << endl;
		}
    }

#endif
	class Logger : public nvinfer1::ILogger {
		void log(Severity severity, const char* msg) noexcept override {
			// Only output logs with severity greater than warning
			if (severity <= Severity::kWARNING)
				std::cout << msg << std::endl;
		}
	}logger;



    //开启串口
    // GreenSpot greenSpot;               //目标识别
	YOLOv11 yolo(logger);
    SerialTool tool;
	    if(tool.init(databuffcom,18,"/dev/ttyUSB0",115200,8,'N',1)==0)
		{
			if(tool.init(databuffcom,18,"/dev/ttyUSB1",115200,8,'N',1)==0) 
			{
				if(tool.init(databuffcom,18,"/dev/ttyACM0",115200,8,'N',1)==0)
				{
					tool.init(databuffcom,18,"/dev/ttyACM1",115200,8,'N',1);
				}
			}
		}
	double X_BIAS, Y_BIAS;
	cv::FileStorage fs("../control/debug.yaml", cv::FileStorage::READ);
	fs["x"] >> X_BIAS;
	fs["y"] >> Y_BIAS;
	// std::cout<<"X:"<<X_BIAS<<";Y:"<<Y_BIAS<<std::endl;
	fs.release();

while (true)
	{

		// 若满足这个条件，则让这个函数一只停在这里
		//std::cout << "enter consum lock" << std::endl;
		//image_mutex_.lock();
		while (image_buffer_front_ <= image_buffer_rear_)
		{
		};
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		// 读取最新的图片
		image_buffer_rear_ = image_buffer_front_ - 1;
#ifndef GENERIC
#ifndef Nova_Get_Img
		// 直接获取引用（lqq，使用该方法时thread.h里不能有cv::Mat img;否则海康相机会出现无法画线的问题）
		cv::Mat &img = image_buffer_[image_buffer_rear_ % IMGAE_BUFFER];
#endif
#ifdef Nova_Get_Img
		cv::Mat img = image_buffer_[image_buffer_rear_ % IMGAE_BUFFER]; //mlx
#endif
#endif
#ifdef GENERIC
    	//cout << "Built with OpenCV " << CV_VERSION << endl;
		capture >> img;
#endif

		double src_time = timer_buffer_[image_buffer_rear_ % IMGAE_BUFFER];
		std::cout << "time :" << src_time << std::endl;


		// 读取图像
		// frames.clear(); 
		// frames.emplace_back(img); 
		cout<<"kuandu:"<<img.rows<<endl;
		cout<<"kuandu"<<img.cols<<endl;

        // 使用 GreenSpot 检测绿色区域
        // greenSpot.find(img);
		yolo.inference(img);

		
        // 按ESC退出
        if(cv::waitKey(30) == 27) break;

		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_run = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

		float FPS = 1 / (time_run.count());

		std::cout << "                 "
				  << "FPS: " << FPS << std::endl;
//	}
	//DartSocket dartsocket;             //套接字图传
    std::vector<cv::Point> targetData; //用于目标识别返回数据
    std::cout << "[main.cpp::main] 相机视频流启动\n";

    unsigned char senddata[18];
    int tempcount=0;


    unsigned int last[4]={0,0,0,0};
    //Index Thread
 //   while(1){
        SerialToolPack pack;
		std::vector<cv::Point>resul;
		resul=yolo.getTarget();
		// resul[0].x
		
		// pack.x1=resul[0].x;
		// pack.y1=resul[0].y;
		// pack.x2=resul[1].x;
		// pack.y2=resul[1].y;

        pack.x1=resul[0].x;
		pack.y1=resul[0].y;
		pack.x2=resul[1].x;
		pack.y2=resul[1].y;
		pack.x3=resul[2].x;
		pack.y3=resul[2].y;
		pack.x4=resul[3].x;
		pack.y4=resul[3].y;

        pack.pack(senddata,18);
    	tool.writeData(senddata,18);
		printf("PACK:x1=%u,y1=%u,x2=%u,y2=%u,x0=%u\n",pack.x1,pack.y1,pack.x2,pack.y2,pack.x0);
		
        //LOCK
        pthread_mutex_lock(&SERIALTOOL_THREAD_COMBUFFER_MUTEX);
            pack.unpack(senddata,18);
        //UNLOCK
        pthread_mutex_unlock(&SERIALTOOL_THREAD_COMBUFFER_MUTEX);
        
        last[0]=pack.x1;last[1]=pack.y1;last[2]=pack.x2;last[3]=pack.y2;
        printf("UNPACK:x1=%u,y1=%u,x2=%u,y2=%u\n",pack.x1,pack.y1,pack.x2,pack.y2);

        usleep(100);
    }
}
// void Factory::sr_serial()
// {

// 	//循环嵌套自适应串口
// 	fd = OpenPort("/dev/ttyACM0");
// 	if (fd == -1)
// 	{
// 		fd = OpenPort("/dev/ttyACM1");
// 		if (fd == -1)
// 		{
// 			fd = OpenPort("/dev/ttyUSB0");
// 			if (fd == -1)
// 			{
// 				fd = OpenPort("/dev/ttyUSB1");
// 				if (fd == -1)
// 				{
// 					fd = OpenPort("/dev/ttyTHS0");
// 					if (fd == -1)
// 					{
// 						fd = OpenPort("/dev/ttyTHS1");
// 					}
// 				}
// 			}
// 		}
// 	}

	
// 	configureSerial(fd);
// 	while (1)
// 	{
// 		// cv::waitKey(1);
// 		// cv::waitKey(2);
// 		if (fd == -1)
// 		{
// 			continue;
// 		}

// 		serial_mutex_.lock();
// 		data_controler_.getData(fd, stm32data);

// 		serial_mutex_.unlock();
// 	}
// }



