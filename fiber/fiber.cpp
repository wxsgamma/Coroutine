#include"fiber.hpp"

static bool debug=false;

namespace Zeus{
static thread_local Fiber* t_fiber=nullptr;
// 主协程
static thread_local std::shared_ptr<Fiber> t_thread_fiber = nullptr;
// 调度协程
static thread_local Fiber* t_scheduler_fiber = nullptr;

// 协程计数器
static std::atomic<uint64_t> s_fiber_id{0};
// 协程id
static std::atomic<uint64_t> s_fiber_count{0};

void Fiber::SetThis(Fiber* f){
    t_fiber=f;
}

std::shared_ptr<Fiber> Fiber::GetThis(){
    if(t_fiber)return t_fiber->shared_from_this();
    std::shared_ptr<Fiber>mainfiber(new Fiber());
    t_thread_fiber=mainfiber;
    t_scheduler_fiber=mainfiber.get();
    assert(t_fiber==mainfiber.get());
    return t_fiber->shared_from_this();
}

}