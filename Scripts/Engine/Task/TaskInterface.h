#ifndef _TASKINTERFACE_H_
#define _TASKINTERFACE_H_

#include <coroutine>

// 共通インターフェース
class TaskInterface {
public:
    virtual ~TaskInterface() noexcept {}
    virtual bool IsDone() const = 0;   // 完了判定
    virtual void Resume() = 0;         // 再開
};
#endif // !_TASKINTERFACE_H_
