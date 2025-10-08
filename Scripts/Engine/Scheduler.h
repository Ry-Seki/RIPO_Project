/*
 *  @file   Scheduler.h
 */

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "TaskInterface.h"

#include <coroutine>
#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <iostream>

class Scheduler {
private:
    std::vector<std::shared_ptr<TaskInterface>> tasks;
    std::mutex mtx;

public:
    static Scheduler& Instance() {
        static Scheduler instance;
        return instance;
    }

    void Add(std::shared_ptr<TaskInterface> task) {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push_back(task);
    }

    void Update() {
        std::lock_guard<std::mutex> lock(mtx);

        for (auto& t : tasks) t->Resume();

        tasks.erase(
            std::remove_if(tasks.begin(), tasks.end(),
            [](auto& t) { return t->IsDone(); }),
            tasks.end()
        );
    }
}; 
#endif // !_SCHEDULER_H_


