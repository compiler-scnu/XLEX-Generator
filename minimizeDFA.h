#ifndef MiNIMIZEDFA_H
#define MiNIMIZEDFA_H

#include "node.h"

Graph toMinimizeDFA(Graph); //得到最小化DFA
QList<QList<int>> transform(QList<int>, Graph);    //得到转换后的状态矩阵
QList<QList<int>> splitStateList(QList<int>, Graph);   //划分状态组

//判断两个列表是否相同，形状要完成相同，即[1, 2 ,3]和[1, 3, 2]为不同列表
bool compareListSame(QList<int>, QList<int>);
bool isClosure(int, QList<int>);    //判断某一行是否为闭包  即  2  [0, 0, 2, 0] 形式

#endif // MiNIMIZEDFA_H
