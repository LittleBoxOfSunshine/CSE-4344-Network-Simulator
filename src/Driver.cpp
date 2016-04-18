//
// Created by cahenk on 4/17/16.
//

#include <iostream>
#include <fstream>
#include <algorithm>

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

    std::vector<Node*> aNeighbors;
    aNeighbors.push_back((&B));
    aNeighbors.push_back((&C));
    aNeighbors.push_back((&D));
    A.setNeighbors(aNeighbors);

    std::vector<Node*> bNeighbors;
    bNeighbors.push_back((&C));
    bNeighbors.push_back((&A));
    B.setNeighbors(bNeighbors);

    std::vector<Node*> cNeighbors;
    cNeighbors.push_back((&A));
    cNeighbors.push_back((&D));
    C.setNeighbors(cNeighbors);

    std::vector<Node*> dNeighbors;
    dNeighbors.push_back((&A));
    dNeighbors.push_back((&C));
    D.setNeighbors(dNeighbors);

    //list of visited nodes
    std::vector<Node*> allNodes;

    Queue<Node*> frontier;

    //build topology

    //mark root visited
    allNodes.push_back(&A);
    frontier.push(&A);

    while( !frontier.getQueue().empty() ){
        Node* n = frontier.pop();

        for(int i=0;i<n->getNeighbors().size();i++){
            Node* nPrime = n->getNeighbors().at(i);

            //allNodes doesn't contain nPrime (nPrime hasn't been visited)
            if(std::find(allNodes.begin(), allNodes.end(), nPrime) == allNodes.end()){
                frontier.push(nPrime);

                //mark nPrime visited
                allNodes.push_back(nPrime);
            }
        }
    }

    for(int i=0;i<allNodes.size();i++)
        std::cout << allNodes.at(i)->getUniqueID() << std::endl;

    //Dij

    //can I switch these two?
    std::unordered_map<Node*, unsigned int> routingTable;

    //N' is represented by those nodes in routingTable with value > -1

    //initialize Dij
    //A = this
    //initialize to -1
    for(int i=0;i<allNodes.size();i++)
        routingTable.insert({allNodes.at(i), -1});

    routingTable.at(&A) = 0; //A = this

    //list of nodes whose least cost path is known
    std::vector<Node*> pathKnown;
    pathKnown.push_back(&A);

    for(int i=0;i<allNodes.size();i++){
        Node* v = allNodes.at(i);

        //current node is adjacent to root (this)
        if( std::find(A.getNeighbors().begin(), A.getNeighbors().end(), v) != A.getNeighbors().end() )
            routingTable.at(v) = 1;
        // = inf is -1
    }

    //loop until all least cost paths are known
    while( pathKnown.size() != allNodes.size() ){
        //find a node not in pathKnown
        //this is probably wrong, depending on what line 9 means
        Node* w;
        for(int i=0;i<allNodes.size();i++){
            if( std::find(pathKnown.begin(), pathKnown.end(), allNodes.at(i)) != pathKnown.end() ){
                w = allNodes.at(i);
                break;
            }
        }

        

    }

    return 0;
}
