#ifndef THREAD_TASKS
#define THREAD_TASKS

#include <functional>

#include "global.h"

class ThreadGuard {
 public:
  explicit ThreadGuard(std::thread &Thread) : p_thread(Thread){};

  ~ThreadGuard() {
    if (p_thread.joinable()) {
      p_thread.join();
    }
  };

  ThreadGuard(ThreadGuard const &) = delete;
  ThreadGuard &operator=(ThreadGuard const &) = delete;

 private:
  std::thread &p_thread;
};

// template <typename TData>
// struct WorkThread {
//   WorkThread(TData Task) : task(Task) {}
//   TData task;
//   std::thread worker_thread(task);
//   ThreadGuard(wThread);
// };

// template <typename T>
class ThreadPool {
 public:
  ThreadPool() { thread_pool.reserve(kPoolSize); }

  ~ThreadPool() {
    for (auto &entry : thread_pool) {
      entry.join();
    }
  }
  //   void AssignThread(std::function<void()> Work) {
  //     std::thread my_thread(Work);
  //     my_thread.join();
  //     std::clog << "Hello\n";
  //     }

  void AssignThread(std::function<void()> Work) {
    /*
    the main thread will continue to try to add the job for
    until theres availability
    */
    while (true) {
      for (uint32_t i = 0; i < kPoolSize; i++) {
        if (!thread_pool[i].joinable()) {
          thread_pool.emplace_back(Work);
          return;
        }
      }
      for (auto &entry : thread_pool) {
        if (entry.joinable()) {
          entry.join();
        }
      }
    }
  }

 private:
  const uint32_t kPoolSize =
      static_cast<uint32_t>(std::thread::hardware_concurrency() - 1);
  std::vector<std::thread> thread_pool;
};

#endif  // THREAD_TASKS
