#include "scheduler.h"
#include "IntervalTimer.h"

#define MAX_TASKS 40

typedef struct
{
    bool repeat;
    int counter;
    int periodTicks;
    TASK task;
    
}TASK_SCHEDULE;

TASK_SCHEDULE taskList[MAX_TASKS];

static const int tickInterval_us = 10000;
IntervalTimer scheduler;

static void TaskScheduler(void);


void InitScheduler(void)
{
    int i;
    
    for(i=0; i<MAX_TASKS; i++)
    {
        taskList[i].task = NULL;
    }
}

void RunScheduler(void)
{

    scheduler.begin(TaskScheduler, tickInterval_us);
    
}


bool ScheduleTask(TASK task, int taskPeriod_ms, bool repeat)
{

    int i;

    for(i=0; i<MAX_TASKS; i++)
    {
        if(taskList[i].task == NULL)
        {
            taskList[i].periodTicks = (taskPeriod_ms * 1000) / tickInterval_us;
            taskList[i].counter = taskList[i].periodTicks + 1;
            taskList[i].repeat = repeat;
            taskList[i].task = task;

            return true;
        }
    }

    return false;       
       
}


bool CancelTask(TASK task)
{
    int i;

    for(i=0; i< MAX_TASKS; i++)
    {
        if(taskList[i].task == task)
        {
            taskList[i].task = NULL;
            return true;
        }
    }
    
    return false;

}

static void TaskScheduler(void)
{
    int i;
    
    for(i=0; i< MAX_TASKS; i++)
    {
        if(taskList[i].task != NULL)
        {
            if(!--taskList[i].counter)
            {
                (*taskList[i].task)();
                if(taskList[i].repeat)
                {
                    taskList[i].counter = taskList[i].periodTicks;
                }
                else
                {
                    taskList[i].task = NULL;
                }

            }
        }
    }

}
