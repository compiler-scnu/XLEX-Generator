#include <QQueue>
#include "hopcroft.h"
#include "subsetconstruction.h"
#include<QDebug>

/* 得到最小化DFA */
Graph toMinimizeDFA(Graph DFA)
{
    //Graph minimizeDFA = Graph(DFA.vertexNum);

    QList<int> terminalStates = DFA.startStateList;   //接收态组  {1, 2, 3}
    QList<int> nonTerminalStates = DFA.endStateList;  //非接收态组 {4}

    QList<QList<int>> startGroup = splitStateList(terminalStates, DFA); //分好组的接收态
    QList<QList<int>> endGroup = splitStateList(nonTerminalStates, DFA);    //分好组的非接收态

    QList<QString> transSymbolList = DFA.transSymbolList;//状态列表
    QMap<int, QList<int>> map;

    //构建map
    int key = 1;
    for(int i=0; i<startGroup.length(); i++)
    {
        if(!startGroup[i].empty())
            map[key++] = startGroup[i];
    }
    for(int i=0; i<endGroup.length(); i++)
    {
        if(!endGroup[i].empty())
            map[key++] = endGroup[i];
    }

    Graph minimizeDFA = Graph(key-1);

    //接受态遍历
    if(!startGroup[0].empty())
    {
        for(int i=0; i<startGroup.length(); i++)
        {
            for(auto transSymbol: transSymbolList)//接受态字符遍历
            {
                int start = startGroup[i][0];//取接受态列表第一个为新名称
                for(int j=1;j<=DFA.vertexNum;j++)//去原DFA图中遍历节点
                {
                    if(DFA.edges[start][j]==transSymbol)//找到转换符对应的终点
                    {
                        for(auto list:startGroup)//在接受态组中找到终点对应的新列表
                        {
                            if(list.contains(j))
                            {
                                //getKey(map, startGroup[i]);  startState对应的名字
                                //getKey(map, list);  endState对应的名字
                                minimizeDFA.edges[getKey(map, startGroup[i])][getKey(map, list)] = transSymbol;
                            }
                        }
                        for(auto list:endGroup)//在非接受态组中找到终点对应的新列表
                        {
                            if(list.contains(j))
                            {
                                minimizeDFA.edges[getKey(map, startGroup[i])][getKey(map, list)] = transSymbol;
                            }
                        }
                    }
                }
            }
        }
    }

    //非接受态遍历
    if(!endGroup[0].empty())
    {
        for(int i=0; i<endGroup.length(); i++)
        {
            for(auto transSymbol: transSymbolList)//接受态字符遍历
            {
                int start = endGroup[i][0];//取非接受态列表第一个为新名称
                for(int j=1;j<=DFA.vertexNum;j++)//去原DFA图中遍历节点
                {
                    if(DFA.edges[start][j]==transSymbol)//找到转换符对应的终点
                    {
                        for(auto list:startGroup)//在接受态组中找到终点对应的新列表
                        {
                            if(list.contains(j))
                            {
                                //getKey(map, startGroup[i]);  startState对应的名字
                                //getKey(map, list);  endState对应的名字
                                minimizeDFA.edges[getKey(map, endGroup[i])][getKey(map, list)] = transSymbol;
                            }
                        }
                        for(auto list:endGroup)//在非接受态组中找到终点对应的新列表
                        {
                            if(list.contains(j))
                            {
                                minimizeDFA.edges[getKey(map, endGroup[i])][getKey(map, list)] = transSymbol;
                            }
                        }
                    }
                }
            }
        }
    }

    //非接受态遍历*同上*
    minimizeDFA.map = map;
    minimizeDFA.transSymbolList = transSymbolList;

    return minimizeDFA;
}

/* 转换操作 放一个列表 得到该状态经过转换后的新状态
 * {1, 2, 3}
 * DFA状态    a   b   c
 *   1        2   3   4
 *   2        1   4   5
 *   3        2   3   4     ->  {1, 3}, {2}, {4} -> {1, 3}, {2, 4}
 *   4        3   4   5
 * return [[2, 3, 4], [1, 4, 5], [3, 4, 5]] ,两个相同的列表即为同一个状态, 但两个不同的列表不一定是两个状态，也有可能是同一个状态
 * return [[2, 3, 4], [1, 4, 5]] 
 */
QList<QList<int>> transform(QList<int> stateList, Graph DFA)
{
    QList<QString> transSymbolList = DFA.transSymbolList;
    QList<QList<int>> transMatrix;

    for(auto state: stateList)
    {
        QList<int> transList;
        for(auto transSymbol: transSymbolList)
        {
            bool flag = true;
            for(int i=1; i<=DFA.vertexNum; i++)
            {
                if(DFA.edges[state][i] == transSymbol)
                {
                    transList.append(i);
                    flag = false;
                }
            }
            if(flag)
            {
                transList.append(0);
            }
        }
        transMatrix.append(transList);
    }

    return transMatrix;
}

/* 划分状态组 转换状态前的列表 */
QList<QList<int>> splitStateList(QList<int> stateList, Graph DFA)
{
    QList<QList<int>> splitMatrix;  //存放每个分组

    if(stateList.length() <= 1)
    {
        splitMatrix.append(stateList);
        return splitMatrix;
    }

    QList<QList<int>> transMatrix = transform(stateList, DFA);   //转换后的状态列表

    QList<int> numList;

    for(int i=0; i<transMatrix.length(); i++)
    {
        QList<int> groupList;   //存放同一个组的状态

        for(int j=i+1; j<transMatrix.length(); j++)
        {
            if(compareListSame(transMatrix[i], transMatrix[j]) && !numList.contains(j))
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

bool compareListSame(QList<int> l1, QList<int> l2)
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

