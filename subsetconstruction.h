#ifndef SUBSETCONSTRUCTION_H
#define SUBSETCONSTRUCTION_H
/*
 * 子集构造法
 * 将NFA转为DFA
*/
#include "node.h"

/* 构造DFA */
Graph definite(Graph);    //【子集法】将NFA确定化为DFA
QList<int> closure(QList<int>, Graph);   //ε-closure闭包运算：某个状态集经过任意多个ε，得到当前的真正状态集
QList<int> doMove(QList<int>, QChar, Graph);  //当前状态集通过某个字母可以转化到的下一状态集（一步转换，没有进行ε-闭包运算）

/* 工具函数 */
bool listIsSame(QList<int>, QList<int>);    //两个列表是否相同
int getKey(QMap<int, QList<int>>, QList<int>);  //通过value找key

#endif // SUBSETCONSTRUCTION_H
