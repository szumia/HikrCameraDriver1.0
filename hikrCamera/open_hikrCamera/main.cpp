#include "hik_cmaera.h"
#include "iostream"

using namespace std;
using namespace cv;


//#define RecordVideo
//#define ManualRecordPitcture
#define AutoRecordPicture

int main(){
    static HikCameraManager camera_manager;
    camera_manager.init_manager_for_one_camera();
    HikCamera* test_camera = camera_manager.get_hik_camera(0);

#if !defined(RecordVideo) && !defined(ManualRecordPitcture) && !defined(AutoRecordPicture)
    Mat M;
    float c_exposure = 1300;
    while(1){
        char c = waitKey(10);
        if(c=='e'){
            exit(0);
        }
        else if(c=='a'){
            // test_camera->set_float_value("Gain",10.0);
            MVCC_STRINGVALUE str = test_camera->get_string_value("DeviceSerialNumber");
            cout<<"a:"<<str.chCurValue<<endl;
        }
        else if(c=='r'){
            test_camera->set_param(0,1);
        }
        else if(c=='b'){
            test_camera->set_param(2,0);
        }
        else if(c=='+'){
            c_exposure+=50;
            test_camera->set_float_value("ExposureTime",c_exposure);
            cout<<"set exposure:"<<c_exposure<<endl;
        }
        else if(c=='-'){
            c_exposure-=50;
            test_camera->set_float_value("ExposureTime",c_exposure);
            cout<<"set exposure:"<<c_exposure<<endl;
        }
        test_camera->get_image(M);
        cv::namedWindow("ori_img",cv::WINDOW_NORMAL);
        imshow("ori_img",M);
    }
#endif

#ifdef AutoRecordPicture
    int savePicturesSum = 100;
    test_camera->auto_recordImgs(savePicturesSum);
#endif

#ifdef ManualRecordPitcture
    test_camera->manual_recordImgs();
#endif

#ifdef RecordVideo
    test_camera->recordVideo();
#endif

    return 0;
}