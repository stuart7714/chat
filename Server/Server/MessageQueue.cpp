#include "MessageQueue.h"

void MessageQueue::Push(std::shared_ptr<Message> message)
{
    {
        std::lock_guard lock(m_mutex);
        m_queue.push(message);
    }

    m_condition.notify_one();
}

std::shared_ptr<Message> MessageQueue::WaitAndPop(std::chrono::milliseconds timeout)
{
    std::unique_lock lock(m_mutex);

    if (!m_condition.wait_for(lock, timeout, [this]
        {
            return !m_queue.empty();
        }))
    {
        return nullptr;
    }

    std::shared_ptr<Message> message = m_queue.front();
    m_queue.pop();

    return message;
}
