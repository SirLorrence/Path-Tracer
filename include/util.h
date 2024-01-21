#pragma once

#include <functional>
#include "global.h"

// template <typename T>
class ThreadPool {
public:
  ThreadPool() {
    m_threadPool.resize(kPoolSize);
    std::clog << "Pool Size: " << m_threadPool.size() << "\n";
  }

  ~ThreadPool() { CleanThreads(); }

  void AssignThread(std::function<void()> work) {
    /*
    the main thread will continue to try to add the job for
    until theres availability
    */
    while (true) {
      for (uint32_t i = 0; i < kPoolSize; i++) {
        if (!m_threadPool[i].joinable()) {
          // std::clog << "Assigning thread\n";
          m_threadPool[i] = std::thread(work);
          return;
        }
      }
      CleanThreads();
    }
  }

  void ThreadStatus() const {
    for (const std::thread& worker : m_threadPool) {
      std::clog << worker.joinable() << " ";
    }
  }

  void CleanThreads() {
    for (std::thread& worker : m_threadPool) {
      if (worker.joinable()) {
        worker.join();
      }
    }
  }

private:
  const uint32_t kPoolSize = g_ThreadsAvailable;
  // const uint32_t kPoolSize = 1;
  std::vector<std::thread> m_threadPool;
};

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
