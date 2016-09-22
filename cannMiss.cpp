/*
 * Cannibals and Missionaries Problem
 * Author: Prabal Basu (A02049867)
 * Contact: prabalb@aggiemail.usu.edu
 */


#include "./cannMiss.h"
#include <cassert>

unsigned int StateGraph::numCannibals, StateGraph::numMissionaries;

State::State (unsigned int numCann, unsigned int numMiss, bool boatAtLeft) {
  assert(numCann >= 0 && numMiss >= 0);
  state = std::make_tuple(numCann, numMiss, boatAtLeft);
}

// number of cannibals should not be more than the number of missionaries at any state
bool State::isLegalState () {
  bool legalLeftBankState  = std::get<1>(state) > 0 ? std::get<0>(state) <= std::get<1>(state) : true;
  bool legalRightBankState = (StateGraph::getTotalNumberofMissionaries() - std::get<1>(state)) > 0 ?
                             (StateGraph::getTotalNumberofCannibals() - std::get<0>(state)) 
                              <= (StateGraph::getTotalNumberofMissionaries() - std::get<1>(state)) : true;

  return (legalLeftBankState && legalRightBankState);
}

// Goal state: all people are at the right side of the river <0,0,0>
bool State::isGoalState () const {
  if(std::get<0>(state) == 0 &&
     std::get<1>(state) == 0 &&
     std::get<2>(state) == false) {
    return true;
  }
  return false;
}

void State::printState () {
  std::cout << std::get<0>(state) << "," << std::get<1>(state) << "," << std::get<2>(state) << std::endl;
}

Tuple State::getStateAttr () {
  return state;
}

// generate all the possible next states from a given state
bool StateGraph::generateLegalNextStates (State *startingState, std::vector<State*>& nextSates) {
  unsigned int num_cann = std::get<0>((*startingState).getStateAttr()); 
  unsigned int num_miss = std::get<1>((*startingState).getStateAttr()); 
  bool boatPos = std::get<2>((*startingState).getStateAttr());

  if(boatPos) {
    int iter = 2;
    while (iter > 0 && num_cann > 0) {
      --num_cann;
      --iter;
      State *s = new State(num_cann, num_miss, !boatPos);
      if(!s->isLegalState()) {
        delete[] s;
        continue;
      }
      if(search(s, allStates)) {
        continue;
      }
      nextSates.push_back(s);
    }

    num_cann = std::get<0>((*startingState).getStateAttr());
    iter = 2;
    while (iter > 0 && num_miss > 0) {
      --num_miss;
      --iter;
      State *s = new State(num_cann, num_miss, !boatPos);
      if(!s->isLegalState()) {
        delete[] s;
        continue;
      }
      if(search(s, allStates)) {
        continue;
      }
      nextSates.push_back(s);
    }

    num_miss = std::get<1>((*startingState).getStateAttr());
    if(num_cann > 0 && num_miss > 0) {
      State *s = new State(num_cann - 1, num_miss - 1, !boatPos);
      if(s->isLegalState() && !search(s, allStates)) {
        nextSates.push_back(s);
      } else {
        delete[] s;
      }
    }
  } else {
    num_cann = std::get<0>((*startingState).getStateAttr()); 
    num_miss = std::get<1>((*startingState).getStateAttr()); 
    while (num_cann < getTotalNumberofCannibals()) {
      ++num_cann;
      State *s = new State(num_cann, num_miss, !boatPos);
      if(!s->isLegalState()) {
        delete[] s;
        continue;
      }
      if(search(s, allStates)) {
        continue;
      }
      nextSates.push_back(s);
    }

    num_cann = std::get<0>((*startingState).getStateAttr()); 
    while (num_miss < getTotalNumberofMissionaries()) {
      ++num_miss;
      State *s = new State(num_cann, num_miss, !boatPos);
      if(!s->isLegalState()) {
        delete[] s;
        continue;
      }
      if(search(s, allStates)) {
        continue;
      }
      nextSates.push_back(s);
    }

    num_miss = std::get<1>((*startingState).getStateAttr()); 
    if(num_cann < getTotalNumberofCannibals() && num_miss < getTotalNumberofMissionaries()) {
      State *s = new State(num_cann + 1, num_miss + 1, !boatPos);
      if(s->isLegalState() && !search(s, allStates)) {
        nextSates.push_back(s);
      } else {
        delete[] s;
      }
    }
  }

  if(nextSates.size() == 0) {
    return false;
  }
  return true;
}

