#ifndef HOPCROFT_H
#define HOPCROFT_H

#include "node.h"

Graph toMinimizeDFA(Graph); //得到最小化DFA
QList<QList<int>> transform(QList<int>, Graph);    //得到转换后的状态
QList<QList<int>> splitStateList(QList<int>, Graph);   //划分状态组
//QMap<int, QList<int>> splitStateListIfCould(QList<int>, QList<QString>);
bool compareListSame(QList<int>, QList<int>);

#endif // HOPCROFT_H
