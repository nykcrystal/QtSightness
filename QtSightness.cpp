#include "QtSightness.h"
#include"gammaramp.h"
#include<qslider.h>
#include<QTime>
#include<qspinbox.h>
#include<qtimeline.h>
#include<qdebug.h>
Ptr<cv::face::FaceRecognizer> model;
string str;
int flag;
RNG g_rng(12345);
QtSightness::QtSightness(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.spinBox_2->setMinimum(0);
    
    ui.spinBox->setMinimum(0);
    ui.spinBox->setMaximum(100);
    ui.spinBox_2->setMaximum(100);
    ui.spinBox->setSingleStep(1);
    ui.spinBox_2->setSingleStep(1);
    
    ui.verticalSlider_color->setMinimum(0);
    ui.verticalSlider_brightness->setMinimum(0);
    ui.verticalSlider_color->setMaximum(100);
    ui.verticalSlider_brightness->setMaximum(100);
    ui.verticalSlider_brightness->setSingleStep(1);
    ui.verticalSlider_color->setSingleStep(1);

    ui.timeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    ui.timeEdit->setDateTime(QDateTime::currentDateTime());

    connect(ui.verticalSlider_color, &QSlider::valueChanged, ui.spinBox, &QSpinBox::setValue);
    connect(ui.verticalSlider_color, &QSlider::valueChanged, this, &QtSightness::colorTemperature);
   // connect(ui.verticalSlider_color, &QSlider::valueChanged, this, &QtSightness::kttoRGB);
       
      
       
    connect(ui.verticalSlider_brightness, &QSlider:: valueChanged, this, [=] {
        int brightness_value = ui.verticalSlider_brightness->value();
        gammaramp Gammarmap;
        Gammarmap.SetBrightness(NULL, brightness_value);
        });
    connect(ui.verticalSlider_brightness, &QSlider::valueChanged, ui.spinBox_2, &QSpinBox::setValue);
    connect(ui.btn_begin_simulation, &QPushButton::clicked, this, &QtSightness::deteface);
    
}
void QtSightness::raseBrightness()
{
    
    gammaramp Grammarmap;
    for (int i = 0; i < 130; i++)
    {
        Grammarmap.SetBrightness(NULL, 125+i);
    }
  
     Sleep(3000);
     Grammarmap.SetBrightness(NULL, 130);
}
void QtSightness::declineBrightness()
{
    gammaramp Grammarmap;
    for (int i = 0; i > 110; i--) 
    {
        Grammarmap.SetBrightness(NULL, 128-i);
    }
    Sleep(3000);
    Grammarmap.SetBrightness(NULL, 100);
}
void QtSightness::deteface()
{
    VideoCapture cap(0, cv::CAP_DSHOW);
    if (!cap.isOpened())
    {
        return;
    }
    const static Scalar colors[] = {
    CV_RGB(0,0,255),
    CV_RGB(0,128,255),
    CV_RGB(0,255,255),
    CV_RGB(0,255,0),
    CV_RGB(255,128,0),
    CV_RGB(255,255,0),
    CV_RGB(255,0,0),
    CV_RGB(255,0,255)
    };
    Mat fream;
    Mat gray;
    CascadeClassifier cascade,eyescascade;
    cascade.load("D:\\QT\\qt(ui)\\mywidge\\haarcascade_frontalface_alt2.xml");
    eyescascade.load("D:\\QT\\qt(ui)\\mywidge\\haarcascade_eye_tree_eyeglasses.xml");
    model = face::FisherFaceRecognizer::create();
    //加载训练好的模型
    model->read("D:\\QT\\qt(ui)\\MyFaceFisherModel.xml");
    while (1)
    {
        
        cap >> fream;
        string laststr;
        vector<Rect>eyes;
        vector<Rect> faces(0);//用于存放人脸的向量容器
       
        cvtColor(fream, gray, CV_RGB2GRAY);
        equalizeHist(gray, gray);//直方图均衡化
        cascade.detectMultiScale(gray, faces,
            1.1, 4, 0
            | CASCADE_SCALE_IMAGE,
            Size(30, 30), Size(500, 500));
        eyescascade.detectMultiScale(gray, eyes, 1.3, 4, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
        if (faces.empty()) {
            gammaramp Grammaramp;
            Grammaramp.SetBrightness(NULL, 60);

        }
        Mat* pImage_roi = new Mat[faces.size()];
        
        Point text_lb;//显示文本
        //string str;
        for (int i = 0; i < faces.size(); i++)
        {
            pImage_roi[i] = gray(faces[i]);
            text_lb = Point(faces[i].x, faces[i].y);
            if (pImage_roi[i].empty())
                continue;
            //cout << Predict(pImage_roi[i]);
            switch (Predict(pImage_roi[i]))
            {
            case 35:str = "niyangke"; break;
            case 36:str = "caochao"; break;
            default: str = "error"; break;

            }
            Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));//所取的颜色任意值
            rectangle(fream, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), color, 1, 8);//放入缓存
            putText(fream, str, text_lb, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));//添加文字
            

        }
        if (eyes.empty()) {
            flag = 1;
        }
        else
        {
            flag = 0;
          for (int j = 0; j < eyes.size(); j++)
          {
            Point center;
            Scalar eyecolor = colors[j % 8];
            int radius = 2;
            center.x = cvRound(eyes[j].x + eyes[j].width * 0.5);
            center.y = cvRound(eyes[j].y + eyes[j].height * 0.5);
            circle(fream, center, radius, eyecolor, 3, 8, 0);
          }
        }


        delete[]pImage_roi;

        imshow("face", fream);
        //检测到人脸但是没检测到眼睛
        if (str == "niyangke"&&flag==1) {  
            
           declineBrightness();
         
        }
        //检测到人脸和眼睛
        else if (str == "niyangke" && flag == 0) {
           
            gammaramp Gammaramp;
            Gammaramp.SetBrightness(NULL, 128);
        }
        else if (str != "niyangke")
        {
            QTime notimeflag = ui.timeEdit->time();
            gammaramp Grammaramp;
            Sleep(1000);
            QTime currentTime = ui.timeEdit->time();
            if (currentTime.second() - notimeflag.second() > 5) {     //没有检测到人脸超过5秒则调暗亮度
                //declineBrightness();
                Grammaramp.SetBrightness(NULL, 80);
            }
            
        }
       
        int c = cv::waitKey(200);
        if (c == 27)
        {
            break;
        }
    }
}


