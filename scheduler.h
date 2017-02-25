typedef void  (* TASK)(void);

extern void InitScheduler(void);
extern void RunScheduler(void);
extern bool ScheduleTask(TASK task,int taskPeriod_ms, bool repeat);
extern bool CancelTask(TASK task);

