/*
 * @Author       : Y. F. Zhang
 * @Date         : 2022-11-09
 * @copyleft Apache 2.0
 */ 

#ifndef THREADPOOLV2_H
#define THREADPOOLV2_H

#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
private:
    struct Pool {
        std::queue<std::function<void()>> tasks_;
        std::mutex mtx_;
        std::condition_variable cond_;
        std::atomic<bool> isClosed_;
        std::atomic<int> doTaskNum_;
    };
    std::shared_ptr<Pool> pool_;
public:
    explicit ThreadPool(int threadNum) : pool_(std::make_shared<Pool>()) {
        for (int i = 0; i < threadNum; ++i) {
            std::thread(
                [pool = pool_] () {
                    while (true) { 
                        if (pool->isClosed_.load()) break;
                        std::unique_lock<std::mutex> lk(pool->mtx_);
                        // The consumer implemented by conditional variable uses "while" instead of "if" to prevent false wake-up
                        while (pool->tasks_.empty()) { 
                            pool->cond_.wait(lk);
                        }
                        auto task = std::move(pool->tasks_.front());
                        pool->tasks_.pop();
                        lk.unlock(); // before excute task, unlock to allow other threads waken
                        pool->doTaskNum_.fetch_add(1);
                        task();
                        pool->doTaskNum_.fetch_sub(1);
                    }
                }
            ).detach();
        }
    }

    ~ThreadPool() {
        shutdown();
    }

    void shutdown() {
        pool_->isClosed_.store(true);
        pool_->cond_.notify_all();
    }
    
    template<class F> // 完美转发需要用模板
    void addTask(F&& t) {
        {
            std::unique_lock<std::mutex> lk(pool_->mtx_);
            pool_->tasks_.push(std::forward<F>(t));
        }
        pool_->cond_.notify_one();
    }

    int getDoTaskThreadNum() {
        return pool_->doTaskNum_.load();
    }
};

#endif