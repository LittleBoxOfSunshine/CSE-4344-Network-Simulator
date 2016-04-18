//
// Created by cahenk on 4/17/16.
//

/// This file contains code for random network / topology generation

#include <vector>

#include "matrix.hpp"
#include "Node.hpp"

/// Generate a random network with n nodes arranged in the star topology
void starGen(int numNodes) {
    std::vector<Node> nodes(numNodes);
    Node center = Node(0);
    std::vector<Node> leaves(numNodes-1);
    nodes.push_back(center);
    for(int i=1;i<numNodes;i++) {
        Node leaf = Node(i);
        std::vector<Node> leafNeighbor;
        leafNeighbor.push_back(center);
        leaf.setNeighbors(leafNeighbor);
        leaves.push_back(leaf);
        nodes.push_back(leaf);
    }
    center.setNeighbors(leaves);
}

/// Generate a random network with n nodes arranged in the grid topology
void gridGen(int numNodes, int numColumns) {
    std::vector<Node> nodes(numNodes);
    for(int i=0;i<numNodes;i++){
        std::vector<Node> neighbors;
        if(i - numColumns > 0){
            neighbors.push_back(nodes[i-numColumns]);
        }
        if(i + numColumns < numNodes){
            neighbors.push_back(nodes[i+numColumns]);
        }
        if(i%numColumns!=0){
            neighbors.push_back(nodes[i-1]);
        }
        if((i+1)%numColumns!=0){
            neighbors.push_back(nodes[i+1]);
        }
        nodes[i].setNeighbors(neighbors);
    }
}

/// Generate a random network with n nodes arranged in the mesh topology
void meshGen(int numNodes, std::vector<Node> & nodes) {

}

/// Generates the Galois Field GF(2^m) in a Matrix object
Matrix getGaloisField(int m) { // Returns GF(2^m) //if a field is calculated, don’t re-calculate

}