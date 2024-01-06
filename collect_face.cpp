#include"collect_face.h"
#include<opencv.hpp>
using namespace std;
using namespace cv;
int collect_my_face()
{
	cv::CascadeClassifier cascada;
    //��opencv�ٷ�ѵ���õ�����ʶ��������������Լ��Ĺ���Ŀ¼��
    cascada.load("D:\\QT\\qt(ui)\\mywidgehaarcascade_frontalface_alt2.xml");
    VideoCapture cap(0);
    Mat frame, myFace;
    int pic_num = 1;
    while (1) {
        cap >> frame;
        vector<Rect> faces;//vector�������⵽��faces
        Mat frame_gray;

        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);//ת�ҶȻ�����������

        cascada.detectMultiScale(frame_gray, faces, 1.1, 4, CASCADE_DO_ROUGH_SEARCH, Size(70, 70), Size(1000, 1000));
        printf("��⵽����������%d\n", faces.size());

        //ʶ�𵽵����þ���Ȧ��
        for (int i = 0; i < faces.size(); i++)
        {
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
        }
        //��ֻ��һ������ʱ����ʼ����
        if (faces.size() == 1)
        {
            Mat faceROI = frame_gray(faces[0]);//�ڻҶ�ͼ�н�Ȧ��������������ü���
            //cout << faces[0].x << endl;//������face[0].x
            cv::resize(faceROI, myFace, Size(92, 112));//����Ȥ��sizeΪ92*112
            putText(frame, to_string(pic_num), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0);//�� faces[0].tl()�����Ͻ�����д���
            string filename = format("E:\\thunder\\ORL\\s41\\%d.jpg", pic_num);            
            imwrite(filename, myFace);//���ڵ�ǰĿ¼��
            imshow(filename, myFace);
            waitKey(500);//�ȴ�500us
            destroyWindow(filename);//:����ָ���Ĵ���
            pic_num++;//��ż�1
            if (pic_num == 11)
            {
                return 0;//�����Ϊ11ʱ�˳�ѭ��,һ����10����Ƭ
            }
        }
        int c = waitKey(10);
        if ((char)c == 27) { break; } //10us������esc���˳�ѭ��
        imshow("frame", frame);//��ʾ��Ƶ��
        waitKey(100);//�ȴ�100us
    }
    return 0;
}