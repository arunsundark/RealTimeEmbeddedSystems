#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

#define HRES 640
#define VRES 480


#define NSEC_PER_SEC (1000000000)
#define DELAY_TICKS (1)
#define ERROR (-1)
#define OK (0)

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

void histogramCalculate(Mat src)
{

	Mat hsv_base;
  	Mat src_test1, hsv_test1;
    	//Mat src_test2, hsv_test2;

//do imread here 
src_test1 = imread("banana.jpeg", CV_LOAD_IMAGE_COLOR);
if(!src_test1.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        
//return -1;
    }

 vector<Mat> bgr_planes;
  split( src, bgr_planes );

vector<Mat> compbgr_planes;
  split( src_test1, compbgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;
	Mat compb_hist, compg_hist, compr_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
	
  
calcHist( &compbgr_planes[0], 1, 0, Mat(), compb_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &compbgr_planes[1], 1, 0, Mat(), compg_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &compbgr_planes[2], 1, 0, Mat(), compr_hist, 1, &histSize, &histRange, uniform, accumulate );
	

	int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

normalize(compb_hist, compb_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(compg_hist, compg_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(compr_hist, compr_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

 
int compare_method[3] = {0,0,0};
        double base_test1 = compareHist( b_hist,compb_hist, compare_method[0] );
	double base_test2 = compareHist( g_hist,compg_hist, compare_method[0] );
	double base_test3 = compareHist( r_hist,compr_hist, compare_method[0] );
	
       // double base_test2 = compareHist( hist_base, hist_test2, compare_method );

printf( " Base-Test(1) :%f\n",(base_test1 + base_test2 + base_test3)/3);
    

    printf( "Done \n" );



}



VideoCapture capture;

int main( int argc, char** argv )
{	//cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
	Mat frame;
	Mat rgbImage;
clock_gettime(CLOCK_REALTIME, &rtclk_start_time);
capture=VideoCapture(0);

if (!capture.isOpened()) 
	{
	cerr << "Cannot Open Webcam !!!" << endl; 
	exit(EXIT_FAILURE);
	}

//capture.read(frame);
if (!capture.read(frame)) 	
	{
	cerr << "Unable to read next frame." << endl;
	cerr << "Exiting..." << endl;
	exit(EXIT_FAILURE);
	}


namedWindow("capture", CV_WINDOW_AUTOSIZE);
imshow("CAPTURE",frame); 
waitKey(0);
//clock_gettime(CLOCK_REALTIME, &rtclk_start_time);
histogramCalculate(frame);
clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
end_delay_test();
};
