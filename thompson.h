#ifndef THOMPSON_H
#define THOMPSON_H

#include "node.h"

/* 函数声明 */
/* 检查合法性 */
bool checkLegal(QString);    //检查正则表达式的合法性
bool checkCharacter(QString);    //检查字符的合法性
bool checkParenthesis(QString); //检查括号是否匹配
bool inSymbolArray(QChar);    //判断是否在操作符数组中
bool checkString(QString);  //检查串是否合法

/* 中缀转后缀 */
QString addJoinSymbol(QString); //显式添加连接运算符'_'
QString postfix(QString);   //中缀转后缀
int isp(QChar);  //栈内优先级
int icp(QChar);  //栈外优先级

/* 后缀转NFA */
Graph toNFAGraph(Cell);  //将NFA单元转换为图结构
Cell postfixExpressToNFA(QString);  //后缀表达式转为NFA
Cell doUnite(Cell, Cell);   //或运算处理，a|b
Cell doJoin(Cell, Cell);    //连接运算处理，ab
Cell doClosure(Cell, QChar);   //闭包处理，a* 或 a+
Cell doCell(QChar);  //单词处理，a
void copyCellEdgeSet(Cell&, Cell);  //将一个单元的边的集合复制到另一个单元
State newStateNode();   //产生一个新的状态节点，便于管理

#endif // THOMPSON_H
