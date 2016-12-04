#include <atomic>
#include <iostream>

void *sem_signal(std::atomic<int>& sem) {
    sem.fetch_add(1, std::memory_order_release);
    return nullptr;
}

void *sem_wait(std::atomic<int>& sem) {
    int oldCount;

    while(true) {
        oldCount = sem.load(std::memory_order_relaxed);
        if (oldCount > 0 && sem.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire)) {
            break;
        }
    }
    return nullptr;
}

int main() {
    std::atomic<int> sem(0);
    long long counter = 0;

    while (true) {
        sem_signal(sem);
        counter += 1;
        sem_wait(sem);
        if (!(counter % 10000)) {
            std::cout << "." << std::endl;
        }
    }
}
