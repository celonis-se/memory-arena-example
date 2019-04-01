#include <iostream>
#include <cstring>
#include <memory>
#include <thread>
#include <vector>
#include <malloc.h>
#include <random>

const int MB100 = 100 * 1024 * 1024;
const int MB30 = 30 * 1024 * 1024;

void constant_100MB_worker() {
    for (size_t j = 0; j < 10; ++j) {
        std::unique_ptr<char[]> allocated_memory(new char[MB100]());
        std::cout << "Allocated 100MB by thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void constant_30MB_worker() {
    for (size_t j = 0; j < 10; ++j) {
        std::unique_ptr<char[]> allocated_memory(new char[MB30]());
        std::cout << "Allocated 30MB by thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, char *argv[]) {
    std::string worker_name(argv[1]);
    const int threads = std::atoi(argv[2]);

    std::vector<std::thread> workers;
    for (size_t i = 0; i < threads; ++i) {
        if (worker_name == "30MB") {
            workers.emplace_back(constant_30MB_worker);
        } else if (worker_name == "100MB") {
            workers.emplace_back(constant_100MB_worker);
        }
    }

    for (std::thread &worker : workers) {
        worker.join();
    }
    std::string mystr;
    do {
        std::cout << "Enter 's' for statistic, 't' to trim or an empty line to exit ..." << std::endl;
        getline(std::cin, mystr);

        if (mystr.find('t') != std::string::npos) {
            malloc_trim(0);
        }

        if (mystr.find('s') != std::string::npos) {
            malloc_stats();
        }
    } while (!mystr.empty());
    return 0;
}
