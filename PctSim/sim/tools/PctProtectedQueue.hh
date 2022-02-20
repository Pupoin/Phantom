//created by hujf@scnu.edu.cn @ 2021.10.25
#ifndef QUEUES_PROTECTEDQUEUE_H_
#define QUEUES_PROTECTEDQUEUE_H_ 1

#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition.hpp>

/**
 * A finite-sized queue protected against multiple access from
 * different threads.
 */

template <class T>
class ProtectedQueue
{
public: // creation/destruction
    /// Create queue with size
    explicit ProtectedQueue(unsigned int size = 0);
    virtual ~ProtectedQueue();

public: // access
    /// Insert value into queue. Block if queue is full.
    void put(const T &value);

    /// Retrieve next value from queue. Block if queue is empty.
    bool get(T &value);

    /** Retrieve next value from queue. If queue is empty
     * block until at least 'num_ready' entries are available.
     *
     * This is a way to avoid wakeups if the cost of a context
     * switch is too high for a single value to be read from the
     * queue. Either this method or the previous one should be
     * use exclusively if you want to understand your program
     * behaviour.
     */
    bool get(T &value, unsigned int num_ready);

    /// Try to retrieve value from queue. Return true if successfull, false otherwise.
    bool try_get(T &value);

    /// Clear all remaining queue entries
    void clear();

    /// Clear all remaining queue entries, calling
    /// func(x) on each x in the queue.
    template <class FUNC>
    void clear(FUNC func);

    /** Helper function for clear for the common case where T
     *  is a pointer to some object that should be deleted:
     *     queue.clear(ProtectedQueue<T>::deleter())
     */
    struct deleter
    {
        /// delete the passed pointer
        void operator()(T p) { delete p; }
    };

    /// Wakeup any threads blocked on a get()
    void wakeup();
    // wakeup any threads blocked on a put();
    void stop();
    /// Met maximum size of queue. Returns 0 if unbounded queue.
    unsigned int max_size() const;

    /** Get current size of queue.
     * This value may be wrong by the time you look at
     * it. It should only be used as a hint and not as
     * a reliable indication of what's currently in the queue.
     */
    unsigned int size() const;

private:
    boost::mutex m_mutex;
    boost::condition_variable m_not_full;
    boost::condition_variable m_not_empty;
    unsigned int m_max_size;
    std::queue<T, std::deque<T>> m_queue;
    bool m_wakeup;
    bool m_stop;
    unsigned int m_wait_for;
};

template <class T>
ProtectedQueue<T>::ProtectedQueue(unsigned int size)
    : m_max_size(size),
      m_wakeup(false), m_stop(false),
      m_wait_for(0)
{
}

template <class T>
ProtectedQueue<T>::~ProtectedQueue()
{
}

template <class T>
void ProtectedQueue<T>::put(const T &value)
{
    boost::mutex::scoped_lock lock(m_mutex);
    m_stop = false;
    while ((m_max_size != 0) && m_queue.size() == m_max_size)
    {
        if (m_stop)
            break;
        m_not_full.wait(lock);
    }
    m_queue.push(value);

    if (m_queue.size() >= m_wait_for)
    {
        m_not_empty.notify_one();
    }
}

template <class T>
bool ProtectedQueue<T>::get(T &value)
{
    {
        boost::mutex::scoped_lock lock(m_mutex);

        while (!m_wakeup && m_queue.size() == 0)
        {
            m_not_empty.wait(lock);
        }

        if (m_wakeup)
        {
            return false;
        }

        value = m_queue.front();
        m_queue.pop();
    }

    m_not_full.notify_one();

    return true;
}

template <class T>
bool ProtectedQueue<T>::get(T &value, unsigned int num_ready)
{
    {
        boost::mutex::scoped_lock lock(m_mutex);

        while (!m_wakeup && !m_queue.size())
        {
            m_wait_for = num_ready;
            m_not_empty.wait(lock);
        }

        if (m_wakeup)
        {
            return false;
        }

        value = m_queue.front();
        m_queue.pop();

        m_wait_for = 0;
    }

    m_not_full.notify_one();

    return true;
}

template <class T>
bool ProtectedQueue<T>::try_get(T &value)
{
    m_mutex.lock();

    if (m_queue.size() > 0)
    {
        value = m_queue.front();
        m_queue.pop();
        m_mutex.unlock();
        m_not_full.notify_one();
        return true;
    }
    else
    {
        m_mutex.unlock();
        return false;
    }
}

template <class T>
void ProtectedQueue<T>::clear()
{
    boost::mutex::scoped_lock lock(m_mutex);
    while (!m_queue.empty())
    {
        m_queue.pop();
    }
    m_wakeup = false;
}

template <class T>
template <class FUNC>
void ProtectedQueue<T>::clear(FUNC func)
{
    boost::mutex::scoped_lock lock(m_mutex);
    while (!m_queue.empty())
    {
        func(m_queue.front());
        m_queue.pop();
    }
    m_wakeup = false;
}

template <class T>
void ProtectedQueue<T>::wakeup()
{
    boost::mutex::scoped_lock lock(m_mutex);

    m_wakeup = true;
    m_not_empty.notify_all();
}
template <class T>
void ProtectedQueue<T>::stop()
{
    boost::mutex::scoped_lock lock(m_mutex);

    m_stop = true;
    m_not_full.notify_all();
}
template <class T>
unsigned int ProtectedQueue<T>::max_size() const
{
    return m_max_size;
}

template <class T>
unsigned int ProtectedQueue<T>::size() const
{
    return m_queue.size();
}

#endif // QUEUES_PROTECTEDQUEUE_H_