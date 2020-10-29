#include <QQueue>
#include "hopcroft.h"

/* 得到最小化DFA */
Graph toMinimizeDFA(Graph DFA)
{
    Graph minimizeDFA = Graph(DFA.vertexNum);

    QList<int> terminalStates = DFA.startStateList;   //接收态组  {1, 2, 3}
    QList<int> nonTerminalStates = DFA.endStateList;  //非接收态组 {4}

    QList<QList<int>> startGroup = splitStateList(terminalStates, DFA);
    QList<QList<int>> endGroup = splitStateList(nonTerminalStates, DFA);

    for(int i=0; i<startGroup.length(); i++)
    {

    }


}

/* 转换操作 放一个列表 得到该状态经过转换后的新状态
 * {1, 2, 3}
 * DFA状态    a   b   c
 *   1        2   4   3
 *   2        0   4   3
 *   3        2   4   0     ->  {1, 4}, {2}, {3}
 *   4        2   4   3
 * return [[2, 4, 3], [0, 4, 3], [2, 4, 0]] ,两个相同的列表即为同一个状态
 */
QList<QList<int>> transform(QList<int> stateList, Graph DFA)
{
    QList<QString> transSymbolList = DFA.transSymbolList;
    QList<int> transList;
    QList<QList<int>> transMatrix;

    for(auto state: stateList)
    {
        for(int i=1; i<=DFA.vertexNum; i++)
        {
            for(auto transSymbol: transSymbolList)
            {
                if(DFA.edges[state][i] == transSymbol)
                {
                    transList.append(i);
                }
                else
                {
                    transList.append(0);
                }
            }
        }
        transMatrix.append(transList);
    }

    return transMatrix;
}

/* 划分状态组 转换状态前的列表 */
QList<QList<int>> splitStateList(QList<int> stateList, Graph DFA)
{
    QList<QList<int>> transMatrix = transform(stateList, DFA);   //转换后的状态列表

    QList<QList<int>> splitMatrix;  //存放每个分组
    QList<int> numList;

    for(int i=0; i<transMatrix.length(); i++)
    {
        QList<int> groupList;   //存放同一个组的状态

        for(int j=i+1; j<transMatrix.length(); j++)
        {
            if(compareList(transMatrix[i], transMatrix[j]) && !numList.contains(j))
            {
                if(!groupList.contains(stateList[i]))
                    groupList.append(stateList[i]);
                groupList.append(stateList[j]);
                numList.append(j);
            }
        }
        if(!groupList.empty())
        {
            splitMatrix.append(groupList);
        }
    }
    return splitMatrix;
}

bool compareList(QList<int> l1, QList<int> l2)
{
    if(l1.length() != l2.length())
        return false;
    for(int i=0; i<l1.length(); i++)
    {
        if(l1[i] != l2[i])
            return false;
    }
    return true;
}

