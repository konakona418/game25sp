// Game - NWPU C++ sp25
// Created on 2025/5/15
// by konakona418 (https://github.com/konakona418)

#include "ThreadPool.hpp"

#include "Common.hpp"
#include "Logger.hpp"

game::ThreadPool::~ThreadPool() {
    close();
}

void game::ThreadPool::close() {
    getLogger().logDebug("ThreadPool::close()");
    if (!m_cancellationToken.exchange(false, std::memory_order_acq_rel)) {
        return;
    }

    m_cv.notify_all();
    for (auto& thread : m_threads) {
        thread.join();
    }
}

void game::ThreadPool::schedule(const Task& task) {
    std::unique_lock lock(m_tasksMutex);
    m_tasks.push_back(task);

    m_cv.notify_one();
}

void game::ThreadPool::schedule(std::function<void()> task) {
    schedule(Task(std::move(task)));
}

void game::ThreadPool::run() {
    getLogger().logDebug("ThreadPool::run");
    m_cancellationToken.exchange(true, std::memory_order_relaxed);
    m_threads.reserve(m_threadCount);

    for (uint32_t i = 0; i < m_threadCount; i++) {
        m_threads.emplace_back(&ThreadPool::executor, this);
    }
}

void game::ThreadPool::executor() {
    while (m_cancellationToken.load(std::memory_order_acquire)) {
        {
            std::unique_lock<std::mutex> lock(m_waitMutex);
            m_cv.wait(lock, [this] { return !m_tasks.empty() || !m_cancellationToken.load(std::memory_order_relaxed); });
        }

        if (!m_cancellationToken.load(std::memory_order_acquire)) {
            while (!m_tasks.empty()) {
                Task task;
                {
                    std::scoped_lock scopedLock(m_tasksMutex);

                    // 防止中间出错
                    if (!m_tasks.empty()) {
                        task = m_tasks.front();
                        m_tasks.pop_front();
                    }
                }
                task.run();
            }
            return;
        }

        while (!m_tasks.empty()) {
            Task task;
            {
                std::scoped_lock scopedLock(m_tasksMutex);
                if (!m_tasks.empty()) {
                    task = m_tasks.front();
                    m_tasks.pop_front();
                }
            }
            task.run();
        }
    }
}
