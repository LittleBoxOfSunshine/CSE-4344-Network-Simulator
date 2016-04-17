//
// Created by cahenk on 4/17/16.
// Written by Eric Smith
//

#include "Simulator.hpp"

Simulator::Simulator(){
    out.open("../log.csv");
    out <<"test,test,test";
    thread_ = std::thread(&Simulator::handler, this);
}

Simulator::~Simulator(){
    thread_.join();
    out.close();
}

void Simulator::handler() {
    if( queue.getQueue().empty() ){
        //print to file
        out << queue.pop() << "\n";
    }
    else{
        sleep(3);
    }
}

void Simulator::log(std::string logString){
    queue.push(logString);
}