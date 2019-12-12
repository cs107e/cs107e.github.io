#include "mcp3008.h"
#include "xcir.h"
#include "audio.h"
#include "filters.h"
#include "printf.h"
#include "malloc.h"
#include "timer.h"
/*
 *  Implements the simplest possible low pass filter, as described by CCRMA:
 *  https://ccrma.stanford.edu/~jos/fp/Definition_Simplest_Low_Pass.html
 *  Essentially, an average across two time steps in real time.
 *  @author aangelopoulos
 *  @date 2/6/17
*/
#define SP0 0
#define avg(a,b) (a+b)/2
#define PASS_CAPACITY 300
#define DELAY_CAPACITY 1500
#define WET_SIGNAL 0.1
#define TREMLENGTH 10000
#define MAX_THRESHOLD 520
#define MIN_THRESHOLD 505
#define ADJUST_VOLUME(signal,scaling) (scaling * (signal - CYCLES/2)) + CYCLES/2;
#define REVERB_TEST_LENGTH 5000

static int V0;
static int V1;
static xcir_t * cir;
//static int delay_capacity;
//static xcir_t * history;
static unsigned * tremArr;
static unsigned tremLength;
static unsigned tremIndex;
static unsigned delayLength;
static xcir_t * delayArr;
static unsigned * postArr;
static unsigned postLength;
static unsigned reverb_delay;

void filters_init() {
  V0 = V1 = 0;
  cir = xcir_new(PASS_CAPACITY);
  filters_phaserinit(TREMLENGTH);
  filters_delayinit(DELAY_CAPACITY);
}

void filters_phaserinit(unsigned length) {
  tremIndex = 0;
  tremLength = length;
  tremArr = malloc(sizeof(unsigned)*(length*2-1));
  int i;
  for(i = 0; i < length; i++) tremArr[i] = i;
  for(int j = length; j > 0; j--) tremArr[i++] = j;
}

void filters_delayinit(unsigned length) {
  delayLength = length;
  delayArr = xcir_new(delayLength);
}

void filters_printtremarr() {
  for(int i = 0; i < tremLength*2; i++) printf("%d ",tremArr[i]);
}
/*
 *  Implements a simple numerical low pass filter.
 *  Only stores two values based on the current voltage and outputs the result.
 *  @author aangelopoulos
 *  @date 2/6/17
*/
unsigned filters_nofilter() {
  return mcp3008_read(SP0);
}

/*
 *  Implements a simple numerical low pass filter.
 *  Only stores two values based on the current voltage and outputs the result.
 *  @author aangelopoulos
 *  @date 2/6/17
*/
unsigned filters_lowpass() {
  V0 = V1;
  V1 = mcp3008_read(SP0);
  return avg(V0,V1);
}

/*
 *  Implements a simple numerical high pass filter.
 *  Just subtracts a lowpass filter from a high pass filter.
 *  @author aangelopoulos
 *  @date 10/6/17
*/
unsigned filters_distortion() {
  V0 = V1;
  V1 = mcp3008_read(SP0);
  return V1 - avg(V0,V1);
}

/*
 *  Implements a slightly more complex numerical low pass filter.
 *  Averages a circular buffer of arbitrary points.
 *
 *  @author aangelopoulos
 *  @date 10/6/17
*/
unsigned filters_cirlowpass() {
  if(xcir_full(cir)) xcir_dequeue(cir);
  xcir_enqueue(cir,mcp3008_read(SP0));
  return xcir_avg(cir);
}

unsigned filters_phaser() {
  V0 = V1;
  V1 = mcp3008_read(SP0);
  int read = avg(V0,V1);
  if(tremIndex == tremLength*2) tremIndex = 0;
  if(read < MAX_THRESHOLD && read > MIN_THRESHOLD) return CYCLES/2;
  V1 = (tremArr[tremIndex++] * (read - CYCLES/2))/tremLength + CYCLES/2;
  return V1;
}

unsigned filters_delay() {
    V0 = V1;
    V1 = mcp3008_read(SP0);
    V1 = avg(V0,V1);
    int add = 0;
    if(xcir_full(delayArr)) add = xcir_dequeue(delayArr);
    V1 = avg(V1,add);
    xcir_enqueue(delayArr,V1);
    return V1;
}

static void filters_collect() {
  printf("Collecting %d samples\n",postLength);
  for(int i = 0; i < postLength; i++) postArr[i] = mcp3008_read(SP0);
}

static unsigned totalDecay(unsigned index) {
  unsigned reverb = 0;
  for(int i = reverb_delay; i > 0; i--) {
    reverb += ADJUST_VOLUME(postArr[index-i+reverb_delay-1],i);
    printf("Reverb i:%d\n",i);

  }
  //reverb = ADJUST_VOLUME(reverb,1/reverb_delay);
  printf("Reverb q:%d\n",reverb);
  return reverb;
}

unsigned * filters_reverb(unsigned length, unsigned delay) {
  postLength = length;
  reverb_delay = delay;
  postArr = malloc(sizeof(unsigned) * length);
  filters_collect();
  unsigned * toPlay = malloc(sizeof(unsigned) * (postLength + reverb_delay)); //For tail.
  printf("Processing %d samples...\n", postLength);
  for(int i = 0; i < postLength; i++) {
    if(i < reverb_delay) toPlay[i] = postArr[i];
    else toPlay[i] = postArr[i] + totalDecay(i);
  }
  return toPlay;
}

unsigned * filters_reverb_wrapper(unsigned samples) {
  return filters_reverb(samples,REVERB_TEST_LENGTH);
}

unsigned * filters_reverse(unsigned samples) {
  filters_collect();
  unsigned * toPlay = malloc(sizeof(unsigned) * samples);
  for(int i = 0; i < samples; i++) toPlay[i] = postArr[samples-1-i];
  return toPlay;
}
  /*
  unsigned totalDecay =  0;
  int i;
  if(index < reverb_delay*2) {
    for(i = 0; i < index - reverb_delay; i++)
      totalDecay += reverbArr[index-reverb_delay-i] * (reverb_delay-i)/(reverb_delay);
  } else {
    for(i = 0; i < reverb_delay; i++)
      totalDecay += reverbArr[index-reverb_delay-i] * (reverb_delay-i)/(reverb_delay);
  }
  return totalDecay/i;
  */

unsigned * filters_test(unsigned (*function)(void), int samples) {
  unsigned * tst = malloc(sizeof(int)*samples);
  for(int i = 0; i < samples; i++) {
    tst[i] = function();
  }
  return tst;
}
