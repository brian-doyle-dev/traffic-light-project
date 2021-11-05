template <class T>
class MessageQueue
{
public:
    T receive() {
        std::unique_lock<std::mutex> lock(_mut);
        _cond.wait(lock, [this] { return !_queue.empty(); });
        T message = std::move(_queue.back());

        _queue.pop_back();
        return message;
    }

    void send(T &&message)
    {
        std::unique_lock<std::mutex> lock(_mut);

        _queue.push_back(std::move(message));
        _cond.notify_one();
    }

private:
    std::deque<T> _queue;
    std::condition_variable _cond;
    std::mutex _mut;
};

