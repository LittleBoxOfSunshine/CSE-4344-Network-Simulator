//
// Created by cahenk on 4/17/16.
//

#include <iostream>
#include <fstream>

#include "matrix.hpp"
#include "Simulator.hpp"

// Prototypes for network initialization
std::vector<Node*> starGen(int numNodes, std::vector<Node> & nodes);
std::vector<Node*> gridGen(int numNodes, std::vector<Node> & nodes);
void meshGen(int numNodes, std::vector<Node> & nodes);

/*
int main( int argc, char * argv[] ) {

    // Check that required command line args were supplied
    if( argc == 3 ){
        // Load config
        std::ifstream configFile(argv[2], std::ios::in);


        // Load messages
        std::ifstream messageFile(argv[1], std::ios::in);

        // Create random network

        // Create & start simulator
        //Simulator exampleSimulator;
        //exampleSimulator.log("testdata1,testdata2");
        return 0;
    }
    // Invalid command line arguments, alert user and halt program
    else{
        std::cout << "ERROR: INVALID COMMAND LINE ARGS..." << std::endl
                  << "       Proper Usage: <exec_cmd> <topology_conf_path> <message_file_path>" << std::endl;
    }
}

*/

int main(){

    return 0;
}
