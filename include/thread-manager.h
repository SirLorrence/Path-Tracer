#ifndef THREAD_TASKS
#define THREAD_TASKS

#include <functional>

#include "global.h"

class ThreadGuard {
 public:
  explicit ThreadGuard(std::thread &Thread, bool *ThreadStatus, uint32_t ID)
      : control_thread(Thread), thread_status(thread_status), r_id(ID){};

  ~ThreadGuard() {
    // std::clog << "Thread Done\n";
    if (control_thread.joinable()) {
      control_thread.join();
      std::clog << "Thread " << r_id << " Done\n";
      *thread_status = false;
    }
  };

  ThreadGuard(ThreadGuard const &) = delete;
  ThreadGuard &operator=(ThreadGuard const &) = delete;

 private:
  std::thread &control_thread;
  bool *thread_status;
  uint32_t r_id;
};

struct WorkThread {
  WorkThread() { inUse = false; }
  void AssignWork(std::function<void()> work) {
    inUse = true;
    std::thread worker_thread(work);
    ThreadGuard(worker_thread, &inUse, id);
  }
  bool inUse;
  uint32_t id = 0;
};

// template <typename T>
class ThreadPool {
 public:
  ThreadPool() {
    thread_pool.resize(kPoolSize);
    std::clog << "Pool Size: " << thread_pool.size() << "\n";
    for (uint32_t i = 0; i < thread_pool.size(); i++) {
      thread_pool[i] = WorkThread();
      thread_pool[i].id = i;
    }
  }

  // ~ThreadPool() {
  //   for (auto &entry : thread_pool) {
  //     entry.join();
  //   }
  // }
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
    bool thread_assigned = false;
    int timeout_count = 0;
    while (!thread_assigned) {
      for (uint32_t i = 0; i < kPoolSize; i++) {
        if (!thread_pool[i].inUse) {
          thread_pool[i].AssignWork(Work);
          thread_assigned = true;
          return;
        }
      }
      if (timeout_count > 10) {
        std::cerr << "No available threads\n";
        ThreadStatus();
        return;
      }
      timeout_count++;
    }

    //   for (auto &entry : thread_pool) {
    //     if (entry.joinable()) {
    //       entry.join();
    //     }
    // }
  }

 private:
  const uint32_t kPoolSize =
      static_cast<uint32_t>(std::thread::hardware_concurrency() - 2);
  std::vector<WorkThread> thread_pool;


  void ThreadStatus(){
    for (auto &worker : thread_pool)
    {
      std::clog << worker.inUse << " ";
    }
    std::clog << "\n";
  }
};

#endif  // THREAD_TASKS
