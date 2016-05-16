#ifndef FRONTDECTOR_H
#define FRONTDECTOR_H
#include <stdio.h>
int check(const Mat &input,Point &Left,Point &Right);
/*����һ���ı���
��ǰ���������������
*/
class Quadrilateral
{
private:
	//�ֱ�Ϊ��������������
	vector<Point> foreside;
	vector<Point> leftside;
	vector<Point> rightside;
	vector<Point> behind;

	int CarNum;
	bool CarIn;
	bool CarOut;
	string name;
public:
	/*���캯��˳ʱ���ĸ���*/
	Quadrilateral(Point a,Point b,Point c,Point d)
	{

		CarIn = false;
		CarNum = 0;
		foreside.push_back(a);
		foreside.push_back(b);

		rightside.push_back(b);
		rightside.push_back(c);

		behind.push_back(c);
		behind.push_back(d);

		leftside.push_back(d);
		leftside.push_back(a);
	}
	Quadrilateral()
	{
		CarIn = false;
	}

	void create(Point a,Point b,Point c,Point d,char* str)
	{
		name = str;
		CarIn = false;
		CarNum = 0;
		foreside.push_back(a);
		foreside.push_back(b);

		rightside.push_back(b);
		rightside.push_back(c);

		behind.push_back(c);
		behind.push_back(d);

		leftside.push_back(d);
		leftside.push_back(a);
	}
	/*������*/
	void draw(Mat &image)
	{
		string str;
		char c[20];
		if(CarIn==true)
		{
			cv::line(image,foreside[0],foreside[1],Scalar(0,255,0));
			cv::line(image,leftside[0],leftside[1],Scalar(0,255,0));
			cv::line(image,rightside[0],rightside[1],Scalar(0,255,0));
			cv::line(image,behind[0],behind[1],Scalar(0,255,0));
		}
		else
		{
			cv::line(image,foreside[0],foreside[1],Scalar(0,0,255));
			cv::line(image,leftside[0],leftside[1],Scalar(0,0,255));
			cv::line(image,rightside[0],rightside[1],Scalar(0,0,255));
			cv::line(image,behind[0],behind[1],Scalar(0,0,255));
		}
                sprintf(c,"%d",CarNum);
                cv::putText(image,c,Point((foreside[0].x+foreside[1].x)/2,(leftside[0].y+leftside[1].y)/2), FONT_HERSHEY_COMPLEX ,0.7,Scalar(255,255,0));

                cout<<name<<":"<<CarNum<<endl;
	}
	/*���¸����ݽṹ*/
	void updata(const Mat& image)
	{
		if(CarIn == false)
		{
			if(check(image,foreside[0],foreside[1])&&
			   !check(image,leftside[0],leftside[1])&&
			   !check(image,rightside[0],rightside[1])&&
			   !check(image,behind[0],behind[1]))
			{
				CarIn = true;
			}
		}
		else
		{
			if(check(image,behind[0],behind[1]))
			{
				CarIn = false;
				CarNum++;
			}
		}
	}
        int getnum()
        {
            return CarNum;
        }
};

extern Quadrilateral middle_road;
extern Quadrilateral left_road;
extern Quadrilateral right_road;
class BGFGSegmentor:public FrameProcessor
{
private:
	Mat gray;
	Mat background;
	Mat backimage;
	Mat foreground;
	Mat HSV; 
	vector<Mat> v;
	Ptr<BackgroundSubtractor> mog;      //opencv �ṩ�����ֱ�����ģ�ķ���

	double learningRate;
	int thresh;
public:
	BGFGSegmentor():thresh(128),learningRate(0.01)
	{
		mog = cv::createBackgroundSubtractorMOG2();
	}
	void process(cv:: Mat &frame, cv:: Mat &output)
	{
		Mat BG_now;                  //��ǰͼ�񱳾�
		Mat FG_now;                  //ǰ��ͼ��
		Mat temp;
                Mat element5(7,7,CV_8U,Scalar(1));     //������������ͱ�����
		Mat kernel(3,3,CV_32F,Scalar(0));
 //               vector<vector<Point>> contours;
		Point left(180,133);
		Point right(260,133);


		/*��˹���ģ��*/
 		mog->apply(frame,foreground,learningRate);               //���±���
		
		Mat ground(frame.rows,frame.cols,CV_8U);
		mog->getBackgroundImage(ground);
		//imshow("1",ground);

		threshold(foreground,BG_now,thresh,255,THRESH_BINARY_INV);            //������ֵ    ����Ϊ255
		threshold(foreground,FG_now,thresh,255,THRESH_BINARY);                //ǰ����ֵ    ǰ��Ϊ255

                morphologyEx(FG_now,FG_now,MORPH_OPEN,element5);
                morphologyEx(FG_now,FG_now,MORPH_CLOSE,element5);              //
		
		/******��ȡǰ��****/
		frame.copyTo(temp,FG_now);
//		findContours(FG_now,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
//		drawContours(temp,contours,-1,Scalar(255,255,0),2);
		imshow("front",FG_now);
		
		frame.copyTo(temp);

		middle_road.updata(FG_now);
		middle_road.draw(temp);

		left_road.updata(FG_now);
		left_road.draw(temp);

		right_road.updata(FG_now);
		right_road.draw(temp);
		output=temp;
	}
};

/*���ĳ�������Ƿ���ǰ��
���룺input ͼ��
left������
right���Ҳ��
��������ɨ��
*/
int check(const Mat& input,Point &Left,Point &Right)
{
	float k=0;
	long length=0;
	long front=0;        //��¼��һ������ǰ���������
	Point right(Right);
	Point temp(Left);
	Mat_<uchar> image=(Mat_<uchar> &)input;

	/*ʹ��opencv���߶ε�����*/
	LineIterator line_it(input,Left,Right);
	int i,count = line_it.count;
	for(i=0;i<count;i++,line_it++)
	{
		uchar *ptr= *line_it;

		if(ptr[0]>100)
			front++;
	}
	k=(float)front/(float)count;
	if(k>0.1)
		return 1;
	else
		return 0;
}

#endif
