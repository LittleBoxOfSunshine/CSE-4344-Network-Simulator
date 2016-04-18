//
// Created by cahenk on 4/17/16.
//

#include <iostream>

#include "matrix.hpp"
#include "Simulator.hpp"

// Prototypes for network initialization
void starGen(int numNodes, std::vector<Node> & nodes);
void gridGen(int numNodes, std::vector<Node> & nodes);
void meshGen(int numNodes, std::vector<Node> & nodes);
Matrix getGaloisField(int m); // Returns GF(2^m) //if a field is calculated, don’t re-calculate

int main( int argc, char * argv[] ) {

    // Check that required command line args were supplied
    if( argc == 3 ){

        return 0;
    }
    // Invalid command line arguments, alert user and halt program
    else{
        std::cout << "ERROR: INVALID COMMAND LINE ARGS..." << std::endl
                  << "       Proper Usage: <exec_cmd> <message_file_path> <topology_conf_path>" << std::endl;
    }
}

