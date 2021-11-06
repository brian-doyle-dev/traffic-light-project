template <class T>
class MessageQueue
{
public:
    T receive() {
        std::unique_lock<std::mutex> lock(_mut);
        _cond.wait(lock, [this] { return !_queue.empty(); });
        T message = std::move(_queue.back());

        _queue.pop_front();
        return message;
    }

    void send(T &&message)
    {
        std::lock_guard<std::mutex> lock(_mut);

        _queue.push_back(std::move(message));
        std::cout << "Queue Size: " << _queue.size() << std::endl;
#if 1
        while (_queue.size() > 1)
        {
            _queue.pop_front();
        }
#endif
        _cond.notify_one();
    }

private:
    std::deque<T> _queue;
    std::condition_variable _cond;
    std::mutex _mut;
};

