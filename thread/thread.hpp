/*
 * @Date: 2025-01-08 15:42:39
 * @LastEditTime: 2025-01-09 21:19:53
 * @FilePath: \Coroutine\thread\thread.hpp
 */
#ifndef _THREAD_HPP
#define _THREAD_HPP

#include<mutex>
#include<condition_variable>
#include<functional>
#include <unistd.h>
namespace Zeus{
    class Semaphore{
        private:
            std::condition_variable cv;
            std::mutex mtx;
            int _num=0;
        public:
            explicit Semaphore(int num=0):_num(num){}
            void wait(){
                std::unique_lock<std::mutex>lock(mtx);
                while(_num==0){
                    cv.wait(lock);
                };
                _num--;
            }
            void signal(){
                std::unique_lock<std::mutex>lock(mtx);
                _num++;
                cv.notify_one();
            }

    };

    class Thread{
        public:
            explicit Thread(std::function<void()> cb,const std::string name);
            ~Thread();
            
            pid_t getID() const {return m_id;};
            const std::string &getname() const {return m_name;};
            void join();
            
            static pid_t Get_thread_it();
            static Thread* Getthis();
            static const std::string& Getname();
            static void setname(const std::string& name);

        private:
            static void *run(void* arg);//运行函数
        
        private:
            pid_t m_id=-1;
            pthread_t m_thread=0;
            std::function<void()> m_cb;
            std::string m_name;
            Semaphore sem;
            

    };
}




#endif