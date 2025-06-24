# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

project(rm_auto)

add_definitions(-std=c++11)
add_definitions(-DAPI_EXPORTS)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "debug")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})


# 库文件的放置目录
set(SDK_DIR /home/robo/tools/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.5.2303.9221/Galaxy_camera/lib/x86_64/libgxiapi.so)
# C++11 标准、o3优化、多线程
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread -Wall -Ofast -g -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=160000000")
set(OpenCV_DIR "/usr/local/include/opencv4")
set(TENSORRT_DIR "/home/robo/tools/TensorRT-8.5.1.7")

find_package(CUDA REQUIRED)
find_package(OpenCV  REQUIRED)
find_package(yaml-cpp REQUIRED)

#include_directories("/usr/include/eigen3") #new
include_directories(${OpenCV_INCLUDE_DIRS} )

include_directories(${CUDA_INCLUDE_DIRS})
# include_directories(src/yolo11)

# Add CUDA source files
set(CUDA_SOURCES src/yolo11/preprocess.cu)

# Add headers
set(HEADERS
    src/yolo11/yolov11.h
    src/yolo11/macros.h
    src/yolo11/logging.h
    src/yolo11/cuda_utils.h
    src/yolo11/preprocess.h
    src/yolo11/common.h
)

# Include TensorRT
include_directories(${TENSORRT_DIR}/include)
link_directories(${TENSORRT_DIR}/lib)
set(TENSORRT_LIBS nvinfer nvinfer_plugin nvparsers nvonnxparser)


include_directories(${PROJECT_SOURCE_DIR}/drivers)
include_directories(${PROJECT_SOURCE_DIR}/control)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/Camara)
include_directories(${PROJECT_SOURCE_DIR}/src/Serial)
include_directories(${PROJECT_SOURCE_DIR}/src/yolo11)

# 海康相机库文件的放置目录
set(HIKVISION_CAMERA_DIR "${PROJECT_SOURCE_DIR}/drivers/HikVision") # 设置海康相机SDK工具包目录
SET(YAML_CPP_LIBRARIES /usr/local/lib/libyaml-cpp.so) # 设置yaml-cpp库
# 指定相机动态链接库所在路径
LINK_DIRECTORIES(${HIKVISION_CAMERA_DIR}/64)
# 指定相机库目录
INCLUDE_DIRECTORIES(${HIKVISION_CAMERA_DIR}/include)
INCLUDE_DIRECTORIES(${HIKVISION_CAMERA_DIR}/tool/include)
# 相机拓展工具文件
FILE(GLOB_RECURSE sourcefiles "${HIKVISION_CAMERA_DIR}/src/*.cpp" "${HIKVISION_CAMERA_DIR}/tool/src/*.cpp")


# link the source files
aux_source_directory(./drivers/DaHeng SRC_FILES)
aux_source_directory(./src SRC_FILES)
aux_source_directory(./src/Camara SRC_FILES)
aux_source_directory(./src/Serial SRC_FILES)
aux_source_directory(./src/yolo11 SRC_FILES)


set(MV_SDK /home/robo/tools/linuxSDK_V2.1.0.37/lib/x64/libMVSDK.so)

cuda_add_executable(${PROJECT_NAME} ${SRC_FILES} ${sourcefiles} ${HEADERS} ${CUDA_SOURCES} ./src/main.cpp)
#set_target_properties(rm_auto PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})#new
target_link_libraries(${PROJECT_NAME} ${SDK_DIR})
target_link_libraries(${PROJECT_NAME} ${MV_SDK})

# 将spdlog的include目录添加到项目中
target_include_directories(${PROJECT_NAME} PRIVATE spdlog/include)  
target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} MvCameraControl -L/opt/MVS/lib/64)
#target_link_libraries(${PROJECT_NAME}/HikVision/64/libMvCameraControl.so.4.3.0.4)
target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})
# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${TENSORRT_LIBS}
)

