#ifndef MiNIMIZEDFA_H
#define MiNIMIZEDFA_H

#include "node.h"

Graph toMinimizeDFA(Graph); //得到最小化DFA
QList<QList<int>> transform(QList<int>, Graph);    //得到转换后的状态
QList<QList<int>> splitStateList(QList<int>, Graph);   //划分状态组
bool compareListSame(QList<int>, QList<int>);
bool isClosure(int, QList<int>);

#endif // MiNIMIZEDFA_H
