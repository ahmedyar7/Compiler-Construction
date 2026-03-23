/*
 * NFA to DFA Conversion using Subset Construction Algorithm
 *
 * NFA Description (from diagram):
 *   States  : 0, 1, 2, 3, 5, 6, 7   (note: no state 4)
 *   Start   : 0
 *   Accept  : 6
 *   Alphabet: {a, b}
 *
 * Transitions:
 *   0 --e--> 1, 3, 2        (e = epsilon)
 *   1 --e--> 1 (self-loop)
 *   1 --a--> 3
 *   1 --b--> 7
 *   3 --e--> 3 (self-loop)
 *   3 --b--> 7
 *   2 --a--> 5
 *   2 --b--> 5
 *   5 --b--> 7
 *   7 --a--> 6
 *
 * Corrections/Assumptions made:
 *   1. The label "3, e" on the arrow from state 0 means an e-transition to
 * state 3 (not a new combined state). So 0 has three e-transitions: to 1, 3,
 * and 2.
 *   2. From state 2, both 'a' and 'b' lead to state 5 (the diagram shows two
 *      arrows from 2 to 5 labeled 'b' and 'a').
 */

#include <iostream>
using namespace std;

// ─────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────
const int MAX_NFA_STATES = 10;  // max NFA states
const int MAX_DFA_STATES = 64;  // max DFA (subset) states
const int MAX_SYMBOLS = 2;      // alphabet size: a=0, b=1
const int EPSILON = -1;         // epsilon marker
const int EMPTY = -1;           // empty / no state
const int MAX_TRANS = 10;       // max epsilon targets per state

// ─────────────────────────────────────────────
//  NFA Transition Table Entry
//  Each NFA state can go to multiple states on one symbol
// ─────────────────────────────────────────────
class NFATransition {
   public:
    int targets[MAX_TRANS];  // destination NFA states
    int count;               // how many targets

    NFATransition() {
        count = 0;
        for (int i = 0; i < MAX_TRANS; i++) targets[i] = EMPTY;
    }

    void addTarget(int state) {
        // avoid duplicates
        for (int i = 0; i < count; i++)
            if (targets[i] == state) return;
        targets[count++] = state;
    }
};

// ─────────────────────────────────────────────
//  NFA class
// ─────────────────────────────────────────────
class NFA {
   public:
    int numStates;
    int startState;
    int acceptStates[MAX_NFA_STATES];
    int numAccept;

    // on[state][symbol] — symbol: 0=a, 1=b
    NFATransition on[MAX_NFA_STATES][MAX_SYMBOLS];
    // epsilon[state]
    NFATransition eps[MAX_NFA_STATES];

    NFA() {
        numStates = 0;
        startState = 0;
        numAccept = 0;
        for (int i = 0; i < MAX_NFA_STATES; i++) acceptStates[i] = EMPTY;
    }

    void addTransition(int from, int symbol, int to) {
        on[from][symbol].addTarget(to);
    }

    void addEpsilon(int from, int to) { eps[from].addTarget(to); }

    void addAcceptState(int s) { acceptStates[numAccept++] = s; }

    bool isAccept(int s) const {
        for (int i = 0; i < numAccept; i++)
            if (acceptStates[i] == s) return true;
        return false;
    }
};

// ─────────────────────────────────────────────
//  StateSet — a set of NFA states (bit-array style)
// ─────────────────────────────────────────────
class StateSet {
   public:
    bool has[MAX_NFA_STATES];
    int members[MAX_NFA_STATES];
    int size;

    StateSet() {
        size = 0;
        for (int i = 0; i < MAX_NFA_STATES; i++) has[i] = false;
    }

    void add(int s) {
        if (s < 0 || s >= MAX_NFA_STATES) return;
        if (!has[s]) {
            has[s] = true;
            members[size++] = s;
        }
    }

    bool contains(int s) const {
        if (s < 0 || s >= MAX_NFA_STATES) return false;
        return has[s];
    }

