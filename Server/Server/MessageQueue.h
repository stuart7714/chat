#pragma once

#include <memory>
#include <mutex>
#include <queue>

#include "Message.h"

class MessageQueue
{
public:
    void Push(std::shared_ptr<Message> message);
    std::shared_ptr<Message> WaitAndPop(std::chrono::milliseconds timeout);
private:
    std::queue<std::shared_ptr<Message>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};
