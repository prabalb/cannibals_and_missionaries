/*
 * Cannibals and Missionaries Problem
 * Author: Prabal Basu (A02049867)
 * Contact: prabalb@aggiemail.usu.edu
 */


#include "./cannMiss.h"

int main() {
  int numMissionaries = 0, numCannibals = 0;

  std::cout << "Enter the number of missionaries: ";
  std::cin >> numMissionaries;

  std::cout << "Enter the number of cannibals: ";
  std::cin >> numCannibals;

  if(numMissionaries > 0 && numCannibals > numMissionaries) {
    std::cout << "No solution exists ..." << std::endl;
    return 1;
  }
  
  State *startingState = new State(numCannibals, numMissionaries, true);
  StateGraph &stGraph = StateGraph::getOrCreateStateGraph(startingState, numCannibals, numMissionaries);
  //stGraph.showConnections();
  if(!stGraph.isSolvable()) {
    std::cout << "No solution exists ..." << std::endl;
    return 1;
  }

  std::cout << "Number of boat crossing needed: " << stGraph.doBFS(startingState) << std::endl;
  return 0;
}
