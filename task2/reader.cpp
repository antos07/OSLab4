#include <iostream>
#include <format>
#include <string>
#include <cinttypes>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <map>


const std::map<int, char> key_codes_to_symbols{
        {KEY_0, '0'},
        {KEY_1, '1'},
        {KEY_2, '2'},
        {KEY_3, '3'},
        {KEY_4, '4'},
        {KEY_5, '5'},
        {KEY_6, '6'},
        {KEY_7, '7'},
        {KEY_8, '8'},
        {KEY_9, '9'},
        {KEY_A, 'a'},
        {KEY_B, 'b'},
        {KEY_C, 'c'},
        {KEY_D, 'd'},
        {KEY_E, 'e'},
        {KEY_F, 'f'},
        {KEY_G, 'g'},
        {KEY_H, 'h'},
        {KEY_I, 'i'},
        {KEY_J, 'j'},
        {KEY_K, 'k'},
        {KEY_L, 'l'},
        {KEY_M, 'm'},
        {KEY_N, 'n'},
        {KEY_O, 'o'},
        {KEY_P, 'p'},
        {KEY_Q, 'q'},
        {KEY_R, 'r'},
        {KEY_S, 's'},
        {KEY_T, 't'},
        {KEY_U, 'u'},
        {KEY_V, 'v'},
        {KEY_W, 'w'},
        {KEY_X, 'x'},
        {KEY_Y, 'y'},
        {KEY_Z, 'z'},
        {KEY_SPACE, ' '},
        {KEY_COMMA, ','},
        {KEY_DOT, '.'},
};


std::uint32_t get_event_index() {
    std::uint32_t event_index{};
    do {
        std::cin.clear();
        std::cout << "Enter the event index (4 for the keyboard on my machine): ";
    } while (!(std::cin >> event_index));
    return event_index;
}


int main() {
    std::uint32_t event_index{get_event_index()};

    std::string filename{std::format("/dev/input/event{}", event_index)};
    int fd = ::open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        std::perror(nullptr);
        throw std::runtime_error{"Failed to open the event file"};
    }

    while (true) {
        ::input_event event{};
        if (read(fd, &event, sizeof(event)) == -1) {
            throw std::runtime_error{"Failed to read the event"};
        }

        if (event.type != EV_KEY || !key_codes_to_symbols.contains(event.code) || event.value != 1)
            continue;

        std::cout << key_codes_to_symbols.at(event.code) << std::flush;
    }
}
