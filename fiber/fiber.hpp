/*
 * @Date: 2025-01-08 21:17:43
 * @LastEditTime: 2025-01-13 18:19:25
 * @FilePath: \Coroutine\fiber\fiber.hpp
 */
#ifndef FIBER_HPP
#define FIBER_HPP

#include <iostream>     
#include <memory>       
#include <atomic>       
#include <functional>   
#include <cassert>      
#include <ucontext.h>   
#include <unistd.h>
#include <mutex>

namespace Zeus{

class Fiber : public std::enable_shared_from_this<Fiber>{
    public:
        enum state{
            READY,
            RUNNING,
            TERM
        };
    private:
        Fiber();
    public:
        Fiber(std::function<void()> cb,size_t stacksize=0,bool run_in_scheduler=true);
        ~Fiebr();

        void reset(std::function<void()>cb);
        void resume();
        void yield();
        uint64_t getid() const {return m_id;};
        state getstate() const {return m_state;};
        
        static void SetThis(Fiber* f);
        static std::shared_ptr<Fiber> GetThis();
        static void SetSchedulerFiber(Fiber* f);
        static uint64_t GetFiberId();
        static void MainFunc();

    private:
        uint64_t m_id=0;//Id
        uint32_t m_stacksize=0;
        state m_state=READY;
        ucontext_t m_ctx;
        void* m_stack=nullptr;
        bool m_runInScheduler;
        std::function<void()>cb;

    public:
        std::mutex m_mutex;
}


}


#endif