#include <iostream>
#include <pthread.h>
#include <mutex>
#include <string>
#include <unistd.h>

std::mutex mtx;
std::string shared_data = "";
bool data_available = false;

void* write_thread(void* arg) {
    for (int i = 0; i < 5; ++i) {
        mtx.lock();
        shared_data = "Message " + std::to_string(i + 1);
        std::cout << "Thread_1: Written -> " << shared_data << std::endl;
        data_available = true;
        mtx.unlock();
        sleep(1); // Simulate some work
    }
    return nullptr;
}

void* read_thread(void* arg) {
    for (int i = 0; i < 5; ++i) {
        mtx.lock();
        if (data_available) {
            std::cout << "Thread_2: Read -> " << shared_data << std::endl;
            data_available = false;
        }
        mtx.unlock();
        sleep(1); // Simulate some work
    }
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, nullptr, write_thread, nullptr);
    pthread_create(&thread2, nullptr, read_thread, nullptr);

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    return 0;
}
