#include "Motor/common/dev/job_system.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>


JobSystem::JobSystem(size_t workerCount, GLFWwindow* main_window)
{

    workers_.reserve(workerCount);
    shared_window_.reserve(workerCount);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    for (size_t i = 0; i < workerCount; ++i)
    {

         GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, main_window);

         shared_window_.push_back(window);
         workers_.emplace_back(&JobSystem::WorkerLoop,this,static_cast<int>(i));
    }
}

JobSystem::~JobSystem()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
    }

    condition_.notify_all();

    for (std::thread& worker : workers_)
    {
        if (worker.joinable())
            worker.join();
    }

    glfwMakeContextCurrent(nullptr);

    for (GLFWwindow* window : shared_window_)
    {
        glfwDestroyWindow(window);
    }
}

void JobSystem::WorkerLoop(int i)
{
    glfwMakeContextCurrent(shared_window_[i]);

    printf("Worker %d GL VERSION: %s\n", i, glGetString(GL_VERSION));
    printf("Worker %d GL CONTEXT: %p\n",
        i,
        glfwGetCurrentContext());

    while (true)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            condition_.wait(lock, [this]()
                {
                    return !jobs_.empty() || !running_;
                });

            if (!running_ && jobs_.empty())
                return;

            job = std::move(jobs_.front());
            jobs_.pop();
        }

        job();
    }
}