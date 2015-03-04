#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "method_request.h"
#include "activation_list.h"
#include "threadpool.h"

#include <thread>
#include <string>

class Scheduler
{
private:
    ActivationList _al;
    ThreadPool *t;

public:
    inline explicit Scheduler(ThreadPool *t) : t(t)
    {
    }

    inline explicit Scheduler() : t(NULL)
    {
    }
    
    inline ~Scheduler() {}
    
    void setThreadPool(ThreadPool *t);
    void insert(MethodRequest *mr);
    void run(std::string);
};

#endif
