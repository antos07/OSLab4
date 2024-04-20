#include <iostream>
#include <cstdio>
#include <format>
#include <cinttypes>
#include <exception>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

using namespace std::chrono_literals;


::pid_t get_writer_pid() {
    ::pid_t pid{};
    do {
        std::cin.clear();
        std::cout << "Enter the writer PID: ";
    } while (!(std::cin >> pid));
    return pid;
}


std::ptrdiff_t get_writer_value_offset() {
    std::ptrdiff_t offset{};
    do {
        std::cin.clear();
        std::cout << "Enter the writer value address: ";
    } while (!(std::cin >> std::hex >> offset));
    std::cin >> std::dec;
    return offset;
}


std::int32_t read_value_from_writer(::pid_t writer_pid, std::ptrdiff_t offset) {
//    if (::ptrace(PTRACE_ATTACH, writer_pid) == -1) {
//        std::perror(nullptr);
//        throw std::runtime_error{"Failed to attach to the writer process"};
//    }
//    if(::waitpid(writer_pid, nullptr, 0) == -1) {
//        std::perror(nullptr);
//        throw std::runtime_error{"Failed to terminate to the writer process"};
//    }

    std::string memory_file{std::format("/proc/{}/mem", writer_pid)};
    int fd {::open(memory_file.c_str(), O_RDONLY)};
    if (fd == -1) {
        std::perror(nullptr);
        throw std::runtime_error{"Failed to open the writer memory"};
    }

    std::int32_t value{};
    if (::pread(fd, &value, sizeof(value), offset) == -1) {
        std::perror(nullptr);
        throw std::runtime_error{"Failed to read writer memory"};
    }

    ::close(fd);

//    if (::ptrace(PTRACE_DETACH, writer_pid) == -1) {
//        std::perror(nullptr);
//        throw std::runtime_error{"Failed to detach from the writer process"};
//    }

    return value;
}


int main() {
    ::pid_t writer_pid{get_writer_pid()};
    std::ptrdiff_t  writer_value_offset{get_writer_value_offset()};

    std::int32_t old_value{read_value_from_writer(writer_pid, writer_value_offset)};
    std::cout << "New value from writer: " << old_value << std::endl;
    while (true) {
        std::this_thread::sleep_for(1s);

        std::int32_t new_value{read_value_from_writer(writer_pid, writer_value_offset)};

        if (old_value == new_value)
            continue;

        std::cout << "New value from writer: " << new_value << std::endl;
        old_value = new_value;
    }

    return 0;
}