    bool isEmpty() const { return size == 0; }

    // true if both sets have exactly the same members
    bool equals(const StateSet& other) const {
        for (int i = 0; i < MAX_NFA_STATES; i++)
            if (has[i] != other.has[i]) return false;
        return true;
    }

    void copyFrom(const StateSet& other) {
        size = other.size;
        for (int i = 0; i < MAX_NFA_STATES; i++) {
            has[i] = other.has[i];
            members[i] = other.members[i];
        }
    }

    void print() const {
        cout << "{";
        bool first = true;
        for (int i = 0; i < MAX_NFA_STATES; i++) {
            if (has[i]) {
                if (!first) cout << ",";
                cout << i;
                first = false;
            }
        }
        cout << "}";
    }
};

// ─────────────────────────────────────────────
//  DFA class — result of subset construction
// ─────────────────────────────────────────────
class DFA {
   public:
    int numStates;
    int startState;
    bool isAccept[MAX_DFA_STATES];
    int trans[MAX_DFA_STATES][MAX_SYMBOLS];  // -1 = dead state
    StateSet nfaSubset[MAX_DFA_STATES];      // which NFA states each DFA state
                                             // represents

    DFA() {
        numStates = 0;
        startState = 0;
        for (int i = 0; i < MAX_DFA_STATES; i++) {
            isAccept[i] = false;
            for (int j = 0; j < MAX_SYMBOLS; j++) trans[i][j] = EMPTY;
        }
    }

    // find the DFA state index for a given NFA subset; return -1 if not found
    int findState(const StateSet& s) const {
        for (int i = 0; i < numStates; i++)
            if (nfaSubset[i].equals(s)) return i;
        return -1;
    }

    // add a new DFA state; return its index
    int addState(const StateSet& s, bool accept) {
        int idx = numStates++;
        nfaSubset[idx].copyFrom(s);
        isAccept[idx] = accept;
        return idx;
    }
};

// ─────────────────────────────────────────────
//  SubsetConstruction — the algorithm
// ─────────────────────────────────────────────
class SubsetConstruction {
   private:
    NFA& nfa;
    DFA dfa;

    // ── ε-closure of a single state ──────────
    void epsClosure(int state, StateSet& result) const {
        if (result.contains(state)) return;  // already visited
        result.add(state);

        // follow all epsilon transitions
        for (int i = 0; i < nfa.eps[state].count; i++)
            epsClosure(nfa.eps[state].targets[i], result);
    }

    // ── ε-closure of a set ───────────────────
    StateSet epsClosure(const StateSet& states) const {
        StateSet result;
        for (int i = 0; i < states.size; i++)
            epsClosure(states.members[i], result);
        return result;
    }

    // ── move(states, symbol) ─────────────────
    // all NFA states reachable from 'states' on 'symbol' (no epsilon)
    StateSet move(const StateSet& states, int symbol) const {
        StateSet result;
        for (int i = 0; i < states.size; i++) {
            int s = states.members[i];
            for (int j = 0; j < nfa.on[s][symbol].count; j++)
                result.add(nfa.on[s][symbol].targets[j]);
        }
        return result;
    }

    // does a StateSet contain any NFA accept state?
    bool containsAccept(const StateSet& s) const {
        for (int i = 0; i < s.size; i++)
            if (nfa.isAccept(s.members[i])) return true;
        return false;
    }

   public:
    SubsetConstruction(NFA& n) : nfa(n) {}

