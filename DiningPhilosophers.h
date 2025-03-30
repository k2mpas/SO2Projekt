#ifndef DINING_PHILOSOPHERS_H
#define DINING_PHILOSOPHERS_H

#include <vector>
#include <atomic>
#include <thread>
#include <string>

enum class pState {
    Thinking,
    Hungry,
    Eating
};

// spinlock
class SpinLock {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock();
    void unlock();
};

// guard
template<typename T>
class LockGuard {
private:
    T& lock;
public:
    explicit LockGuard(T& l) : lock(l) {
        lock.lock();
    }
    ~LockGuard() {
        lock.unlock();
    }
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

class DiningPhilosophers {
private:
    int numPhilosophers;
    std::vector<std::atomic<bool>*> forks;
    std::vector<std::atomic<pState>*> states;
    std::atomic<bool> running;
    SpinLock forkLock;

    static void printState(int id, pState state);
    void updateState(int id, pState state);
    bool getForks(int id);
    void releaseForks(int id);
    void philosopher(int id);

public:
    explicit DiningPhilosophers(int n);
    ~DiningPhilosophers();
    void stop();
    void start();
};

#endif // DINING_PHILOSOPHERS_H