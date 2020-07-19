#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>


#ifdef __SOLA_LOGGING_ENABLED
#define __SOLA_LOG(level,msg) sola::level(msg,__FILE__,__LINE__);
#else
#define __SOLA_LOG(level,msg)
#endif

namespace sola {
    class logger_iface{
        public:
        logger_iface(void) = default;

        virtual ~logger_iface(void) = default;

        logger_iface(const logger_iface&) = default;
        logger_iface& operator=(const logger_iface&) = default;

        public:
        virtual void debug(const std::string& msg,const std::string& file,std::size_t line) = 0;

        virtual void info(const std::string& msg,const std::string& file,std::size_t line) = 0;

        virtual void warn(const std::string& msg,const std::string& file,std::size_t line) = 0;

        virtual void error(const std::string& msg,const std::string& file,std::size_t line) = 0;

    };
    class logger : public logger_iface {
    public:

        enum class log_level {
            error = 0,
            warn = 1,
            info = 2,
            debug = 3
        };

    public:
        logger(log_level = log_level::info);

        ~logger(void) = default;

        logger(const logger&) = default;

        logger& operator=(const logger&) = default;

    public:
        void debug(const std::string& msg, const std::string& file, std::size_t line);
        void error(const std::string& msg, const std::string& file, std::size_t line);
        void info(const std::string& msg, const std::string& file, std::size_t line);
        void warn(const std::string& msg, const std::string& file, std::size_t line);
        
    private:
        log_level m_level;
        std::mutex m_mutex;
    };

    extern std::unique_ptr<logger_iface> active_logger;
    void debug(const std::string& msg, const std::string& file, std::size_t line);
    void info(const std::string& msg, const std::string& file, std::size_t line);
    void warn(const std::string& msg, const std::string& file, std::size_t line);
    void error(const std::string& msg, const std::string& file, std::size_t line);
        



class ThreadPool{
public:
    static const int kInitThreadsSize = 3;

    
    bool m_is_started;

    // enum taskPriorityE { level0=0, level1=1, level2=2};
    typedef std::function<void()> Task;
    typedef std::pair<int, Task> TaskPair;
    
    // ThreadPool();
    ThreadPool(int init_size = 4);
    ~ThreadPool();

    void start();
    void stop();
    void addTask(const Task&,int level);
    void addTask(const TaskPair&);

private:
    ThreadPool(const ThreadPool&);
    const ThreadPool& operator=(const ThreadPool&);

    struct TaskPriorityCmp{
        bool operator()(const ThreadPool::TaskPair p1, const ThreadPool::TaskPair p2)
        {
            return p1.first < p2.first;
        }
    };

    void thread_loop();
    Task take();

    typedef std::vector<std::thread*> Threads;
    // typedef std::priority_queue<TaskPair,std::vector<TaskPair>> Tasks;
    typedef std::priority_queue<TaskPair,std::vector<TaskPair>,TaskPriorityCmp> Tasks;

private:
    Threads m_threads;
    Tasks m_tasks;

    int m_init_threads_size;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    // bool m_is_started;
};

}

#endif