#include <iostream>
#include <chrono>
#include <condition_variable>
#include "ThreadPool.h"


std::mutex g_mutex;

void priorityFunc()
{
    for(int i=1;i<4;i++){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "PriorityFunc() [" << i << "] at thread [ " << std::this_thread::get_id() << "] output" << std::endl;
    }
}

int main(int argc,char **argv)
{
  sola::active_logger = std::unique_ptr<sola::logger>(new sola::logger(sola::logger::log_level::debug));
  
  sola::ThreadPool thread_pool;

  for(int i = 0; i < 5 ; i++)
    thread_pool.addTask(priorityFunc,i);

  getchar();
  return 0;
}