int QtSightness::Predict(Mat src_image)
{
    Mat face_test;
    int predict = 0;
    if (src_image.rows >= 120)
    {
        cv::resize(src_image, face_test, Size(92, 112));
    }
    //判断是否正确检测roi
    if (!face_test.empty())
    {
        //测试灰度图
        predict = model->predict(face_test);

    }
    cout << predict << endl;
    return predict;
}

void QtSightness::colorTemperature()//n越大色调越暖
{
   Mat input;
   VideoCapture cap2(0);
   while (1)
   {
       cap2 >> input;
       if (input.empty()) {
           //break;
       }
       else
       {
          int n = 50;
          Mat result = input.clone();
          int row = input.rows;
          int col = input.cols;
          int level = n / 2;

       for (int i = 0; i < row; ++i)
       {
           uchar* a = input.ptr<uchar>(i);//返回指向图像每一行的指针
           uchar* r = result.ptr<uchar>(i);
           for (int j = 0; j < col; ++j)
           {
               int R, G, B;
               R = a[j * 3 + 2];
               R = R + level;
               if (R > 255) {
                   r[j * 3 + 2] = 255;//读入rgb实际是bgr
               }
               else if (R < 0) {
                   r[j * 3 + 2] = 0;
               }
               else {
                   r[j * 3 + 2] = R;
               }
               G = a[j * 3 + 1];
               G = G + level;
               if (G > 255) {
                   r[j * 3 + 1] = 255;
               }
               else if (G < 0) {
                   r[j * 3 + 1] = 0;
               }
               else
               {
                   r[j * 3 + 1] = G;
               }
               B = a[j * 3];
               B = B - level;
               if (B > 255) {
                   r[j * 3] = 255;
               }
               else if (B < 0) {
                   r[j * 3] = 0;
               }
               else {
                   r[j * 3] = B;
               }

           }

       }
         namedWindow("input",0);
         namedWindow("result", 0);
         imshow("input", input);
         imshow("result", result);
         int key = waitKey(1);
         if (key == 27)
         {
             break;

         }
       }
    }
   cap2.release();
   destroyAllWindows();
  
}
void QtSightness::kttoRGB()
{
    int temperature = 8500;
    float ktRGB_Temperature = temperature / 100;

    Mat ktoRGB_red, ktoRGB_green, ktoRGB_blue;
    Mat input, result, output;
    VideoCapture cap(0);
    cap >> input;

    vector<Mat>mv;
    split(input, mv);
    ktoRGB_red = mv[2];
    ktoRGB_blue = mv[0];
    ktoRGB_green = mv[1];


    if (ktRGB_Temperature <= 66)
    {
        ktoRGB_red = 255;
    }
    else {
        ktoRGB_red = 329.698727446 * pow(ktRGB_Temperature - 60, -0.1332047592);

        for (int i = 0; i < ktoRGB_red.rows; i++) 
        {
            for (int j = 0; j < ktoRGB_red.cols; j++) 
            {
                if (ktoRGB_red.at<uchar>(i, j) < 0) {
                    ktoRGB_red.at<uchar>(i, j) = 0;
                }
                else if (ktoRGB_red.at<uchar>(i, j) > 255)
                {
                    ktoRGB_red.at<uchar>(i, j) = 255;
                }

            }
        }
    }
    
  
        //green
      if (ktRGB_Temperature <= 66)
      {
        ktoRGB_green = 99.4708025861 * log(ktRGB_Temperature) - 161.1195681661;
      }
      else
      {
        ktoRGB_green = 288.1221695283 * pow(ktRGB_Temperature - 60, -0.0755148492);

      }
           for (int i = 0; i < ktoRGB_green.rows; i++) 
           {
                for (int j = 0; j < ktoRGB_green.rows; j++) 
                {
                    if (ktoRGB_green.at<uchar>(i, j) < 0) 
                    {
                        ktoRGB_green.at<uchar>(i, j) = 0;
                    }
                    else if (ktoRGB_green.at<uchar>(i, j) > 255)
                    {
                        ktoRGB_green.at<uchar>(i, j) = 255;
                    }
                }
           }
      
        if (ktRGB_Temperature >= 66)
        {
            ktoRGB_blue = 255;
        }
        else
        {
            if (ktRGB_Temperature <= 19)
            {
                ktoRGB_blue = 0;
            }
            else
            {
                ktoRGB_blue = 138.5177312231 * log(ktRGB_Temperature - 10) - 305.0447927307;
                for (int i = 0; i < ktoRGB_blue.rows; i++) {
                   for (int j = 0; j < ktoRGB_blue.cols; j++) {
                      if (ktoRGB_blue.at<uchar>(i, j) < 0) {
                       ktoRGB_blue.at<uchar>(i,j) = 0;
                       }
                       else if (ktoRGB_blue.at<uchar>(i,j) > 255) {
                       ktoRGB_blue.at<uchar>(i, j) = 255;
                       }
                   }
                }
            }
        }
        
    merge(mv, result);
    imshow("declineT", result);

}