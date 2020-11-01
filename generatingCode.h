#ifndef GENERATINGCODE_H
#define GENERATINGCODE_H

#include "node.h"

QString getNodeCode(Graph, int); //得到一个结点的代码块
QList<QString> getCode(Graph);  //得到各个结点的代码块

#endif // GENERATINGCODE_H
