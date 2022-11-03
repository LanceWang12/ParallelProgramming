#include <thread>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "CycleTimer.h"

// divide the mandelbrot image in n_task part by row
// 300 is better than 200
#define n_task 300

using namespace std;

// -------------- Step1 --------------
// Modify the starter code to parallelize the Mandelbrot generation using two processors. 
// Specifically, compute the top half of the image in thread 0, and the bottom half of the image in thread 1. 
// This type of problem decomposition is referred to as spatial decomposition 
// since different spatial regions of the image are computed by different processors.

// [start, end)
typedef struct{
  int start;
  int end;
} work_interval;

static inline int mandel(float c_re, float c_im, int count)
{
  float z_re = c_re, z_im = c_im;
  int i;
  for (i = 0; i < count; ++i)
  {

    if (z_re * z_re + z_im * z_im > 4.f)
      break;

    float new_re = z_re * z_re - z_im * z_im;
    float new_im = 2.f * z_re * z_im;
    z_re = c_re + new_re;
    z_im = c_im + new_im;
  }

  return i;
}

// compute mandelbrot by different interval
void mandelbrot_divide_by_task(
    float x0, float y0, float x1, float y1,
    int width, int height,
    vector<work_interval> task_lst,
    int maxIterations,
    int output[])
{
  float dx = (x1 - x0) / width;
  float dy = (y1 - y0) / height;
  int startRow = 0, endRow = 0;
  for (int work_idx = 0; work_idx < (int)task_lst.size(); work_idx++){
    startRow = task_lst[work_idx].start;
    endRow = task_lst[work_idx].end;
    for (int j = startRow; j < endRow; j++){
      for (int i = 0; i < width; ++i){
        float x = x0 + i * dx;
        float y = y0 + j * dy;

        int index = (j * width + i);
        output[index] = mandel(x, y, maxIterations);
      }
    }
  }
}

// Args for worker
typedef struct
{
    float x0, x1;
    float y0, y1;
    unsigned int width;
    unsigned int height;
    int startRow, endRow;
    int maxIterations;
    int *output;
    int threadId;
    int numThreads;
    // record different work_interval[start, end) in the task_lst
    vector<work_interval> task_lst;
    double *time; // record execution time
} WorkerArgs;

//
// workerThreadStart --
//
// Thread entrypoint.
void workerThreadStart(WorkerArgs *const args)
{

    // TODO FOR PP STUDENTS: Implement the body of the worker
    // thread here. Each thread could make a call to mandelbrotSerial()
    // to compute a part of the output image. For example, in a
    // program that uses two threads, thread 0 could compute the top
    // half of the image and thread 1 could compute the bottom half.
    // Of course, you can copy mandelbrotSerial() to this file and 
    // modify it to pursue a better performance.

    // printf("Hello world from thread %d\n", args->threadId);
    float x0 = args -> x0, y0 = args -> y0;
    float x1 = args -> x1, y1 = args -> y1;
    int width = args -> width, height = args -> height;
    vector<work_interval> task_lst = args -> task_lst;
    int maxIterations = args -> maxIterations;
    int *output = args -> output;
    // double startTime = CycleTimer::currentSeconds();
    mandelbrot_divide_by_task(x0, y0, x1, y1, width, height, task_lst, maxIterations, output);
    // double endTime = CycleTimer::currentSeconds();
    // double duration = endTime - startTime;
    // *(args -> time) = duration;
}

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Threads of execution are created by spawning std::threads.
void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    static constexpr int MAX_THREADS = 32;

    if (numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

    // the height of image must be divisible by n_task
    assert(height % n_task == 0);
    
    // Creates thread objects that do not yet represent a thread.
    std::thread workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];
    double *duration = new double[numThreads];

    // Set params which is same in every thread
    for (int i = 0; i < numThreads; i++)
    {
        // TODO FOR PP STUDENTS: You may or may not wish to modify
        // the per-thread arguments here.  The code below copies the
        // same arguments for each thread
        args[i].x0 = x0;
        args[i].x1 = x1;
        args[i].y0 = y0;
        args[i].y1 = y1;
        args[i].width = width;
        args[i].height = height;
        args[i].maxIterations = maxIterations;
        args[i].numThreads = numThreads;
        args[i].output = output;
        args[i].threadId = i;
        args[i].time = &duration[i];
    }

    
    int interval = height / n_task; // the interval of an image
    int idx = 0; // the index for each thread
    // set tasks for every thread
    for(int i = 0; i < height; i += interval){
      work_interval task = {.start = i, .end = i + interval};
      args[idx % numThreads].task_lst.push_back(task);
      idx++;
    }

    // Spawn the worker threads.  Note that only numThreads-1 std::threads
    // are created and the main application thread is used as a worker
    // as well.
    for (int i = 1; i < numThreads; i++)
    {
        workers[i] = std::thread(workerThreadStart, &args[i]);
    }

    workerThreadStart(&args[0]);

    // join worker threads
    for (int i = 1; i < numThreads; i++)
    {
        workers[i].join();
    }
    
    // for(int i = 0; i < numThreads; i++){
    //     cout << *args[i].time;
    //     if(i < numThreads - 1)
    //         cout << ", ";
    // }
    // cout << endl;
}
