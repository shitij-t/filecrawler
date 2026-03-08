#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class ThreadSafeQueue {
  std::queue<T> task_queue;
  std::condition_variable not_empty_cv;
  std::condition_variable not_full_cv;
  std::mutex queue_mtx;
  bool finished;
  size_t max_capacity;

  public:
  ThreadSafeQueue(size_t capacity = 10)
      : finished(false), max_capacity(capacity) {}
  void push(T item) {
    std::unique_lock<std::mutex> lock(queue_mtx);
    not_full_cv.wait(lock,
                     [this] { return (task_queue.size() < max_capacity); });
    task_queue.push(item);
    lock.unlock();
    not_empty_cv.notify_one();
  }

  bool pop(T &value) {
    std::unique_lock<std::mutex> lock(queue_mtx);

    not_empty_cv.wait(lock,
                      [this] { return (!task_queue.empty() || finished); });

    if (task_queue.empty() && finished) {
      lock.unlock();
      return false;
    }

    value = task_queue.front();
    task_queue.pop();
    lock.unlock();
    not_full_cv.notify_one();
    return true;
  }

  void set_finished() {
    std::lock_guard<std::mutex> lock(queue_mtx);
    finished = true;
    not_empty_cv.notify_all();
  }
};

#endif