#include"thread.hpp"

#include<iostream>
#include<sys/syscall.h>
#include <unistd.h>  

namespace Zeus{
static thread_local Thread* t_thread=NULL;
static thread_local std::string t_thread_name="UNKNOWN";

Thread::Thread(std::function<void()> cb,const std::string name):m_cb(cb),m_name(name){
    int ret=pthread_create(&m_thread,NULL,&Thread::run,this);
    if(ret){
        std::cerr<<"pthread create fail! ret="<<ret<<" name:"<<name;
        throw std::logic_error("pthread create fail!");
    }
    sem.wait();
}
Thread::~Thread(){
    if(m_thread){
        pthread_detach(m_thread);
        m_thread=0;
    }
}

void Thread::join(){
    if (m_thread) 
    {
        int ret = pthread_join(m_thread, nullptr);
        if (ret) 
        {
            std::cerr << "pthread_join failed, ret = " << ret << ", name = " << m_name << std::endl;
            throw std::logic_error("pthread_join error");
        }
        m_thread = 0;
    }
}

pid_t Thread::Get_thread_id(){
    return syscall(SYS_gettid);
}

Thread* Thread::Getthis(){
    return t_thread;
}

std::string Thread::Getname(){
    return t_thread_name;
}

void Thread::setname(const std::string& name){
    if(t_thread){
        t_thread->m_name=name;
    }
    t_thread_name=name;
}

void* Thread::run(void* arg){
    Thread* thread=(Thread*)arg;

    t_thread=thread;
    t_thread_name=thread->m_name;
    t_thread->m_id=Get_thread_id();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb); // swap -> 可以减少m_cb中只能指针的引用计数
    
    // 初始化完成
    thread->m_semaphore.signal();

    cb();
    return 0;
}
}