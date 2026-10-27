#include "MessageQueue.h"

void MessageQueue::Push(std::shared_ptr<Message> message)
{
    {
        std::lock_guard lock(m_mutex);
        m_queue.push(message);
    }

    m_semaphore.release();
}

std::shared_ptr<Message> MessageQueue::WaitAndPop(std::chrono::milliseconds timeout)
{
    if (!m_semaphore.try_acquire_for(timeout))
    {
        return nullptr;
    }

    std::unique_lock lock(m_mutex);

    std::shared_ptr<Message> message = m_queue.front();
    m_queue.pop();

    return message;
}
