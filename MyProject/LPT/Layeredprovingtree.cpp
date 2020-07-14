#include <cstdlib>
#include <iostream>

#include <QMessageBox>

#include "LPTDefinitions.h"
#include "LPTContainers.h"
#include "LPTList.h"
#include "LPTQueue.h"
#include "LPTStack.h"
#include "Layeredprovingtree.h"
#include "LPTExceptions.h"
#include "../BAN/banlogicimpl.h"
#include "../GNY/gnylogic.h"

namespace LPT {

// define static members:
LayeredProvingTree *LayeredProvingTree::LPTNode::lpt = nullptr;
int LayeredProvingTree::LPTNode::nodeCount = 0;


void LayeredProvingTree::printInfo() {
    using namespace std;
    cout << "Assumpions: \n";
    for (int i=0; i<assumptions.count(); ++i) {
        cout << "\t" + assumptions.at(i)->getString().toStdString() << endl;
    }
    cout << "Steps: \n";
    for (int i=0; i<steps.count(); ++i) {
        cout << "\t" + steps.at(i)->getString().toStdString() << endl;
    }
    cout << "goals: \n";
    for (int i=0; i<goals.count(); ++i) {
        cout << "\t" + goals.at(i)->getString().toStdString() << endl;
    }

    cout << "Root children: \n";
    for (int i=0; i<root.children.count(); ++i) {
        cout << "\t" + root.children.at(i)->stmt->getString().toStdString() << endl;
    }

    cout << "-------------------------------------------\n\n";
}

LayeredProvingTree::LayeredProvingTree(ImplementedLogics logic)
    :allNodesTrueClosed(false)
{
    LPTNode::lpt = this;
    switch (logic){
    case ImplementedLogics::GNY:
        l = new GNY::GNYLogic();
        l->setLPT(this);
        break;
    case ImplementedLogics::BAN:
        l = new BANLogic::BANLogicImpl();
        l->setLPT(this);
        break;
    default:
        l = nullptr;
    }
}

LayeredProvingTree::LayeredProvingTree(ImplementedLogics logic, StatementPtrList &assumptions,
                                       StatementPtrList &steps, StatementPtrList &goals)
    :allNodesTrueClosed(false)
{
    LPTNode::lpt = this;
    switch (logic){
    case ImplementedLogics::GNY:
        l = new GNY::GNYLogic();
        l->setLPT(this);
        break;
    case ImplementedLogics::BAN:
        l = new BANLogic::BANLogicImpl();
        l->setLPT(this);
        break;
    default:
        l = nullptr;
    }
    initialise(assumptions, steps, goals);
}

LayeredProvingTree::~LayeredProvingTree()
{
    reset();
    if (l != nullptr) delete l;
}

/*
 * initialises layered proving tree
 * firstly, removes any potential information from previous runs
 * then:
 *    - copies assumptions, steps and goals
 *    - setup lpt root
 *
 * param:
 * assumptions - list of assumptions
 * steps - list of steps
 * goals - list of goals
 */
void LayeredProvingTree::initialise(StatementPtrList &assumptions, StatementPtrList &steps, StatementPtrList &goals)
{
    // make sure all lists are empty
    reset();
    // copy lists from parameter to attributes
    this->assumptions.append(assumptions);
    this->steps.append(steps);
    this->goals.append(goals);

    // add assumptions and steps to known statements
    sigma.append(assumptions);
    sigma.append(steps);

    // add goals as ANDChild to root
    for (int i=0; i<goals.count(); ++i) {
        root.children.append(new LPTAndNode(&root, goals.at(i)));
    }
    // set root truth value
    root.truthValue = TruthValue::UNKNOWN;
}

/*
 * resets the layered proving tree, i.e. removes all information from previous runs
 */
void LayeredProvingTree::reset()
{
    // delete all lpt nodes
    int i;
    for (i=0; i<allNodes.count(); ++i) {
        delete allNodes.at(i);
    }
    // clear roots child list and reset truth value to unknown
    root.children.clear();
    root.truthValue = TruthValue::UNKNOWN;

    // clear all lists
    assumptions.clear();
    steps.clear();
    goals.clear();
    sigma.clear();
    allNodes.clear();

}

// starts the verification process with the given properties.
bool LayeredProvingTree::verifyProtocol(bool exhaustive, TraverseType strategy)
{
    try {
        while ( (!allNodesTrueClosed && exhaustive)
                || (root.truthValue != TruthValue::TRUE && !allNodesTrueClosed) ) {
            expand(exhaustive, strategy);
            propagateTruthValues();
        }
    } catch (LPTException lpte) {
        QString msg = "Verification process prematurely terminated!!!!\n\n\nReason: ";
        msg += lpte.getMessage();
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", msg);
    } catch (...) {
        QString msg = "Verification process prematurely terminated!!!!\n\n\nUnrecognised exception caused termination";
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", msg);
    }

    return root.truthValue==TruthValue::TRUE?true:false;
}

/*
 * epands the layered proving tree.
 * this picks one node (which should be in an AND layer), gets all matching postulates from the logic,
 * gets all possible instantiations for each matching postulate and adds them as a child to the picked node.
 *
 * param:
 * exhautive: true: will attempt to prove each statement in the tree, even if parent statements are already true.
 *            false: only proves minimum number of statements to prove individual statements.
 * strategy: selected strategy for picking the next node
 *
 * algorithm:
    // nextNode = get next node (according to strategy)
    // if nextNode == nullptr
    //    set alltrueclosed to true
    // else
    //    nextNode->isExtended = true;
    //    logic->getMatchingPostulates(nextNode->stmt, matchingPostList)
    //    for each p in matchingPostList
    //       logic->getAllInstantiatedPostulates(p, instantiatedPostList)
    //        for each ip in instantiatedPostList
    //           nextNode->addChild(ip)
 */
bool LayeredProvingTree::expand(bool exhaustive, TraverseType strategy)
{
    // nextNode = get next node (according to strategy)
    LPTNode *nextNode = getNextNode(strategy, exhaustive);
    // if nextNode == nullptr
    if (nextNode == nullptr) {
        allNodesTrueClosed = true;
    } else {
        // nextNode->isExtended = true;
        nextNode->isExtended = true;

        Postulate *p;
        Postulate *instantiatedP;
        PostulatePtrList matchingPostList;
        PostulatePtrList instantiatedPostList;
        l->getMatchingPostulates(nextNode->stmt, matchingPostList);
        // for each p in matchingPostList
        for (int i=0; i<matchingPostList.count(); ++i) {
            p = matchingPostList.at(i);
            // logic->getAllInstantiatedPostulates(p, instantiatedPostList)
            l->getAllInstantiatedPostulates(p, instantiatedPostList);
            // for each ip in instantiatedPostList
            for (int j=0; j<instantiatedPostList.count(); ++j) {
                instantiatedP = instantiatedPostList.at(j);
                nextNode->addAndLayerChild(instantiatedP);
            }
        }
    }
    return nextNode==nullptr?false:true;
}

/*
 *  propagates all truth values - to be invoked after any truth value has changed in the tree
 *  to ensure that the affect of this change is visible in the entire tree.
 *
 * algorithm:
 * create list levelOrderList of all nodes (except root) in level order
 * bool truthHasChanged = true
 * while (truthHasChanged = true)
 *    truthHasChanged = false
 *    for each node current in levelOrderList from last to first
 *       if current has truthValue true
 *          do nothing
 *       else
 *          if current is cross referenced to otherNode
 *             current-truthValue = otherNode->truthValue
 *          else
 *             if current->layerType = ANDLayer
 *                current->truthValue = AND children->truthValue
 *             else current->layerType = ORLayer
 *                current->truthValue = OR children->truthValue
 *             if (current->truthValue = true
 *                 sigma.append(current)
 *                 truthHasChanged = true;
 * root.truthValue = AND root->children
 */
void LayeredProvingTree::propagateTruthValues()
{
    LPTNodeList levelOrderList;
    bool truthHasChanged = true;
    LPTNode *current;
    TruthValue truth;

    // fill levelOrderList with all nodes in level order
    LPTQueue<LPTNode *> nodeQueue;
    // add all children of root to nodeQueue
    for (int i=0; i<root.children.count(); ++i) {
        nodeQueue.enqueue(root.children.at(i));
    }
    while (!nodeQueue.isEmpty()) {
        current = nodeQueue.dequeue();
        // add all children of current to nodeQueue
        for (int i=0; i<current->children.count(); ++i) {
            nodeQueue.enqueue(current->children.at(i));
        }
        // append current to levelOrderList
        levelOrderList.append(current);
    }
    while (truthHasChanged == true) {
        truthHasChanged = false;
        bool wasUnkown = false;
        // for each node current in levelOrderList from last to first
        for (int i=levelOrderList.count(); i>0; --i) {
            current = levelOrderList.at(i-1);
            if (current->truthValue == TruthValue::UNKNOWN) {
                wasUnkown = true;
                // if current is cross referenced to otherNode
                //    current-truthValue = otherNode->truthValue
                if (current->crossRefTarget != nullptr) {
                    current->truthValue = current->crossRefTarget->truthValue;
                } else {
                    if (current->layerType == LinkType::ANDLink) {
                        // truth value is AND of all children truth values
                        truth = TruthValue::TRUE;
                        for (int j=0; j<current->children.count(); ++j) {
                            truth = truth && current->children.at(j)->truthValue;
                        }
                        current->truthValue = truth;
                    } else if (current->layerType == LinkType::ORLink) {
                        // truth value is OR of all children truth values
                        truth = TruthValue::UNKNOWN;
                        for (int j=0; j<current->children.count(); ++j) {
                            truth = truth || current->children.at(j)->truthValue;
                        }
                        current->truthValue = truth;
                    } else {
                        throw LPTException("Unknown link type encoutered in propagateTruthValues");
                    }
                }
            }
            if (wasUnkown && current->truthValue == TruthValue::TRUE) {
                sigma.append(current->stmt);
                current->stmtIndexInSigma = sigma.count()-1;
                truthHasChanged = true;
            }
        }
    }
    // root.truthValue = AND root->children
    truth = TruthValue::TRUE;
    for (int i=0; i<root.children.count(); ++i) {
        truth = truth && root.children.at(i)->truthValue;
    }
    root.truthValue = truth;
}


/*
 * returns the next node for expansion.
 *
 * param:
 * strategy - strategy for finding next node
 * exhaustive - true: search until all nodes are expanded, false: search until tree is proven (if single node
 * in OR layer is true, stop expanding other siblings)
 */
LayeredProvingTree::LPTNode *LayeredProvingTree::getNextNode(TraverseType strategy, bool exhaustive) {
    LPTNode *nextNode = nullptr;
    LPTNode *current = nullptr;
    LPTNode *child;
    LPTStack<LPTNode *> nodeStack;
    LPTQueue<LPTNode *> nodeQueue;
    LPTList<LPTNode *> nodeList;
    LPTList<int> indexList;

    int count, index, i;
    switch (strategy) {
    case TraverseType::DFS:
        // push root
        nodeStack.push(&root);
        // as long as stack is not empty
        while (!nodeStack.isEmpty()) {
            //    pop node current from stack
            current = nodeStack.pop();
            //    if current is ANDNode then it is not suitable for expansion if it is marked true, already extended or marked as
            //          violating termination
            if (current->layerType == LinkType::ANDLink) {

                // if (current is suitable for expansion) - TruthValue=Unknown && !isExtended && !violatesTermination
                if (current->truthValue==TruthValue::UNKNOWN || !current->isExtended || !current->violatesTerminationRules) {
                    nextNode = current;
                    break;
                } else {
                    // current is not suitable for extension
                    // if exhaustive then push each child of n (right to left!) onto stack
                    // otherwise only push if current node is unknown
                    if (exhaustive || current->truthValue==TruthValue::UNKNOWN) {
                        for (i=current->children.count(); i>0; --i) {
                            nodeStack.push(current->children.at(i-1));
                        }
                    }
                }

            } else {  // current is OR layer node
                // OR layer nodes are never used for expansion
                // if exhaustive is true push all children that are not yet true and not yet extended onto stack
                // if exhautive is false, only push if current node is not true
                if (exhaustive || current->truthValue!=TruthValue::TRUE) {
                    // push each child of n (right to left!) onto stack that is not true/extended
                    for (i=current->children.count(); i>0; --i) {
                        child = current->children.at(i-1);
                        // if exhautive is false, only push child if it is not yet true, otherwise push always
                        if ( child->truthValue!=TruthValue::TRUE || exhaustive) {
                            nodeStack.push(child);
                        }
                    }
                }
            }
        }
        break;

    case TraverseType::INVDFS:
        // push root
        nodeStack.push(&root);
        // as long as stack is not empty
        while (!nodeStack.isEmpty()) {
            //    pop node current from stack
            current = nodeStack.pop();
            //    if current is ANDNode then it is not suitable for expansion if it is marked true, already extended or marked as
            //          violating termination
            if (current->layerType == LinkType::ANDLink) {

                // if (current is suitable for expansion) - TruthValue=Unknown && !isExtended && !violatesTermination
                if (current->truthValue==TruthValue::UNKNOWN || !current->isExtended || !current->violatesTerminationRules) {
                    nextNode = current;
                    break;
                } else {
                    // current is not suitable for extension
                    // if exhaustive then push each child of n (right to left!) onto stack
                    // otherwise only push if current node is unknown
                    if (exhaustive || current->truthValue==TruthValue::UNKNOWN) {
                        count = current->children.count();
                        for (i=0; i<count; ++i) {
                            nodeStack.push(current->children.at(i));
                        }
                    }
                }

            } else {  // current is OR layer node
                // OR layer nodes are never used for expansion
                // if exhaustive is true push all children that are not yet true and not yet extended onto stack
                // if exhautive is false, only push if current node is not true
                if (exhaustive || current->truthValue!=TruthValue::TRUE) {
                    // push each child of n (right to left!) onto stack that is not true/extended
                    count = current->children.count();
                    for (i=0; i<count; ++i) {
                        child = current->children.at(i-1);
                        // if exhautive is false, only push child if it is not yet true, otherwise push always
                        if ( child->truthValue!=TruthValue::TRUE || exhaustive) {
                            nodeStack.push(child);
                        }
                    }
                }
            }
        }
        break;
    case TraverseType::BFS:
        /*// put root
        nodeQueue.enqueue(&root);
        // as long as queue is not empty
        while (!nodeQueue.isEmpty()) {
        //    get node n from stack
            current = nodeQueue.dequeue();
        //    if n is not suitable for expansion
            if (current->isExtended || current->violatesTerminationRules) {
        //      put each child of n (left to right!) onto queue
                count = current->children.count();
                for (i=0; i>count; ++i) {
                    nodeQueue.enqueue(current->children.at(i));
                }
            } else {
        //    else
        //       nextNode = n
        //       terminate loop
                nextNode = current;
                break;
            }
        }*/
        break;
    case TraverseType::INVBFS:
        /*// put root
        nodeQueue.enqueue(&root);
        // as long as queue is not empty
        while (!nodeQueue.isEmpty()) {
        //    get node n from stack
            current = nodeQueue.dequeue();
        //    if n is not suitable for expansion
            if (current->isExtended || current->violatesTerminationRules) {
                //      push each child of n (right to left!) onto stack
                for (i=current->children.count(); i>0; --i) {
                    nodeQueue.enqueue(current->children.at(i-1));
                }
            } else {
        //    else
        //       nextNode = n
        //       terminate loop
                nextNode = current;
                break;
            }
        }*/
        break;
    case TraverseType::RANDOM:
        // create list with indices
        for (i=allNodes.count(); i>0; --i) indexList.append(i-1);

        // while indexlist is not empty
        while ( (count = indexList.count()) > 0) {
            //    pick random index
            index = indexList.at(rand()%count);
            current = allNodes.at(index);
            //    if allNodes[index] is not suitable for expansion
            if (current->isExtended || current->violatesTerminationRules) {
                //       remove index from indexList
                indexList.removeOne(index);
            } else {
                //    else
                //       nextNode = allNodes[index]
                //       terminate loop
                nextNode = current;
                break;
            }
        }
        break;
    default:
        throw new LPTException("Search Strategy not implemented yet");
    }

    return nextNode;
}



////////////////////////////////////////////////////////////////////////////
// LPTNode Stuff
////////////////////////////////////////////////////////////////////////////



/*
 * Default constructor for LPTNodes. Sets up generic base class component.
 */
LayeredProvingTree::LPTNode::LPTNode()
    : parent(nullptr), crossRefTarget(nullptr), stmt(nullptr), stmtIndexInSigma(-1),
      isRoot(false), isExtended(false), violatesTerminationRules(false), truthValue(TruthValue::UNKNOWN)
{
    // increase nodeCount and use updated value as ID for node
    nodeID = ++nodeCount;
}

LayeredProvingTree::LPTNode::~LPTNode()
{

}

/*
 * Constructor for LPTAndNode. LPTAndNode is used in an AND layer - this means that all children must be true for
 * the node to become true.
 *
 * Nodes in ANDLayers store statements (which usually are prerequisites of the parent node in the previous OR layer).
 *
 * Children hold the available Postulate options that may be used to establish truth of this node's statement.
 *
 * Nodes in an AND layer are picked for expansion - i.e. LayeredProvingTree::expand will pick an AND node (via nextNextxxxNode)
 * and epxand it.
 *
 * param:
 * nodeParent - pointer to parent node of new node
 * nodeStmt - pointer to statement that is to be stored in new node
 * ifRootNode - true: new node is root of LPT, false otherwise
 */
LayeredProvingTree::LPTAndNode::LPTAndNode(LPTNode *nodeParent, Statement *nodeStmt)
{
    layerType = LinkType::ANDLink;
    parent = nodeParent;
    stmt = nodeStmt;

    // check if statement is already used anywhere in tree
    crossRefTarget = lpt->allNodes.getNodeByStatement(stmt);
    if (crossRefTarget != nullptr) {
        // increase counter in target node for references to it
        crossRefTarget->incrementRefCount();
        // truth value of this node is same as from referenced node
        truthValue = crossRefTarget->getTruthValue();
        // this node should never be extended -> mark as already extended
        isExtended = true;
    }
    lpt->allNodes.append(this);
}

LayeredProvingTree::LPTAndNode::~LPTAndNode()
{
    lpt->allNodes.removeOne(this);
}

/*
 * method for LPTOrNode - should never be invoked on this node - throw exception
 */
void LayeredProvingTree::LPTAndNode::addOrLayerChild(Statement *stmt) {
    throw new IllegalMethodException("addOrLayerChild called on ANDLayer node. Parameter stmt is " +
                                     stmt->getString());

}

/*
 * adds a child to an AND layer node (child will hold postulate i.e. option to prove statement in current AND node). Will
 * also add all prerequisites of postulate to resulting child.
 */
LayeredProvingTree::LPTNode *LayeredProvingTree::LPTAndNode::addAndLayerChild(Postulate *instGamma) {
    // create new child node and append to children list
    LPTNode *child = new LPTOrNode(this, instGamma);
    children.append(child);

    // add all prerequisites as children to new child
    StatementPtrList &prerequisites = instGamma->getPrerequisites();
    int prereqCount = prerequisites.count();
    for (int i=0; i<prereqCount; ++i) {
        child->addOrLayerChild(prerequisites.at(i));
    }

    return child;
}


/* Default constructor  - creates root node
 */
LayeredProvingTree::LPTOrNode::LPTOrNode()
{
    isRoot = true;
}

/*
 * Constructor for LPTOrNode. LPTOrNode is used in an OR layer - this means that only one child must be true for
 * the node to become true.
 *
 * Nodes in OrLayers store postulates (which usually are options that can be used to prove the statement in parent
 * AND layer node).
 *
 * Children hold statments that represent prerequisites of the postulate.
 *
 * Nodes in an OR layer are never picked for expansion.
 *
 * param:
 * nodeParent - pointer to parent node of new node
 * nodeStmt - pointer to statement that is to be stored in new node
 * ifRootNode - true: new node is root of LPT, false otherwise
 */
LayeredProvingTree::LPTOrNode::LPTOrNode(LPTNode *nodeParent, Postulate *post)
{
    layerType = LinkType::ORLink;
    parent = nodeParent;
    postulateDetail = post->getString();
    stmt = post->getGoal();
    lpt->allNodes.append(this);
}

LayeredProvingTree::LPTOrNode::~LPTOrNode()
{
    lpt->allNodes.removeOne(this);
}

/*
 * adds a child to an OR layer node (child will hold statement i.e. prerequisite of current OR layer node).
 */
void LayeredProvingTree::LPTOrNode::addOrLayerChild(Statement *stmt) {
    // create new child node and append to children list
    LPTNode *child = new LPTAndNode(this, stmt);
    children.append(child);

}

/*
 * method for LPTAndNode - should never be invoked on this node - throw exception
 */
LayeredProvingTree::LPTNode *LayeredProvingTree::LPTOrNode::addAndLayerChild(Postulate *instGamma) {
    throw new IllegalMethodException("addAndLayerChild called on OrLayer node. Parameter instGamma is " +
                                     instGamma->getString());
}

/*
 * logical and of truth values
 * only true && true = true
 * only false && false = false
 * unknown && x or x && unknown = unknown
 */
TruthValue operator&&(TruthValue first, TruthValue second)
{
    TruthValue result = TruthValue::UNKNOWN;
    if (first == TruthValue::TRUE && second == TruthValue::TRUE) {
        result = TruthValue::TRUE;
    } else if (first == TruthValue::FALSE && second == TruthValue::FALSE) {
        result = TruthValue::FALSE;
    }
    return result;
}

/*
 * logical or of truth values (symmetric)
 * true || x = true
 * false || false = false
 * false || unkown = false
 * unknown || unknown = unknown
 */
TruthValue operator||(TruthValue first, TruthValue second)
{
    TruthValue result = TruthValue::UNKNOWN;
    if (first == TruthValue::TRUE || second == TruthValue::TRUE) {
        result = TruthValue::TRUE;
    } else if (first == TruthValue::FALSE || second == TruthValue::FALSE) {
        result = TruthValue::FALSE;
    }
    return result;
}

}
