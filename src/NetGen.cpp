//
// Created by cahenk on 4/17/16.
//

/// This file contains code for random network / topology generation

#include <vector>

#include "matrix.hpp"
#include "Node.hpp"


/// Generate a random network with n nodes arranged in the star topology
void starGen(int numNodes, std::vector<Node> & nodes) {

}

/// Generate a random network with n nodes arranged in the grid topology
void gridGen(int numNodes, std::vector<Node> & nodes) {

}

/// Generate a random network with n nodes arranged in the mesh topology
void meshGen(int numNodes, std::vector<Node> & nodes) {
    nodes.reserve(numNodes);
    std::vector<std::tuple<int, int>> edges;
    for(int i = 0; i < nodes.size(); i++) {     //create nodes with messages and tick times
        nodes.at(i) = Node(0, )//TODO: add with message gen and tick time
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
