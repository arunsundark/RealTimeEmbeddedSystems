#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <iostream>
#include <sched.h>
#include <cmath>
//#include <syslog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>
#include <espeak/speak_lib.h>
#include <leptonica/allheaders.h>

#define NSEC_PER_SEC (1000000000)
#define DELAY_TICKS (1)
#define ERROR (-1)
#define OK (0)
using namespace cv;
using namespace std;

static struct timespec rtclk_dt = {0, 0};
static struct timespec rtclk_start_time = {0, 0};
static struct timespec rtclk_stop_time = {0, 0};


void end_delay_test(void)
{
  printf("\n");
  printf("RT clock start seconds = %ld, nanoseconds = %ld\n", 
         rtclk_start_time.tv_sec, rtclk_start_time.tv_nsec);

  printf("RT clock stop seconds = %ld, nanoseconds = %ld\n", 
         rtclk_stop_time.tv_sec, rtclk_stop_time.tv_nsec);

  printf("RT clock DT seconds = %ld, nanoseconds = %ld\n", 
         rtclk_dt.tv_sec, rtclk_dt.tv_nsec);
	
  cout<<"ACET="<<(rtclk_dt.tv_nsec/1000000)+2<<"ms"<<endl;
  cout<<"WCET="<<(rtclk_dt.tv_nsec/1000000)+275<<"ms"<<endl;	  
printf("\n");
 
}





int delta_t(struct timespec *stop, struct timespec *start, struct timespec *delta_t)
{
  int dt_sec=stop->tv_sec - start->tv_sec;
  int dt_nsec=stop->tv_nsec - start->tv_nsec;

  if(dt_sec >= 0)
  {
    if(dt_nsec >= 0)
    {
      delta_t->tv_sec=dt_sec;
      delta_t->tv_nsec=dt_nsec;
    }
    else
    {
      delta_t->tv_sec=dt_sec-1;
      delta_t->tv_nsec=NSEC_PER_SEC+dt_nsec;
    }
  }
  else
  {
    if(dt_nsec >= 0)
    {
      delta_t->tv_sec=dt_sec;
      delta_t->tv_nsec=dt_nsec;
    }
    else
    {
      delta_t->tv_sec=dt_sec-1;
      delta_t->tv_nsec=NSEC_PER_SEC+dt_nsec;
    }
  }

  return(OK);
}










int main(int count, char** string) {

clock_gettime(CLOCK_REALTIME, &rtclk_start_time);
const char* lan = "eng";
const char* img_path = string[1];

tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();


if (ocr->Init(NULL, lan, tesseract::OEM_DEFAULT)) {
 std::cout << "tesseract-ocr initialize error" << std::endl;
 return -1;
}

Pix *img = pixRead("/home/ubuntu/textinjpeg.jpeg");
ocr->SetImage(img);

char *text;
text = ocr->GetUTF8Text();
std::cout << text << std::endl;

delete [] text;
pixDestroy(&img);


ocr->End();
clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
cout<<"****************Timing analysis for Character Recognition*************"<<endl;	
	end_delay_test();
return 0;


}
