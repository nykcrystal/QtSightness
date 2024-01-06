#pragma once
#include<iostream>
#include<face.hpp>
#include<math.h>
#include<fstream>
#include<sstream>

using namespace std;
using namespace cv;

class train_face_model
{
public:
	train_face_model();
	~train_face_model();

private:
	static Mat norm_0_255(InputArray _src);
	static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels);
	int training();
};

train_face_model::train_face_model()
{
}

train_face_model::~train_face_model()
{
}
