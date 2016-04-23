//
// Created by cahenk on 4/17/16.
//

/// This file contains code for random network / topology generation

#include <vector>

#include "matrix.hpp"
#include "Node.hpp"


/// Generate a random network with n nodes arranged in the star topology
std::vector<Node*> starGen(int numNodes) {
    std::vector<Node*> nodes(numNodes);
    Node* center = nodes[0];
    nodes.push_back(center);
    for(int i=1;i<numNodes;i++) {
        Node* leaf = nodes[i];
        std::vector<Node*> leafNeighbor;
        leafNeighbor.push_back(center);
        leaf->setNeighbors(leafNeighbor);
        nodes.push_back(leaf);
    }
    std::vector<Node*> leaves(nodes.begin()+1,nodes.end());
    center->setNeighbors(leaves);
    return nodes;
}

/// Generate a random network with n nodes arranged in the grid topology
std::vector<Node*> gridGen(int numNodes, int numColumns) {
    std::vector<Node*> nodes(numNodes);
    for(int i=0;i<numNodes;i++){
        std::vector<Node*> neighbors;
        if(i - numColumns >= 0){
            neighbors.push_back(nodes[i-numColumns]);
        }
        if(i + numColumns < numNodes){
            neighbors.push_back(nodes[i+numColumns]);
        }
        if(i%numColumns!=0){
            neighbors.push_back(nodes[i-1]);
        }
        if((i+1)%numColumns!=0 && i+1 < numNodes){
            neighbors.push_back(nodes[i+1]);
        }
        nodes[i]->setNeighbors(neighbors);
    }
    return nodes;
}

/// Generate a random network with n nodes arranged in the mesh topology
void meshGen(int numNodes, std::vector<Node> & nodes) {
    nodes.reserve(numNodes);
    std::vector<std::tuple<int, int>> edges;
    for(int i = 0; i < nodes.size(); i++) {     //create nodes with messages and tick times
        nodes.at(i) = Node(0, );//TODO: add with message gen and tick time
    }

    for(int i = 0; i < edges.size(); i++) {        //create all edges based on read file
        for(int j = 0; j < nodes.size(); j++) {
            createEdge(nodes.at(i), nodes.at(j));    //secondary node that is read from list
        }
    }
}

/// Generates the Galois Field GF(2^m) in a Matrix object
Matrix getGaloisField(int m) { // Returns GF(2^m) //if a field is calculated, don’t re-calculate

}