// create or get the state-graph handler
StateGraph& StateGraph::getOrCreateStateGraph (State *s, unsigned int c, unsigned int m) {
  numCannibals = c;
  numMissionaries = m;
  static StateGraph sg;
  sg.createAndConnectStates(s);
  return sg;
}

unsigned int StateGraph::getTotalNumberofCannibals () {
  return numCannibals;
}

unsigned int StateGraph::getTotalNumberofMissionaries () {
  return numMissionaries;
}

bool StateGraph::search (State *s, std::set<State*> sset) {
  for(std::set<State*>::iterator iter = sset.begin(); iter != sset.end(); iter++) {
    if((*iter)->getStateAttr() == s->getStateAttr()) {
      return true;
    }
  }
  return false;
}

// a recusrive method that actually creates the state-graph
bool StateGraph::createAndConnectStates (State *startingState) {
  if(search(startingState, allStates)) {
    return true;
  }
  allStates.insert(startingState);
  std::vector<State*> nextStates;
  generateLegalNextStates(startingState, nextStates);
  if(nextStates.size() > 0) {
    for(size_t i = 0; i < nextStates.size(); i++) {
      State *s = nextStates[i];
      if(s) {
        addState(startingState, s);
      }
    }
    for(size_t i = 0; i < nextStates.size(); i++) {
      State *s = nextStates[i];
      createAndConnectStates(s);
    }
  }
  return true;
}

void StateGraph::addState (State *s1, State *s2) {
  adjStatesMap[s1].push_back(s2);
}

void StateGraph::showConnections() {
  for(std::map<State*, std::vector<State*> >::iterator i = adjStatesMap.begin(); i != adjStatesMap.end(); i++) {
    ((*i).first)->printState(); 
    std::cout << "  ";
    std::vector<State*> v = (*i).second;
    for(std::vector<State*>::iterator j = v.begin(); j != v.end(); j++) {
      (*j)->printState();
      std::cout << "  ";
    }
    std::cout << "\n";
  }
}

// Breadth First Search traversal from the start state to the goal state
int StateGraph::doBFS (State *s) {
  int numBoatCrossing = 0;
  std::set<State*> visitedStates;
  std::list<State*> queueofStates;

  visitedStates.insert(s);
  queueofStates.push_back(s);

  bool goalReached = false;
  while(!queueofStates.empty()) {
    bool boatCrossed = false;
    State *s = queueofStates.front();
    queueofStates.pop_front();

    for(std::vector<State*>::iterator sIter = adjStatesMap[s].begin(); sIter != adjStatesMap[s].end(); sIter++) {
      if(search(*sIter, visitedStates)) {
        continue;
      }
      if(!boatCrossed) {
        boatCrossed = true;
        ++numBoatCrossing;
      }
      if((*sIter)->isGoalState()) {
        goalReached = true;
        break;
      }
      visitedStates.insert(*sIter);
      queueofStates.push_back(*sIter);
    }
    if(goalReached) {
      return numBoatCrossing;
    }
  }
  assert(0); // control should never reach here
  return numBoatCrossing;
}

// if the goal state is not reachable, the initial configuration is unsolvable
bool StateGraph::isSolvable() {
  for(std::set<State*>::iterator i = allStates.begin(); i != allStates.end(); i++) {
    if((*i)->isGoalState()) {
      return true;
    }
  }
  return false;
}

void StateGraph::printAllStates() {
  for(std::set<State*>::iterator i = allStates.begin(); i != allStates.end(); i++) {
    (*i)->printState();
  }
}
