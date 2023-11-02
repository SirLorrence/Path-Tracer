#ifndef THREAD_TASKS
#define THREAD_TASKS

#include <functional>

#include "global.h"

// class ThreadGuard {
//  public:
//   explicit ThreadGuard(std::thread &Thread, bool *TStatus, uint32_t ID)
//       : control_thread(Thread), thread_status(TStatus), r_id(ID) {
//     *thread_status = true;
//   };

//   ~ThreadGuard() {
//     // std::clog << "Thread Done\n";
//     if (control_thread.joinable()) {
//       control_thread.join();
//       std::clog << "Thread " << r_id << " Done\n";
//       *thread_status = false;
//     }
//   };

//   ThreadGuard(ThreadGuard const &) = delete;
//   ThreadGuard &operator=(ThreadGuard const &) = delete;

//  private:
//   std::thread &control_thread;
//   bool *thread_status;
//   uint32_t r_id;
// };

// struct WorkThread {
//   WorkThread() { inUse = false; }
//   void AssignWork(std::function<void()> work) {
//     work_thread = std::thread(work);
//     // ThreadGuard(worker_thread, &inUse, id);
//   }
//   uint32_t id = 0;
//   bool inUse;
//   std::thread work_thread;

//  private:
//   void CompleteFlag(bool *flag) { *flag = false; };
// };

// template <typename T>
class ThreadPool {
 public:
  ThreadPool() {
    thread_pool.resize(kPoolSize);
    std::clog << "Pool Size: " << thread_pool.size() << "\n";
    // for (uint32_t i = 0; i < thread_pool.size(); i++) {
    //   thread_pool[i] = WorkThread();
    //   thread_pool[i].id = i;
    // }
  }
  ~ThreadPool() { CleanThreads(); }

  void AssignThread(std::function<void()> Work) {
    /*
    the main thread will continue to try to add the job for
    until theres availability
    */
    // std::clog << "Getting thread\n";
    bool thread_assigned = false;
    int timeout_count = 0;
    while (!thread_assigned) {
      for (uint32_t i = 0; i < kPoolSize; i++) {
        if (!thread_pool[i].joinable()) {
          // std::clog << "Assigning thread\n";
          thread_pool[i] = std::thread(Work);
          thread_assigned = true;
          return;
        }
      }
      CleanThreads();
      // if (timeout_count > 10) {
      //   std::cerr << "No available threads\n";
      //   ThreadStatus();
      //   return;
      // }
      // timeout_count++;
    }
  }
  void ThreadStatus() {
    for (auto &worker : thread_pool) {
      std::clog << worker.joinable() << " ";
    }
  }

  void CleanThreads() {
    for (auto &worker : thread_pool) {
      if (worker.joinable()) {
        worker.join();
      }
    }
  }

 private:
  const uint32_t kPoolSize =
      static_cast<uint32_t>(std::thread::hardware_concurrency() - 2);
  std::vector<std::thread> thread_pool;
};

#endif  // THREAD_TASKS
