#include "NFA.h"
#include <QStack>

int STATE_NUM = 1;
QChar symbolArray[] = {'(', ')', '*', '+', '|'};

/* 将NFA单元转换为NFA图 */
Graph toNFAGraph(Cell nfa)
{
    Graph NFAGraph = Graph(nfa.endState.stateName);

    NFAGraph.startStateList.append(nfa.startState.stateName);
    NFAGraph.endStateList.append(nfa.endState.stateName);

    //构建nfa图结构
    for(int i=0; i<nfa.edgeCount; i++)
    {
        int start = nfa.edgeSet[i].startState.stateName;
        int end = nfa.edgeSet[i].endState.stateName;
        QString transSymbl = nfa.edgeSet[i].transSymbol;

        NFAGraph.edges[start][end].append(transSymbl);

        if(!NFAGraph.transSymbolList.contains(transSymbl))
            NFAGraph.transSymbolList.append(transSymbl);
    }

    return NFAGraph;
}

/* 后缀表达式转为NFA */
Cell postfixExpressToNFA(QString postfixExpression)
{
    Cell cell, left, right;
    QStack<Cell> cellStack;

    for(auto element : postfixExpression)
    {
        switch (element.unicode())
        {
        case '|':
            right = cellStack.pop();
            left = cellStack.pop();
            cell = doUnite(left, right);
            cellStack.push(cell);
            break;
        case '*': case '+':
            left = cellStack.pop();
            cell = doClosure(left, element);
            cellStack.push(cell);
            break;
        case '_':
            right = cellStack.pop();
            left = cellStack.pop();
            cell = doJoin(left, right);
            cellStack.push(cell);
            break;
        default:
            cell = doCell(element);
            cellStack.push(cell);
        }   //end switch
    }   //end for

    cell = cellStack.pop();
    STATE_NUM = 1;
    return cell;
}

/* 或运算处理，a|b */
Cell doUnite(Cell left, Cell right)
{
    Cell newCell;
    newCell.edgeCount = 0;
    Edge edge1, edge2, edge3, edge4;

    //获得新的状态节点
    State startState = newStateNode();
    State endState = newStateNode();

    //构建边
    edge1.startState = startState;
    edge1.endState = left.startState;
    edge1.transSymbol = "ε";

    edge2.startState = startState;
    edge2.endState = right.startState;
    edge2.transSymbol = "ε";

    edge3.startState = left.endState;
    edge3.endState = endState;
    edge3.transSymbol = "ε";

    edge4.startState = right.endState;
    edge4.endState = endState;
    edge4.transSymbol = "ε";

    //构建单元
    //先将left和right的edgeSet复制到newCell
    copyCellEdgeSet(newCell, left);
    copyCellEdgeSet(newCell, right);

    //将新构建的四条边加入edgeSet
    newCell.edgeSet[newCell.edgeCount++] = edge1;
    newCell.edgeSet[newCell.edgeCount++] = edge2;
    newCell.edgeSet[newCell.edgeCount++] = edge3;
    newCell.edgeSet[newCell.edgeCount++] = edge4;

    //构建newCell的开始状态和结束状态
    newCell.startState = startState;
    newCell.endState = endState;

    return newCell;
}

/* 连接运算处理，ab */
Cell doJoin(Cell left, Cell right)
{
    Cell newCell;
    newCell.edgeCount = 0;
    Edge newEdge;

    //构建边
    newEdge.startState = left.endState;
    newEdge.endState = right.startState;
    newEdge.transSymbol = "ε";

    //先将left和right的edgeSet复制到newCell
    copyCellEdgeSet(newCell, left);
    copyCellEdgeSet(newCell, right);

    //将新构建的边加到edgeSet
    newCell.edgeSet[newCell.edgeCount++] = newEdge;

    //构建newCell的开始状态和结束状态
    newCell.startState = left.startState;
    newCell.endState = right.endState;

    return newCell;
}

/* 闭包处理，a* 或 a+ */
Cell doClosure(Cell cell, QChar op)
{
    Cell newCell;
    newCell.edgeCount = 0;
    Edge edge1, edge2, edge3;

    //获得新的新状态节点
    State startState = newStateNode();
    State endState = newStateNode();

    //构建边
    edge1.startState = startState;
    edge1.endState = cell.startState;
    edge1.transSymbol = "ε";

    edge2.startState = cell.endState;
    edge2.endState = cell.startState;
    edge2.transSymbol = "ε";

    edge3.startState = cell.endState;
    edge3.endState = endState;
    edge3.transSymbol = "ε";

    //构建单元
    //先将cell的edgeSet复制到newCell
    copyCellEdgeSet(newCell, cell);

    //将新构建的边加入edgeSet
    newCell.edgeSet[newCell.edgeCount++] = edge1;
    newCell.edgeSet[newCell.edgeCount++] = edge2;
    newCell.edgeSet[newCell.edgeCount++] = edge3;

    //*闭包可以出现空，即多一条边
    if(op == '*')
    {
        Edge edge4;
        edge4.startState = startState;
        edge4.endState = endState;
        edge4.transSymbol = "ε";
        newCell.edgeSet[newCell.edgeCount++] = edge4;
    }

    //构建newCell的开始状态和结束状态
    newCell.startState = startState;
    newCell.endState = endState;

    return newCell;
}

