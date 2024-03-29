#include"training_model.h"
#include <opencv.hpp>
#include<iostream>
using namespace std;

 Mat train_face_model::norm_0_255(InputArray _src)
{
    Mat src = _src.getMat();
    // 创建和返回一个归一化后的图像矩阵:
    Mat dst;
    switch (src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

 void train_face_model::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels)
 {
     std::ifstream file(filename.c_str(), ifstream::in);//c_str()函数可用可不用，无需返回一个标准C类型的字符串
     if (!file)
     {
         string error_message = "No valid input file was given, please check the given filename.";
         CV_Error(Error::StsBadArg, error_message);
     }
     string line, path, classlabel;
     while (getline(file, line)) //从文本文件中读取一行字符，未指定限定符默认限定符为“/n”
     {
         stringstream liness(line);//这里采用stringstream主要作用是做字符串的分割
         getline(liness, path, ';');//读入图片文件路径以分好作为限定符
         getline(liness, classlabel);//读入图片标签，默认限定符
         if (!path.empty() && !classlabel.empty()) //如果读取成功，则将图片和对应标签压入对应容器中
         {
             images.push_back(imread(path, 0));
             labels.push_back(atoi(classlabel.c_str()));
         }
     }
 }

 int train_face_model::training()
 {
     //读取你的CSV文件路径.
   //string fn_csv = string(argv[1]);
     string fn_csv = "E:\\thunder\\ORL\\at.txt";

     // 2个容器来存放图像数据和对应的标签
     vector<Mat> images;
     vector<int> labels;
     // 读取数据. 如果文件不合法就会出错
     // 输入的文件名已经有了.
     try
     {
         read_csv(fn_csv, images, labels); //从csv文件中批量读取训练数据
     }
     catch (cv::Exception& e)
     {
         cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
         // 文件有问题，我们啥也做不了了，退出了
         exit(1);
     }
     // 如果没有读取到足够图片，也退出.
     if (images.size() <= 1) {
         string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
         CV_Error(Error::StsBadArg, error_message);
     }

     for (int i = 0; i < images.size(); i++)
     {
         //cout<<images.size();
         if (images[i].size() != Size(92, 112))
         {
             cout << i << endl;
             cout << images[i].size() << endl;
         }

     }

     // 下面的几行代码仅仅是从你的数据集中移除最后一张图片，作为测试图片
     Mat testSample = images[images.size() - 1];
     int testLabel = labels[labels.size() - 1];
     images.pop_back();//删除最后一张照片，此照片作为测试图片
     labels.pop_back();//删除最有一张照片的labels

     //创建一个PCA人脸分类器，暂时命名为model吧，创建完成后
     //调用其中的成员函数train()来完成分类器的训练
     Ptr<face::BasicFaceRecognizer> model = face::EigenFaceRecognizer::create();
     model->train(images, labels);
     model->save("D:\\QT\\qt(ui)\\MyFacePCAModel.xml");//保存路径可自己设置，但注意用“\\”

     Ptr<face::BasicFaceRecognizer> model1 = face::FisherFaceRecognizer::create();
     model1->train(images, labels);
     model1->save("D:\\QT\\qt(ui)\\MyFaceFisherModel.xml");

     Ptr<face::LBPHFaceRecognizer> model2 = face::LBPHFaceRecognizer::create();
     model2->train(images, labels);
     model2->save("D:\\QT\\qt(ui)\\MyFaceLBPHModel.xml");

     // 下面对测试图像进行预测，predictedLabel是预测标签结果
     //注意predict()入口参数必须为单通道灰度图像，如果图像类型不符，需要先进行转换
     //predict()函数返回一个整形变量作为识别标签
     int predictedLabel = model->predict(testSample);//加载分类器
     int predictedLabel1 = model1->predict(testSample);
     int predictedLabel2 = model2->predict(testSample);

     // 还有一种调用方式，可以获取结果同时得到阈值:
     // int predictedLabel = -1;
     // double confidence = 0.0;
     //  model->predict(testSample, predictedLabel, confidence);

     string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
     string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
     string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
     cout << result_message << endl;
     cout << result_message1 << endl;
     cout << result_message2 << endl;

     getchar();
     //waitKey(0);
     return 0;
 }