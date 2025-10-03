/*
 *	@file	Task.h
 */
#ifndef _TASK_H_
#define _TASK_H_
#include <coroutine>
#include <memory>
#include <vector>
#include <exception>
#include <functional>

 // タスクキャンセル用
struct CancellationToken {
    bool canceled = false;
};

// コルーチンで値を返すTask<T>
template<typename T = void>
struct Task {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T value;
        std::exception_ptr exception;

        Task get_return_object() { return Task{ handle_type::from_promise(*this) }; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T v) { value = v; }
        void unhandled_exception() { exception = std::current_exception(); }
    };

    handle_type coro;
    Task(handle_type h) : coro(h) {}
    ~Task() { if (coro) coro.destroy(); }
};

// Schedulerで管理するためのインターフェース
struct ISchedulable {
    virtual bool Update() = 0; // trueなら継続
    virtual ~ISchedulable() = default;
};

// Scheduler：タスクを登録して毎フレーム更新
struct Scheduler {
    std::vector<std::shared_ptr<ISchedulable>> tasks;

    void Add(std::shared_ptr<ISchedulable> t) { tasks.push_back(t); }

    void Update() {
        for (auto it = tasks.begin(); it != tasks.end();) {
            if (!(*it)->Update()) it = tasks.erase(it);
            else ++it;
        }
    }
};
#endif // !_TASK_H_

