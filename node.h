#ifndef NODE_H
#define NODE_H

#include <QChar>
#include <QList>
#include <QMap>
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
    QString transSymbol;   //转换符，空的用ε
};

/* 单元 */
struct Cell
{
    Edge edgeSet[MAX];
    int edgeCount;
    State startState;
    State endState;
};

/* 图 */
struct Graph
{
    QList<QString> edges[MAX][MAX]; //保存边的转换符  ----> QList<QString> edges[MAX][MAX]
    //QList<QString> minimizeDFAEdges[MAX][MAX];  //两个点之间有个多个转换
    int vertexNum; //顶点数量
    QList<int> startStateList;  //初态集合
    QList<int> endStateList;    //dfa的终态集合
    QList<QString> transSymbolList;   //转换符列表
    QMap<int, QList<int>> map;  //状态名向集合的映射，A -> {1, 2, 3}

    Graph()
    {
        for(int i=0; i<MAX; i++)
        {
            for(int j=0; j<MAX; j++)
            {
                this->edges[i][j].append("Φ");
            }
        }
    }

    Graph(int vertexNum)
    {
        this->vertexNum = vertexNum;
        for(int i=0; i<=vertexNum; i++)
        {
            for(int j=0; j<=vertexNum; j++)
            {
                this->edges[i][j].append("Φ");
            }
        }
    }
};

#endif // NODE_H
