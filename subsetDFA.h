#ifndef SUBSETDFA_H
#define SUBSETDFA_H

#include "node.h"

Cell definite(Cell);    //【子集法】将NFA确定化为DFA
QList<int> closure();   //ε-closure闭包运算：某个状态集经过任意多个ε，得到当前的真正状态集
QList<int> move();  //当前状态集通过某个字母可以转化到的下一状态集（一步转换，没有进行ε-闭包运算）

#endif // SUBSETDFA_H
