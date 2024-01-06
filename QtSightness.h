#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSightness.h"
#include<opencv2/imgproc/types_c.h>
#include"gammaramp.h"
#include<opencv2/face.hpp>

#include<vector>
#include<iostream>
#include<opencv2/opencv.hpp>
#include"qcolor.h"
using namespace cv;
using namespace std;
class QtSightness : public QMainWindow
{
    Q_OBJECT

public:
    QtSightness(QWidget *parent = Q_NULLPTR);
    void raseBrightness();
    void declineBrightness();
    void deteface();
    void colorTemperature();
    int Predict(Mat src_image);
    void kttoRGB();
private:
    Ui::QtSightnessClass ui;
    void Temperature_to_RGB();
};
