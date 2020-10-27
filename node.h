#ifndef NODE_H
#define NODE_H

#include <QChar>
#define MAX 100

/* 节点 */
struct State
{
    int stateName;
};

/* 边 */
struct Edge
{
    State startState;   //开始节点
    State endState; //结束节点
    QChar transSymbol;   //转换符，空的用#
};

/* 单元 */
struct Cell
{
    Edge edgeSet[MAX];
    int edgeCount;
    State startState;
    State endState;
};

#endif // NODE_H
