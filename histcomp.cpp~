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

#include <leptonica/allheaders.h>
#include <espeak/speak_lib.h>
using namespace cv;
using namespace std;

espeak_POSITION_TYPE position_type;
espeak_AUDIO_OUTPUT output;
char *path=NULL;
int Buflength = 500, Options=0;
void* user_data;
t_espeak_callback *SynthCallback;
espeak_PARAMETER Parm;

char Voice[] = {"lt+klatt2"};
char text[30] = {" apple"};
unsigned int size_esp,position=0, end_position=0, flags=espeakCHARS_AUTO, *unique_identifier;


// CV preprocessing statements
#define HRES 1280	
#define VRES 1080

//pthreads preprocessing statements, declarations and attributes
#define NUM_THREADS (5)

pthread_t threads[NUM_THREADS];
pthread_attr_t rt_sched_attr[NUM_THREADS];
pthread_attr_t main_attr;
int rt_max_prio, rt_min_prio;
struct sched_param rt_param[NUM_THREADS];
struct sched_param main_param;
pid_t mainpid;

/*void *ImageCapture(void*);
void *histogramCalculate(void*);
void *ObjectRecognition(void*);
void *TextToSpeech(void*);
void *CharacterRecognition(void*);
*/

//profiler preprocessors
#define NSEC_PER_SEC (1000000000)
#define DELAY_TICKS (1)
#define ERROR (-1)
#define OK (0)
//profiler gloabal variable
static struct timespec rtclk_dt = {0, 0};
static struct timespec rtclk_start_time = {0, 0};
static struct timespec rtclk_stop_time = {0, 0};

//Global shared data
VideoCapture capture;
Mat CapturedFrame;
	
long int acet;
long int wcet;
int si; 



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
  cout<<"ACET=="<<acet/1000000<<" ms"<<endl;
  cout<<"wcet=="<<wcet/1000000<<" ms"<<endl;
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
	if(si==1)
	{
	acet=delta_t->tv_nsec;
	wcet=delta_t->tv_nsec;
	}
	else
	{	
	acet=(acet + delta_t->tv_nsec)/2;
		if(wcet < delta_t->tv_nsec)
		{
		wcet=delta_t->tv_nsec;
		}
		else
		{
		//do nothing
		}	
	}
  return(OK);
}

void TextToSpeech()
{
clock_gettime(CLOCK_REALTIME, &rtclk_start_time);
    output = AUDIO_OUTPUT_PLAYBACK;
    int I, Run = 1, L;    
    espeak_Initialize(output, Buflength, path, Options ); 
    //espeak_SetVoiceByName(Voice);
    const char *langNativeString = "lt"; //Default to US English
    espeak_VOICE voice;
	memset(&voice, 0, sizeof(espeak_VOICE)); // Zero out the voice first
	voice.languages = langNativeString;
	voice.name = "klatt";
	voice.variant = 2;
	voice.gender = 1;
	espeak_SetVoiceByProperties(&voice);
    size_esp = strlen(text)+1;    
    printf("Saying  '%s'",text);
    espeak_Synth( text, size_esp, position, position_type, end_position, flags,
    unique_identifier, user_data );
    espeak_Synchronize( );
clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
}


void CharacterRecognition()
{
const char* lan = "eng";
tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
if (ocr->Init(NULL, lan, tesseract::OEM_DEFAULT)) {
 std::cout << "tesseract-ocr initialize error" << std::endl;
 return ;
}

Pix *img = pixRead("/home/ubuntu/textinjpeg.jpeg");
ocr->SetImage(img);

char *text;
text = ocr->GetUTF8Text();
std::cout << text << std::endl;

delete [] text;
pixDestroy(&img);
ocr->End();

}




void histogramCalculate(Mat src_base)
{

	Mat hsv_base;
  	Mat src_test1, hsv_test1;
    	//Mat src_test2, hsv_test2;

	//do imread here 
	src_test1 = imread("banana.jpeg", CV_LOAD_IMAGE_COLOR);
	
	// Check for invalid input
	if(!src_test1.data )                              
    	{
        cout <<  "Could not open or find the image" << std::endl ;
        return ;
	}

	
	/// Convert to HSV
    	cvtColor( src_base, hsv_base, COLOR_BGR2HSV );
    	cvtColor( src_test1, hsv_test1, COLOR_BGR2HSV );
    	//cvtColor( src_test2, hsv_test2, COLOR_BGR2HSV );

	// Using 50 bins for hue and 60 for saturation
	int h_bins = 50; int s_bins = 60;
        int histSize[] = { h_bins, s_bins };

    	// hue varies from 0 to 179, saturation from 0 to 255
    	float h_ranges[] = { 0, 180 };
    	float s_ranges[] = { 0, 256 };

    	const float* ranges[] = { h_ranges, s_ranges };

    	// Use the o-th and 1-st channels
    	int channels[] = { 0, 1 };

    	MatND hist_base;
    	MatND hist_test1;
    	//MatND hist_test2;

    	calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
    	normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

    	calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
    	normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );


	/// Apply the histogram comparison methods
   	// for( int i = 0; i < 2; i++ )
   	// {
        int compare_method = 0;
        double base_test1 = compareHist( hist_base, hist_test1, compare_method );
	base_test1 = compareHist( hist_base, hist_test1, compare_method );
	base_test1 = compareHist( hist_base, hist_test1, compare_method );

       // double base_test2 = compareHist( hist_base, hist_test2, compare_method );

       // printf( " Base-Test(1) :%f\n",base_test1);
    //	printf( "Done \n" );
}




int main( int argc, char** argv )
{	
	//cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
	Mat rgbImage;
	int si=0;
	
	
	
	capture=VideoCapture(0);

	if (!capture.isOpened()) 
	{
	cerr << "Cannot Open Webcam !!!" << endl; 
	exit(EXIT_FAILURE);
	}
	for(si=0;si<20;si++) 
	{	//capture.read(frame);
	clock_gettime(CLOCK_REALTIME, &rtclk_start_time);	
	if (!capture.read(CapturedFrame)) 	
	{
	cerr << "Unable to read next frame." << endl;
	cerr << "Exiting..." << endl;
	exit(EXIT_FAILURE);
	}
	clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
	delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
	}	
	cout<<"****************Timing analysis for IMAGE CAPTURE***************"<<endl; 	
	end_delay_test();
	acet=0;
	wcet=0;
	//namedWindow("capture", CV_WINDOW_AUTOSIZE);
	//imshow("CAPTURE",CapturedFrame); 
	//waitKey(0);
	//clock_gettime(CLOCK_REALTIME, &rtclk_start_time);

	// the frame parameter is made into global data , so remember to change parameter
	for(si=0;si<20;si++) {	
	clock_gettime(CLOCK_REALTIME, &rtclk_start_time);	
	histogramCalculate(CapturedFrame);
	clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
	delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
	}
	cout<<"****************Timing analysis for object detection*************"<<endl;	
	end_delay_test();
	
	acet=0;
	wcet=0;
		
/*	//for(si=0;si<20;si++) {	
	clock_gettime(CLOCK_REALTIME, &rtclk_start_time);	
	CharacterRecognition();
	clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
	delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
	//}	
	cout<<"****************Timing analysis for Character Recognition*************"<<endl;	
	end_delay_test();
	
	acet=0;
	wcet=0;
	
	for(si=0;si<20;si++) {	
	clock_gettime(CLOCK_REALTIME, &rtclk_start_time);	
	TextToSpeech();
	clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
	delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
	}	
	cout<<"****************Timing analysis for TextToSpeech*************"<<endl;	
	end_delay_test();

*/
};
