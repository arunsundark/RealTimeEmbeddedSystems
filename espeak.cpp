
#include <string.h>
#include <malloc.h>
#include <espeak/speak_lib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <iostream>
//using namespace cv;
using namespace std;

espeak_POSITION_TYPE position_type;
espeak_AUDIO_OUTPUT output;
char *path=NULL;
int Buflength = 500, Options=0;
void* user_data;
t_espeak_callback *SynthCallback;
espeak_PARAMETER Parm;

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
	
  cout<<"ACET="<<(rtclk_dt.tv_nsec/1000000)<<"ms"<<endl;
  cout<<"WCET="<<(rtclk_dt.tv_nsec/1000000)+145<<"ms"<<endl;	  
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




char Voice[] = {"lt+klatt2"};
char text[30] = {" banana"};
unsigned int Size,position=0, end_position=0, flags=espeakCHARS_AUTO, *unique_identifier;

int main(int argc, char* argv[] ) 
{	clock_gettime(CLOCK_REALTIME, &rtclk_start_time);
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
    Size = strlen(text)+1;    
    printf("Saying  '%s'",text);
    espeak_Synth( text, Size, position, position_type, end_position, flags,
    unique_identifier, user_data );
    espeak_Synchronize( );
clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);
delta_t(&rtclk_stop_time, &rtclk_start_time ,&rtclk_dt);
cout<<"****************Timing analysis for TextToSpeech*************"<<endl; 
end_delay_test();   
	printf("\n:Done\n"); 
    return 0;
}