    // ── Main algorithm ───────────────────────
    void run() {
        // Step 1: start state = ε-closure({NFA start})
        StateSet startSet;
        startSet.add(nfa.startState);
        StateSet startClosure = epsClosure(startSet);

        cout << "\n========================================\n";
        cout << "  NFA to DFA — Subset Construction\n";
        cout << "========================================\n\n";

        cout << "Step 1: e-closure of start state {" << nfa.startState
             << "} = ";
        startClosure.print();
        cout << "\n\n";

        dfa.addState(startClosure, containsAccept(startClosure));

        // worklist of unprocessed DFA states
        int worklist[MAX_DFA_STATES];
        int wHead = 0, wTail = 0;
        worklist[wTail++] = 0;

        cout << "Step 2: Processing subsets...\n";
        cout << "─────────────────────────────────────────\n";

        while (wHead < wTail) {
            int dfaState = worklist[wHead++];
            StateSet& current = dfa.nfaSubset[dfaState];

            cout << "DFA state D" << dfaState << " = ";
            current.print();
            cout << "\n";

            const char* symbols[] = {"a", "b"};

            for (int sym = 0; sym < MAX_SYMBOLS; sym++) {
                // move then epsilon-closure
                StateSet moved = move(current, sym);
                StateSet closure = epsClosure(moved);

                cout << "  on '" << symbols[sym] << "': move = ";
                moved.print();
                cout << "  e-closure = ";
                closure.print();

                if (closure.isEmpty()) {
                    cout << "  -> dead state\n";
                    dfa.trans[dfaState][sym] = EMPTY;
                    continue;
                }

                int existing = dfa.findState(closure);
                if (existing == EMPTY) {
                    // new DFA state
                    int newIdx = dfa.addState(closure, containsAccept(closure));
                    dfa.trans[dfaState][sym] = newIdx;
                    worklist[wTail++] = newIdx;
                    cout << "  -> NEW D" << newIdx << "\n";
                } else {
                    dfa.trans[dfaState][sym] = existing;
                    cout << "  -> D" << existing << "\n";
                }
            }
            cout << "\n";
        }
    }

    // ── Print ε-closure table ─────────────────
    void printEpsilonClosures() const {
        cout << "========================================\n";
        cout << "  ε-Closure Table (all NFA states)\n";
        cout << "========================================\n";
        cout << "State | ε-closure\n";
        cout << "------+------------------\n";

        for (int s = 0; s < nfa.numStates; s++) {
            StateSet single;
            single.add(s);
            StateSet cl;
            // recompute inline
            // use a small lambda-style helper via a temp object
            for (int i = 0; i < single.size; i++) {
                // BFS epsilon closure
                int stack[MAX_NFA_STATES];
                bool visited[MAX_NFA_STATES] = {};
                int top = 0;
                stack[top++] = single.members[i];
                while (top > 0) {
                    int cur = stack[--top];
                    if (visited[cur]) continue;
                    visited[cur] = true;
                    cl.add(cur);
                    for (int j = 0; j < nfa.eps[cur].count; j++)
                        if (!visited[nfa.eps[cur].targets[j]])
                            stack[top++] = nfa.eps[cur].targets[j];
                }
            }
            cout << "  " << s << "   | ";
            cl.print();
            cout << "\n";
        }
        cout << "\n";
    }

    // ── Print DFA transition table ────────────
    void printDFATable() const {
        cout << "========================================\n";
        cout << "  DFA Transition Table\n";
        cout << "========================================\n";
        cout << "State  | NFA Subset          | 'a'  | 'b'  | Accept?\n";
        cout << "-------+---------------------+------+------+--------\n";

        for (int i = 0; i < dfa.numStates; i++) {
            // state label (mark start with ->)
            cout << (i == dfa.startState ? "->" : "  ");
            cout << " D" << i << "   | ";

            // NFA subset
            dfa.nfaSubset[i].print();
            // padding for alignment
            int pad = 20 - dfa.nfaSubset[i].size * 2;
            for (int p = 0; p < pad; p++) cout << " ";
            cout << "| ";

            // transitions
            for (int sym = 0; sym < MAX_SYMBOLS; sym++) {
                int t = dfa.trans[i][sym];
                if (t == EMPTY)
                    cout << "dead  | ";
                else
                    cout << "D" << t << "    | ";
            }

            // accept?
            cout << (dfa.isAccept[i] ? "  YES" : "   no") << "\n";
        }
        cout << "\n";
    }

