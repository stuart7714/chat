#pragma once

#include <memory>
#include <mutex>
#include <queue>

#include "Message.h"

/*
 * A queue of messages to be broadcast to subscribed clients.
 * The queue is accessed on different threads.
 */
class MessageQueue
{
public:
    // Push the message on the queue so it can be subsequently broadcast.
    void Push(std::shared_ptr<Message> message);

    // Wait for a message to appear on the queue and then pop it off so it
    // can be broadcast. The wait has a timeout.
    std::shared_ptr<Message> WaitAndPop(std::chrono::milliseconds timeout);

private:
    // The queue of messages.
    std::queue<std::shared_ptr<Message>> m_queue;
    
    // A mutex for the condition variable.
    std::mutex m_mutex;

    // The condition variable for when the queue is non-empty.
    std::condition_variable m_condition;
};
