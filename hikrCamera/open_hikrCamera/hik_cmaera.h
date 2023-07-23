#ifndef HIK_CAMERA
#define HIK_CAMERA

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <ctime>
#include <sstream>
#include "include/MvCameraControl.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

enum Camera_State{
    IDLE,
    OPENED,
    GRABBING,
    WRONG_STATE
};

// bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
// {
//     if (NULL == pstMVDevInfo)
//     {
//         printf("The Pointer of pstMVDevInfo is NULL!\n");
//         return false;
//     }
//     if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
//     {
//         int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
//         int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
//         int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
//         int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);
//         // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
//         printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
//         printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
//         printf("UserDefinedName: %s\n\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
//     }
//     else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
//     {
//         printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
//         printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
//     }
//     else
//     {
//         printf("Not support.\n");
//     }
//     return true;
// }


class HikCamera{
public:
    HikCamera(){}
    // HikCamera(MV_CC_DEVICE_INFO &camera_device);
    ~HikCamera();

    int init_camera(MV_CC_DEVICE_INFO *camera_device);

    void set_param(int now_mode, int my_color);
    //设置分辨率
    void set_resolution(int max_width, int roi_width, int roi_width_offset, int max_height, int roi_height, int roi_height_offset);
    
    int create_handle();

    int open_camera();
    void close_camera();

    void set_trigger_mode();

    unsigned int get_int_value(const char *Key);
    float get_float_value(const char *Key);
    unsigned int get_enum_value(const char *Key);
    bool get_bool_value(const char *Key);
    MVCC_STRINGVALUE get_string_value(const char *Key);

    void set_int_value(const char *Key, unsigned int iValue);
    void set_enum_value(const char *Key, unsigned int Value);
    void set_float_value(const char *Key, float fValue);
    void set_bool_value(const char *Key, bool bValue);
    void set_string_value(const char *Key, char* strValue);

    

    int grab_image();

    void get_image(Mat &M);
    void get_image_slow(Mat &M);


    int no_data_times = 0;
    string m_name = "camera";
    string m_ID = "none";
    void* handle = nullptr;
    int pre_mode=-1;

    //自动取图，录视频等
    void auto_recordImgs(int maxImgsSum);
    void manual_recordImgs();
    void recordVideo();


private:
    int nRet = MV_OK;
    MV_CC_DEVICE_INFO* pDeviceInfo = nullptr;
    MVCC_INTVALUE stParam;
    unsigned char* pData = nullptr;
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    int camera_state = Camera_State::IDLE;
    unsigned int nDataSize = 0;
    
};

class HikCameraManager{
public:

    HikCameraManager();

    ~HikCameraManager();

    void init_manager_for_one_camera();
    void init_manager_for_multi_camera();
    int enum_device();

    HikCamera* get_hik_camera(int index);



private:
    int nRet = MV_OK;
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    HikCamera* cameras = nullptr;
    int cameras_size=0;
};



#endif
