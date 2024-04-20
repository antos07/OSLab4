#include <iostream>
#include <cinttypes>
#include <unistd.h>


int main() {
    std::cout << "My pid: " << ::getpid() << '\n';

    std::int32_t number{};
    std::cout << "Value address: " << &number << '\n' << std::endl;

    do {
        std::cout << "Enter a number: ";
    } while (std::cin >> number);

    return 0;
}
