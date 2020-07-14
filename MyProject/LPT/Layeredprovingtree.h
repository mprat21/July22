#ifndef LAYEREDPROVINGTREE_H
#define LAYEREDPROVINGTREE_H

#include "LPTDefinitions.h"
#include "Statement.h"
#include "Postulate.h"
#include "Logic.h"
#include "LPTContainers.h"

namespace LPT {

class LayeredProvingTree
{
    class LPTNode;
public:
    // creates an LPT for the specified logic
    LayeredProvingTree(ImplementedLogics logic);
    LayeredProvingTree(ImplementedLogics logic, StatementPtrList &assumptions, StatementPtrList &steps,
                       StatementPtrList &goals);
    virtual ~LayeredProvingTree();
    // resets the LPT by removing all statements/verfication information
    void reset();
    // setup the tree for a verification by setting assumptions, steps and goals
    void initialise(StatementPtrList &assumptions, StatementPtrList &steps, StatementPtrList &goals);
    // starts the verification process with the given properties.
    bool verifyProtocol(bool exhaustive=false, TraverseType strategy=TraverseType::DFS);

    // get number of statement known to be true
    inline int getKnownTrueStatementCount() {return sigma.count();}
    inline const Statement *getKnownTrueStatement(int index) { return sigma.at(index);}

    void printInfo();


private:

    class LPTNodeList : public LPTList<LPTNode *> {
    public:
        /*
         * returns first node in list that stores stmt as statement.
         */
        LPTNode *getNodeByStatement(Statement *stmt) {
            LPTNode *foundNode = nullptr;
            int loopLimit = count();
            LPTNode *current=nullptr;
            for (int i=0; i<loopLimit; ++i) {
                current = at(i);
                if (*stmt == *(current->getStatement())) {
                    foundNode = current;
                    break;
                }
            }
            return foundNode;
        }
    };

    // abstract class for an LPT node - concreate classes are LPTAndNode and LPTOrNode
    class LPTNode
    {
        friend LayeredProvingTree;
    public:
        // default constructor - sets default properties common to both node types
        LPTNode();
        virtual ~LPTNode();
        // adds a child to an OR layer node (child will hold statement (i.e. prerequite to postulate in current OR node)
        virtual void addOrLayerChild(Statement *stmt) = 0;
        // adds a child to an AND layer node (child will hold postulate i.e. option to prove statement in current AND node)
        virtual LPTNode *addAndLayerChild(Postulate *instGamma) = 0;

        inline Statement *getStatement() { return stmt; }
        inline void incrementRefCount() { ++refCounter; }
        inline void decrementRefCount() { if (refCounter > 0) --refCounter;}
        inline TruthValue getTruthValue() { return truthValue; }

    protected:
        // global reference to used LPT
        static LayeredProvingTree *lpt;
        // keep track of number of nodes
        static int nodeCount;
        // is node ANDLayerNode or ORLayerNode
        LinkType layerType;
        // pointer to parent of current node
        LPTNode *parent;
        // list of children nodes
        LPTNodeList children;
        // pointer to node that is referenced by this node (target has same statement)
        LPTNode *crossRefTarget;
        // counter to count number of nodes referencing to this node
        int refCounter = 0;
        // holds details of postulate used in OR layer
        QString postulateDetail;
        // holds statement associated with node
        // for OR layer this is goal of postulate for AND layer is this a pre-requisite of postulate in previous OR layer
        Statement *stmt;
        // holds index of statement in list of known statements (applies only after statement is true). if statement
        // is not in sigma this holds -1
        int stmtIndexInSigma;
        // unique ID for node
        int nodeID;

        // node properties
        bool isRoot; // node is root of LPT
        bool isExtended; // node has been used in expansion (but expansion may have failed, ie. no need to use in expansion again)
        bool violatesTerminationRules; // node violates termination rule and should not be expanded
        TruthValue truthValue;
    };

    class LPTAndNode : public LPTNode {
    public:
        LPTAndNode(LPTNode *nodeParent=nullptr, Statement *nodeStmt=nullptr);
        ~LPTAndNode();
        // adds a child to an OR layer node (child will hold statement (i.e. prerequite to postulate in current OR node)
        virtual void addOrLayerChild(Statement *stmt);
        // adds a child to an AND layer node (child will hold postulate i.e. option to prove statement in current AND node)
        virtual LPTNode *addAndLayerChild(Postulate *instGamma);

    };

    class LPTOrNode : public LPTNode {
    public:
        // default constructor - creates root node
        LPTOrNode();
        LPTOrNode(LPTNode *nodeParent, Postulate *post);
        ~LPTOrNode();
        // adds a child to an OR layer node (child will hold statement (i.e. prerequite to postulate in current OR node)
        virtual void addOrLayerChild(Statement *stmt);
        // adds a child to an AND layer node (child will hold postulate i.e. option to prove statement in current AND node)
        virtual LPTNode *addAndLayerChild(Postulate *instGamma);

    };


    // expands a node on the LPT, returns true on success or false on failure (all nodes closed/true)
    bool expand(bool exhaustive, TraverseType strategy);
    // propagates all truth values - to be invoked after any truth value has changed in the tree
    // to ensure that the affect of this change is visible in the entire tree
    void propagateTruthValues();
    // returns the next node for expansion
    LPTNode *getNextNode(TraverseType strategy, bool exhaustive);
    // returns the next node for BFS expansion
    //LPTNode *getNextBFSNode(bool exhaustive);
    // returns the next node for DFS expansion
    //LPTNode *getNextDFSNode(bool exhaustive);

    // evaluates whether postulate is already used in the same instantiation on path from root to current node.
    // if yes, then node should be marked as violate termination
    bool isPostulateOnPathFromRoot(LPTNode *currentNode, Postulate *postulate);
    // pointer to used logic
    Logic *l;
    // attributes for protocol and individual statement verification
    // lists for assumptions, steps and goals
    StatementPtrList assumptions;
    StatementPtrList steps;
    StatementPtrList goals;
    // list of all statements known to be true
    StatementPtrList sigma;
    // root of LPT
    LPTOrNode root;
    // flag to indicate whether all nodes are either true or closed
    bool allNodesTrueClosed;
    // list off all nodes in tree
    LPTNodeList allNodes;

};

}
#endif // LAYEREDPROVINGTREE_H