    // ── Print final states ────────────────────
    void printFinalStates() const {
        cout << "========================================\n";
        cout << "  DFA Final (Accept) States\n";
        cout << "========================================\n";
        bool any = false;
        for (int i = 0; i < dfa.numStates; i++) {
            if (dfa.isAccept[i]) {
                cout << "  D" << i << " (NFA subset: ";
                dfa.nfaSubset[i].print();
                cout << ") — contains NFA accept state 6\n";
                any = true;
            }
        }
        if (!any) cout << "  None.\n";
        cout << "\n";
    }

    // ── Summary ───────────────────────────────
    void printSummary() const {
        cout << "========================================\n";
        cout << "  Summary\n";
        cout << "========================================\n";
        cout << "  Total DFA states : " << dfa.numStates << "\n";
        cout << "  DFA start state  : D" << dfa.startState << "\n";
        cout << "  Alphabet         : {a, b}\n";
        int ac = 0;
        for (int i = 0; i < dfa.numStates; i++)
            if (dfa.isAccept[i]) ac++;
        cout << "  DFA accept states: " << ac << "\n\n";
    }
};

// ─────────────────────────────────────────────
//  buildNFA — encodes the NFA from the diagram
// ─────────────────────────────────────────────
NFA buildNFA() {
    /*
     * States used: 0,1,2,3,5,6,7
     * We index them 0-7 (state 4 simply has no transitions)
     * numStates = 8 so all indices 0..7 are valid
     *
     * Corrections applied:
     *   1) "3, e" arrow from 0 => epsilon from 0 to state 3
     *   2) State 2 has BOTH 'a' -> 5 AND 'b' -> 5
     */
    NFA nfa;
    nfa.numStates = 8;  // states 0..7
    nfa.startState = 0;
    nfa.addAcceptState(6);

    // ── Epsilon transitions ───────────────────
    nfa.addEpsilon(0, 1);  // 0 --e--> 1
    nfa.addEpsilon(0, 3);  // 0 --e--> 3  (the "3, e" label)
    nfa.addEpsilon(0, 2);  // 0 --e--> 2
    nfa.addEpsilon(1, 1);  // 1 --e--> 1  (self-loop)
    nfa.addEpsilon(3, 3);  // 3 --e--> 3  (self-loop)

    // ── Symbol transitions (0=a, 1=b) ─────────
    nfa.addTransition(1, 0, 3);  // 1 --a--> 3
    nfa.addTransition(1, 1, 7);  // 1 --b--> 7
    nfa.addTransition(3, 1, 7);  // 3 --b--> 7
    nfa.addTransition(2, 0,
                      5);  // 2 --a--> 5  (correction: both a and b go to 5)
    nfa.addTransition(2, 1, 5);  // 2 --b--> 5
    nfa.addTransition(5, 1, 7);  // 5 --b--> 7
    nfa.addTransition(7, 0, 6);  // 7 --a--> 6

    return nfa;
}

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────
int main() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║   NFA → DFA  (Subset Construction)      ║\n";
    cout << "╚══════════════════════════════════════════╝\n";

    cout << "\nNFA Description:\n";
    cout << "  States      : 0, 1, 2, 3, 5, 6, 7\n";
    cout << "  Start state : 0\n";
    cout << "  Accept state: 6\n";
    cout << "  Alphabet    : {a, b}\n";
    cout << "\nCorrections/Assumptions:\n";
    cout << "  1. Arrow labeled '3, e' from state 0 = epsilon-transition to "
            "state 3\n";
    cout << "  2. From state 2: both 'a' and 'b' lead to state 5\n\n";

    NFA nfa = buildNFA();

    SubsetConstruction sc(nfa);

    // Part (b): ε-closure table
    sc.printEpsilonClosures();

    // Part (a): run the algorithm (prints move/closure steps)
    sc.run();

    // Part (c): DFA transition table
    sc.printDFATable();

    // Part (d): final states
    sc.printFinalStates();

    // Summary
    sc.printSummary();

    return 0;
}