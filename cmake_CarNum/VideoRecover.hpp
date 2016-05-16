#ifndef VIDEORECOVER
#define VIDEORECOVER
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "video.h"

using namespace std;
using namespace cv;

extern Mat mask;
extern bool draw_line;
class VideoRecover:public FrameProcessor
{
private:
	double inpaintRadius;
public:
	virtual void process(cv:: Mat &input, cv:: Mat &output);
	VideoRecover()
	{
		inpaintRadius=3;
	}
};
#endif
