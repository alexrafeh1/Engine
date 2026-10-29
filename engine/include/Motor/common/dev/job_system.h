#ifndef __JOB_SYSTEM_H__
#define __JOB_SYSTEM_H__ 1

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <type_traits>

struct GLFWwindow;

class JobSystem
{
public:
    JobSystem(size_t workerCount, GLFWwindow* main_window);
    ~JobSystem();

    template<typename F>
    auto Submit(F&& job) -> std::future<std::invoke_result_t<F>>
    {
        using ReturnType = std::invoke_result_t<F>;

        auto task =
            std::make_shared<std::packaged_task<ReturnType()>>(
                std::forward<F>(job)
            );

        std::future<ReturnType> future = task->get_future();

        {
            std::lock_guard<std::mutex> lock(mutex_);

            jobs_.push([task]()
            {
                (*task)();
            });
        }

        condition_.notify_one();

        return future;
    }

private:
    void WorkerLoop(int i);

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> jobs_;

    std::mutex mutex_;
    std::condition_variable condition_;

    std::vector<GLFWwindow*> shared_window_;

    bool running_ = true;
};

#endif