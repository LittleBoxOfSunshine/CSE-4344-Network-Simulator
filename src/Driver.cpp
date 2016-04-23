//
// Created by cahenk on 4/17/16.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "matrix.hpp"
#include "Simulator.hpp"

// Prototypes for network initialization
std::vector<Node*> starGen(int numNodes, std::vector<Node> & nodes);
std::vector<Node*> gridGen(int numNodes, std::vector<Node> & nodes);
void meshGen(int numNodes, std::vector<std::vector<std::string>>& neighbors);
Matrix getGaloisField(int m); // Returns GF(2^m) //if a field is calculated, don’t re-calculate

int main( int argc, char * argv[] ) {

    // Check that required command line args were supplied
    if( argc == 2 ){
        // Load config
        std::string configPath(getenv("HOME"));
        configPath += "/ClionProjects/simulator/configs/";
        configPath += argv[1];
        std::ifstream configFile;
        configFile.open(configPath, std::ios::in);

        std::string topologyType;        //type of topology
        int numNodes;                   //number of nodes
        getline(configFile, topologyType);
        configFile >> numNodes;
        configFile.ignore();
        if(topologyType.compare("Grid") == 0) {
            int numCol;
            configFile >> numCol;       //get number of columns

            //CREATE TOPOLOGY HERE
        }
        else if(topologyType.compare("Mesh") == 0) {
            std::vector<std::vector<std::string>> neighbors(numNodes+1);    //vector for neighbors; ID used as index
            for(int i = 1; i < numNodes+1; i++) {
                std::string buffer;
                int nodeid;             //gets id for node
                configFile >> nodeid;
                configFile.ignore();
                getline(configFile, buffer);        //get whole line of neighbors for parsing
                std::stringstream stream(buffer);
                std::string temp;
                while(stream >> temp) {         //parse whole line and push to vector
                    neighbors.at(nodeid).push_back(temp);
                }
            }
            //CREATE TOPOLOGY HERE
            ::meshGen(numNodes, neighbors);
        }

        //only for mesh networks

        // Load messages
        //std::ifstream messageFile(argv[1], std::ios::in);

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



/*int main(){

    return 0;
}*/
