#include "VideoRecover.hpp"

Mat mask;
bool draw_line=false;
Mat temp;

void mouseCallBack(int event,int x,int y,int flag,void *param)
{
	if(!temp.data)
	{
		(*(Mat*) param).copyTo(temp);
	}
	switch(event)
	{
	case CV_EVENT_LBUTTONDOWN:draw_line=true;printf("%d\t%d\n",x,y);break;
	case CV_EVENT_LBUTTONUP:draw_line=false;break;
	case CV_EVENT_MOUSEMOVE:
		if(draw_line==true)
		{
			cv::circle(mask,cv::Point(x,y),5,cv::Scalar(255),-1);
			if(temp.channels()==1)
			{
				cv::circle(temp,cv::Point(x,y),1,cv::Scalar(255),-1);
			}
			else if(temp.channels()==3)
			{
				cv::circle(temp,cv::Point(x,y),5,cv::Scalar(255,255,255),-1);
				
			}
		}
		break;
	}
	imshow("设定修补区域",temp);
	imshow("1",mask);
}

void VideoRecover::process(cv:: Mat &input, cv:: Mat &output)
{
	if(!mask.data)    //还未设置要修补的区域
	{
        mask.create(input.rows,input.cols,CV_8U);
		Mat_<uchar>::iterator it=mask.begin<uchar>();
		Mat_<uchar>::iterator itend=mask.end<uchar>();
		while(it!=itend)
		{
			*it=0;
			it++;
		}

		imshow("设定修补区域",input);
		setMouseCallback("设定修补区域",mouseCallBack,&input);
		while(waitKey(0)!='y');
		destroyWindow("设定修补区域");
		destroyWindow("1");
	}

	inpaint(input,mask,output,inpaintRadius,INPAINT_NS);
}
