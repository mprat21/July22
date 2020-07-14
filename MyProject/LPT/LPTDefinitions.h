#ifndef LPTDEFINITIONS_H
#define LPTDEFINITIONS_H

namespace LPT {
class Logic;
class Postulate;
class Statement;
class LayeredProvingTree;


enum class ImplementedLogics {GNY, RDCS, BAN};
enum class TraverseType {BFS, DFS, INVBFS, INVDFS, RANDOM};
enum class LinkType {ANDLink, ORLink};
enum class TruthValue {FALSE, TRUE, UNKNOWN};

TruthValue operator&&(TruthValue first, TruthValue second);
TruthValue operator||(TruthValue first, TruthValue second);

}
#endif // LPTDEFINITIONS_H
