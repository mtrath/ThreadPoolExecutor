#ifndef PRINTTHREAD_H
#define PRINTTHREAD_H

#include <iostream>
#include <thread>

inline void printThread(const std::string& str)
{
    std::cout << std::hex << std::this_thread::get_id() << " " << str << std::endl;
}

#endif // PRINTTHREAD_H
