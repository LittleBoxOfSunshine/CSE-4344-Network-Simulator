//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_SIMULATOR_HPP
#define SIMULATOR_SIMULATOR_HPP

#include "Node.hpp"
#include <iostream>
#include <fstream>
#include "Queue.hpp"
#include <pthread.h>
#include <unistd.h>

class Simulator {
private:
    std::thread thread_;
public:
    void log(std::string logString);
    std::ofstream out;
    Simulator();
    ~Simulator();
    Queue<std::string> queue;
    void handler();
};


#endif //SIMULATOR_SIMULATOR_HPP
