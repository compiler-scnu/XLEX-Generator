#include "subsetconstruction.h"
#include <QQueue>

/* 经过ε转换可得到的集合 */
QList<int> closure(QList<int> list, Graph nfa)
{
    QList<int> closureList = list;

    //BFS
    QQueue<int> queue;  //存放nfa节点
    for(int i=0; i<list.length(); i++)
    {
        queue.append(closureList[i]);
        while(!queue.empty())
        {
            int row = queue.dequeue();
            for(int col=1; col<=nfa.vertexNum; col++)
            {
                if(nfa.edges[row][col] == "ε" && !closureList.contains(col))
                {
                    closureList.append(col);
                    if(row != col)
                    {
                        queue.append(col);
                    }
                }
            }
        }
    }
    return closureList;
}

/* 通过转换符ch可到达的NFA状态的集合 */
QList<int> doMove(QList<int> list, QChar transSymbol, Graph nfa)
{
    QList<int> moveList;
    for(auto row : list)
    {
        for(int col=1; col<=nfa.vertexNum; col++)
        {
            if(nfa.edges[row][col] == transSymbol && !list.contains(col))
                moveList.append(col);
        }
    }
    return moveList;
}

/* 将NFA确定化为DFA */
Graph definite(Graph NFA)
{
    Graph DFA = Graph();

    QList<int> startList;
    startList.append(NFA.start);    //从开始状态开始做ε闭包操作

    QList<QString> transSymbolList = NFA.transSymbolList;
    QList<int> startClosure = closure(startList, NFA);  //得到开始状态的ε闭包
    int stateName = 1;  //DFA的状态名
    int endState = NFA.endStateList[0]; //拿到终态

    QQueue<QList<int>> queue;
    queue.append(startClosure);

    QMap<int, QList<int>> map;
    map.insert(stateName++, startClosure);

    while(!queue.empty())
    {
          QList<int> list = queue.dequeue();

          for(auto transSymbol : transSymbolList)
          {
              QList<int> newList = closure(doMove(list, transSymbol[0], NFA), NFA);
              if(!newList.isEmpty())
              {
                  if(!listIsSame(list, newList))  //产生了新状态
                  {
                      queue.append(newList);
                      map.insert(stateName++, newList);
                      if(newList.contains(endState))    //终态
                      {
                          DFA.endStateList.append(stateName-1);
                      }
                  }
                  DFA.edges[getKey(map, list)][getKey(map, newList)] = transSymbol;
              }
          }
    }

    DFA.vertexNum = stateName - 1;
    DFA.start = 1;
    DFA.map = map;

    return DFA;
}

/* 两个列表是否相同 */
bool listIsSame(QList<int> list1, QList<int> list2)
{
    if(list1.length() != list2.length())
        return false;

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());
    for(int i = 0; i<list1.length(); i++)
    {
        if(list1[i] != list2[i])
            return false;
    }

    return true;
}

/* 根据value得到key */
int getKey(QMap<int, QList<int>> map, QList<int> value)
{
    for(auto key : map.keys())
    {
        if(map[key] == value)
            return key;
    }
    return -1;
}
