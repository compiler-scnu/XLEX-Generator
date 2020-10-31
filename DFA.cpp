#include "DFA.h"
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
                if(nfa.edges[row][col].contains("ε") && !closureList.contains(col))
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
            if(nfa.edges[row][col].contains(transSymbol) && !moveList.contains(col))
                moveList.append(col);
        }
    }
    return moveList;
}

/* 将NFA确定化为DFA */
Graph toDFA(Graph NFA)
{
    Graph DFA = Graph(NFA.vertexNum);

    /* 获取转换符集合 */
    QList<QString> nfaTransSymbolList = NFA.transSymbolList;
    nfaTransSymbolList.removeOne("ε");
    QList<QString> dfaTransSymbolList = nfaTransSymbolList;

    int endState = NFA.endStateList[0]; //拿到终态

    /* 获取初态的ε闭包 */
    QList<QList<int>> closureList;  //存储产生的ε闭包
    QList<int> startClosure = closure(NFA.startStateList, NFA);  //得到开始状态的ε闭包
    closureList.append(startClosure);

    QQueue<QList<int>> queue;
    queue.append(startClosure);

    int stateName = 1;  //DFA的状态名
    QMap<int, QList<int>> map;
    map.insert(stateName++, startClosure);
    if(startClosure.contains(endState))
    {
        DFA.endStateList.append(stateName-1);
    }
    else
    {
        DFA.startStateList.append(stateName-1);
    }


    while(!queue.empty())
    {
          QList<int> list = queue.dequeue();

          for(auto transSymbol : dfaTransSymbolList)
          {
              QList<int> newList = closure(doMove(list, transSymbol[0], NFA), NFA);
              if(!newList.isEmpty())
              {
                  if(!containList(newList, closureList))  //产生了新状态
                  {
                      queue.append(newList);
                      map.insert(stateName++, newList);
                      closureList.append(newList);
                      if(newList.contains(endState))    //终态
                      {
                          DFA.endStateList.append(stateName-1);
                      }
                      else
                      {
                          DFA.startStateList.append(stateName-1);
                      }
                  }
                  DFA.edges[getKey(map, list)][getKey(map, newList)].append(transSymbol);
              }
          }
    }

    DFA.vertexNum = stateName - 1;
    DFA.map = map;
    DFA.transSymbolList = dfaTransSymbolList;

    return DFA;
}

/* 列表是否在另一个列表中 */
bool containList(QList<int> list, QList<QList<int>> llist)
{
    for(auto l: llist)
    {
        if(compareList(l, list))
        {
            return true;
        }
    }
    return false;
}

/* 两个列表是否相同 */
bool compareList(QList<int> list1, QList<int> list2)
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
