/*
 * @Date: 2025-01-08 15:42:39
 * @LastEditTime: 2025-01-08 16:03:47
 * @FilePath: \Coroutine\thread\thread.hpp
 */
#ifndef _THREAD_HPP
#define _THREAD_HPP

#include<mutex>
#include<condition_variable>

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

    }

}




#endif