/* 单词处理，a */
Cell doCell(QChar element)
{
    Cell newCell;
    newCell.edgeCount = 0;
    Edge newEdge;

    //获得新的状态节点
    State startState = newStateNode();
    State endState = newStateNode();

    //构建边
    newEdge.startState = startState;
    newEdge.endState = endState;
    newEdge.transSymbol = element;

    //构建单元
    newCell.edgeSet[newCell.edgeCount++] = newEdge;
    newCell.startState = startState;
    newCell.endState = endState;

    //edgeCount此时为1
    return newCell;
}

/* 将一个单元的边的集合复制到另一个单元 */
void copyCellEdgeSet(Cell& destination, Cell source)
{
    int destinationCount = destination.edgeCount;
    int sourceCount = source.edgeCount;

    for(int i=0; i<sourceCount; i++)
    {
        destination.edgeSet[destinationCount+i] = source.edgeSet[i];
    }

    destination.edgeCount = destinationCount + sourceCount;
}

/*
 * 获得新的状态节点，统一产生，便于管理，不能产生重复的状态
 * 并添加到stateSet[]数组中
*/
State newStateNode()
{
    State newState;
    newState.stateName = STATE_NUM++;

    return newState;
}

/*检测输入的正则表达式是否合法 */
bool checkLegal(QString regularExpression)
{
    return checkCharacter(regularExpression) && checkParenthesis(regularExpression) && checkString(regularExpression);
}

/* 检查输入的字符是否合适 () * | a~z A~Z 1~9 合法返回true,非法返回false */
bool checkCharacter(QString regularExpression)
{
    for(auto check : regularExpression)
    {
        if(!check.isLetterOrNumber() && !inSymbolArray(check))
            return false;
    }

    return true;
}

/* 检查括号是否匹配 合法返回true 非法返回false */
bool checkParenthesis(QString regularExpression)
{
    QStack<QChar> stack;

    for(auto ch : regularExpression)
    {
        if(ch == '(')
        {
            stack.push(ch);
        }
        else if(ch == ')')
        {
            if(stack.empty())
                return false;
            else
                stack.pop();
        }
    }

    if(!stack.empty())
        return false;

    return true;
}

/* 检查串的样式是否合法 */
bool checkString(QString re)
{
    if(re.isEmpty())
        return false;
    if(!re.at(0).isLetterOrNumber())
        return false;
    if(re.at(re.length()-1) == '|')
        return false;
    for(int i=1; i<re.length()-1; i++)
    {
        if(!re.at(i).isLetterOrNumber() && !re.at(i+1).isLetterOrNumber())
        {

            if(re.at(i+1) == '(')
                continue;
            if(re.at(i) == ')' && re.at(i+1) == ')')
                continue;

            return false;
        }
    }
    return true;
}

/* 判断是否在操作符数组中 */
bool inSymbolArray(QChar check)
{
    for(auto symbol : symbolArray)
    {
        if(check == symbol)
            return true;
    }
    return false;
}

/* 显式添加交操作符“_”，便于中缀转后缀表达式 例如 abb->a_b_b */
QString addJoinSymbol(QString regularExpression)
{
    int length = regularExpression.length();
    QString newExpression = "";
    QChar first, second;

    if(length == 1)
        return regularExpression;

    for(int i=0; i<length-1; i++)
    {
        first = regularExpression.at(i);
        second = regularExpression.at(i+1);
        newExpression.append(first);

        //若第二个是字母、第一个不是'('、'|'都要添加
        if(first != '(' && first != '|' && second.isLetterOrNumber())
            newExpression.append('_');
        //若第二个是'(',第一个不是'|'、'(',也要加
        else if(second == '(' && first != '|' && first != '(')
            newExpression.append('_');
    }   //end for

    //将最后一个字符写入
    newExpression.append(second);

    return newExpression;
}

/* 优先级
 *     # ( * + | _ )
 * isp 0 1 7 7 3 5 8
 * icp 0 8 6 6 2 4 1
 */

/* 栈内优先级 */
int isp(QChar c)
{
    switch (c.unicode())
    {
        case '#': return 0;
        case '(': return 1;
        case '*': case '+': return 7;
        case '|': return 3;
        case '_': return 5;
        case ')': return 8;
    }

    return false;
}

/* 栈外优先级 */
int icp(QChar c)
{
    switch (c.unicode())
    {
        case '#': return 0;
        case '(': return 8;
        case '*': case '+': return 6;
        case '|': return 2;
        case '_': return 4;
        case ')': return 1;
    }

    return false;
}

/* 中缀表达式转后缀表达式 */
QString postfix(QString newExpression)
{
    //设定newExpression的最后一个符号式“#”，而其“#”一开始先放在栈stack的栈底
    newExpression.append('#');
    QStack<QChar> stack;
    QChar ch = '#', top, op;
    stack.push(ch);

    //读一个字符
    QString postfixExpression = "";
    int index = 0;
    ch = newExpression.at(index++);

    while(!stack.empty())
    {
        if(ch.isLetterOrNumber())
        {
            postfixExpression.append(ch);
            ch = newExpression.at(index++);
        }
        else    //ch为操作符
        {
            top = stack.top();

            if(icp(ch) > isp(top))  //压栈
            {
                stack.push(ch);
                ch = newExpression.at(index++);
            }
            else if(icp(ch) < isp(top)) //退栈并添加到后缀表达式中
            {
                op = stack.pop();
                postfixExpression.append(op);
            }
            else    //退栈但不添加到后缀表达式
            {
                op = stack.pop();
                if(op == '(')
                    ch = newExpression.at(index++);
            }   //end if
        }   //end if
    }   //end while

    return postfixExpression;
}
