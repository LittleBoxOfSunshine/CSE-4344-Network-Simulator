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

    //Dij
    for(int i=0;i<allNodes.size();i++)
        std::cout << allNodes.at(i)->getUniqueID() << std::endl;

    std::cout << "----------" << std::endl;
    //can I switch these two?
    std::unordered_map<Node*, unsigned int> routingTable;

    //N' is represented by those nodes in routingTable with value > -1

    //initialize Dij
    //A = this
    //initialize to -1
    for(int i=0;i<allNodes.size();i++)
        routingTable.insert({allNodes.at(i), 4294967295});

    routingTable.at(&A) = 0; //A = this

    for (auto& x: routingTable) {
        std::cout << x.first->getUniqueID() << ": " << x.second << std::endl;
    }
    std::cout << "------------" << std::endl;

    //list of nodes whose least cost path is known
    std::vector<Node*> pathKnown;
    pathKnown.push_back(&A);

    for(int i=0;i<allNodes.size();i++){
        Node* v = allNodes.at(i);

        //current node is adjacent to root (this)
        if( std::find(A.getNeighbors().begin(), A.getNeighbors().end(), v) != A.getNeighbors().end() )
            routingTable.at(v) = 1;
    }

    //loop until all least cost paths are known
    while( pathKnown.size() != allNodes.size() ){
        Node* w;
        unsigned int leastCost = 4294967295;

        //find a node not in pathKnown with the minimum current cost
        for(int i=0;i<allNodes.size();i++){
            int curValue = routingTable.at( allNodes.at(i) );

            if( (std::find(pathKnown.begin(), pathKnown.end(), allNodes.at(i)) == pathKnown.end()) && curValue < leastCost ){
                w = allNodes.at(i);
                leastCost = curValue;
            }
        }

        //add w to N'
        pathKnown.push_back(w);

        //update D(v) for all v adjacent to w and not in pathKnown
        for(int i=0;i<w->getNeighbors().size();i++){
            Node* v = w->getNeighbors().at(i);

            if( std::find(pathKnown.begin(), pathKnown.end(), v) == pathKnown.end() ){
                routingTable.at(v) = std::min(routingTable.at(v), (routingTable.at(w) + 1));
            }
        }
    }

    for (auto& x: routingTable) {
        std::cout << x.first->getUniqueID() << ": " << x.second << std::endl;
    }

    //std::cout << "---------" << std::endl << INT_MAX << std::endl;

    return 0;
}
