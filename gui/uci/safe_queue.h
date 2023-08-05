#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class safe_queue
{
public:
    void push(const T value)
    {
        std::lock_guard lock(m_mutex);
        m_queue.push(value);
        m_condition.notify_one();
    };

    T pop()
    {
        std::unique_lock lock(m_mutex);
        m_condition.wait(lock, [&]{ return !m_queue.empty(); });

        auto value = m_queue.front();
        m_queue.pop();
        return value;
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
};
