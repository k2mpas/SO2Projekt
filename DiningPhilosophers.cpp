#include "DiningPhilosophers.h"
#include <iostream>
#include <random>
#include <chrono>

// convert state of a philosopher to string for reporting in the console
std::string stateToString(pState state) {
    switch (state) {
        case pState::Thinking: return "thinking.";
        case pState::Hungry: return "hungry.";
        case pState::Eating: return "eating.";
        default: return "---";
    }
}

// for printing lock so the console won't be a mess
SpinLock printLock;

// spinlock
void SpinLock::lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
        // and now we wait
    }
}
void SpinLock::unlock() {
    flag.clear(std::memory_order_release);
}

DiningPhilosophers::DiningPhilosophers(int n) : numPhilosophers(n), running(true) {
    // initialization of forks and philosophers
    for (int i = 0; i < n; i++) {
        forks.push_back(new std::atomic<bool>(false));
        states.push_back(new std::atomic<pState>(pState::Thinking));
    }
}

// clean up!
DiningPhilosophers::~DiningPhilosophers() {
    for (auto fork : forks) {
        delete fork;
    }
    for (auto state : states) {
        delete state;
    }
}

// for printing out in the console without a mess
void DiningPhilosophers::printState(int id, pState state) {
    LockGuard<SpinLock> guard(printLock);
    std::cout << "Philosopher " << id << " is " << stateToString(state) << std::endl;
}
// updating the states of a philosopher
void DiningPhilosophers::updateState(int id, pState state) {
    *states[id] = state;
    printState(id, state);
}

// function to try and get both forks (and getting them if available)
bool DiningPhilosophers::getForks(int id) {
    LockGuard<SpinLock> guard(forkLock);

    int leftFork = id;
    int rightFork = (id + 1) % numPhilosophers;

    // always locking left fork first
    int firstFork = std::min(leftFork, rightFork);
    int secondFork = std::max(leftFork, rightFork);

    // check for free forks in philosophers area
    if (!(*forks[firstFork]) && !(*forks[secondFork])) {
        *forks[firstFork] = true;
        *forks[secondFork] = true;
        return true;
    }

    // if unable to take forks
    return false;
}

void DiningPhilosophers::releaseForks(int id) {
    LockGuard<SpinLock> guard(forkLock);
    int leftFork = id;
    int rightFork = (id + 1) % numPhilosophers;
    *forks[leftFork] = false;
    *forks[rightFork] = false;
}

void DiningPhilosophers::philosopher(int id) {
    // random number for duration of waiting, eating, thinking
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomThinking(500, 2000);
    std::uniform_int_distribution<> randomEating(300, 1000);
    std::uniform_int_distribution<> randomWaiting(10, 50);

    while (running) {
        // thinking...
        updateState(id, pState::Thinking);
        std::this_thread::sleep_for(std::chrono::milliseconds(randomThinking(gen)));
        // becoming hungry
        updateState(id, pState::Hungry);

        // trying for forks when hungry
        bool hasForks = false;
        while (!hasForks && running) {
            hasForks = getForks(id);
            if (!hasForks) {
                // waiting before trying again
                std::this_thread::sleep_for(std::chrono::milliseconds(randomWaiting(gen)));
            }
        }
        // eating...
        updateState(id, pState::Eating);
        std::this_thread::sleep_for(std::chrono::milliseconds(randomEating(gen)));

        // putting down forks after finishing eating
        releaseForks(id);
    }
}

// stopping the simulation
void DiningPhilosophers::stop() {
    running = false;
}

void DiningPhilosophers::start() {
    std::vector<std::thread> threads;
    // creating philosophers' threads
    for (int i = 0; i < numPhilosophers; i++) {
        threads.emplace_back(&DiningPhilosophers::philosopher, this, i);
    }

    // stopping the simulation after giving a console input
    if (getchar()) {
        stop();
    }

    // joining threads - all philosophers - together
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}