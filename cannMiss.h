/*
 * Cannibals and Missionaries Problem
 * Author: Prabal Basu (A02049867)
 * Contact: prabalb@aggiemail.usu.edu
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <tuple>

typedef std::tuple<unsigned int, unsigned int, bool> Tuple;

using namespace std;

class State {

  private:
    Tuple state; // < number of cannibals, number of missionaries, boat position >

  public:
    State (unsigned int numCann, unsigned int numMiss, bool boatAtLeft);

    bool isLegalState ();
    bool isGoalState () const;
    void printState ();

    Tuple getStateAttr();

};

// state-sorting rule
template<class T> struct ptr_less {
  bool operator()(T* lhs, T* rhs) {
    return lhs->getStateAttr() < rhs->getStateAttr(); 
  }
};

class StateGraph {

  private:
    static unsigned int numMissionaries;
    static unsigned int numCannibals;

    std::map<State*, std::vector<State*>, ptr_less<State> > adjStatesMap;
    std::set<State*> allStates;

  private:
    // StateGraph is singleton
    StateGraph () {}
    void operator=(StateGraph&);
    StateGraph (const StateGraph&);

  public:
    static StateGraph& getOrCreateStateGraph (State *s, unsigned int c, unsigned int m);

    static unsigned int getTotalNumberofCannibals ();
    static unsigned int getTotalNumberofMissionaries ();

    bool search (State *s, std::set<State*> sset);
    bool createAndConnectStates (State *startingState);
    bool generateLegalNextStates (State *startingState, std::vector<State*>& nextSates);
    bool isSolvable ();

    void addState (State *s1, State *s2);
    void showConnections();
    void printAllStates ();

    int doBFS (State *s);
};
