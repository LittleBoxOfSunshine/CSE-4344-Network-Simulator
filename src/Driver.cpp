//
// Created by cahenk on 4/17/16.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <climits>

#include "matrix.hpp"
#include "Simulator.hpp"

// Prototypes for network initialization
std::vector<Node*> starGen(int numNodes, std::vector<Node> & nodes);
std::vector<Node*> gridGen(int numNodes, std::vector<Node> & nodes);
void meshGen(int numNodes, std::vector<Node> & nodes);
Matrix getGaloisField(int m); // Returns GF(2^m) //if a field is calculated, don’t re-calculate
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

    Node A(1);
    Node B(2);
    Node C(3);
    Node D(4);
    Node E(5);
    Node F(6);
    Node G(7);
    Node H(8);
    Node I(9);

    std::vector<Node*> aNeighbors;
    aNeighbors.push_back((&B));
    aNeighbors.push_back((&C));
    aNeighbors.push_back((&D));
    A.setNeighbors(aNeighbors);

    std::vector<Node*> bNeighbors;
    bNeighbors.push_back((&C));
    bNeighbors.push_back((&A));
    bNeighbors.push_back((&F));
    B.setNeighbors(bNeighbors);

    std::vector<Node*> cNeighbors;
    cNeighbors.push_back((&A));
    cNeighbors.push_back((&D));
    cNeighbors.push_back((&B));
    cNeighbors.push_back((&E));
    C.setNeighbors(cNeighbors);

    std::vector<Node*> dNeighbors;
    dNeighbors.push_back((&A));
    dNeighbors.push_back((&C));
    D.setNeighbors(dNeighbors);

    std::vector<Node*> eNeighbors;
    eNeighbors.push_back((&C));
    eNeighbors.push_back((&G));
    E.setNeighbors(eNeighbors);

    std::vector<Node*> fNeighbors;
    fNeighbors.push_back((&B));
    fNeighbors.push_back((&H));
    F.setNeighbors(fNeighbors);

    std::vector<Node*> gNeighbors;
    gNeighbors.push_back((&E));
    gNeighbors.push_back((&I));
    G.setNeighbors(gNeighbors);

    std::vector<Node*> hNeighbors;
    hNeighbors.push_back((&F));
    H.setNeighbors(hNeighbors);

    std::vector<Node*> iNeighbors;
    iNeighbors.push_back((&G));
    I.setNeighbors(iNeighbors);

    A.buildRoutes();
    //A.printRoutingTable();


    return 0;
}
