













# 一、实验内容

设计一个应用软件，以实现将正则表达式-->NFA--->DFA-->DFA最小化-->词法分析程序

# 二、实验目的

 （1）要提供一个正则表达式的输入界面，让用户输入正则表达式（可保存、打开保存着正则表达式的文件）

 （2）需要提供窗口以便用户可以查看转换得到的NFA（用状态转换表呈现即可）

 （3）需要提供窗口以便用户可以查看转换得到的DFA（用状态转换表呈现即可）

 （4）需要提供窗口以便用户可以查看转换得到的最小化DFA（用状态转换表呈现即可）

 （5）需要提供窗口以便用户可以查看转换得到的词法分析程序（该分析程序需要用C语言描述）

 （6）应该书写完善的软件文档

# 三、实验文档

实现项目分析、设计

实验项目分为四个部分：

## 1、将正则表达式的中缀形式转化为后缀形式

### 1.1 划分运算符优先级（分为栈内优先级与栈外优先级）

将连接运算加上，“_”符号作为连接运算符

```c++
/* 优先级
 *      # (  * + |  _  )
 * isp 0 1 7 7 3 5 8
 * icp 0 8 6 6 2 4 1
 */
```

### 1.2 利用栈结构将中缀形式的正则表达式转化为后缀表达式

```c++
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
```



## 2、将后缀形式的正则表达式转化为NFA

### 2.1 构建NFA单元

```c++
/* 后缀转NFA */
Graph toNFAGraph(Cell);  //将NFA单元转换为图结构
Cell postfixExpressToNFA(QString);  //后缀表达式转为NFA
Cell doUnite(Cell, Cell);   //或运算处理，a|b
Cell doJoin(Cell, Cell);    //连接运算处理，ab
Cell doClosure(Cell, QChar);   //闭包处理，a* 或 a+
Cell doCell(QChar);  //单词处理，a
void copyCellEdgeSet(Cell&, Cell);  //将一个单元的边的集合复制到另一个单元
State newStateNode();   //产生一个新的状态节点，便于管理
```

### 2.2 构建NFA图

依次对NFA栈中各单元进行连接运算，构建出NFA图结构。

![7de632697509ec4820d2b831c05b159](C:\Users\cjk\Desktop\7de632697509ec4820d2b831c05b159.jpg)

![微信图片_20201101201517](C:\Users\cjk\Desktop\微信图片_20201101201517.jpg)

![微信图片_20201101201525](C:\Users\cjk\Desktop\微信图片_20201101201525.jpg)



## 3、将NFA转化为DFA

### 3.1 构建ε转换（ε-closure）

```c
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
                if(nfa.edges[row][col] == "ε" && !closureList.contains(col))
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
```

### 3.2 构建指定转换符转换（ch-closure）

```c++
/* 通过转换符ch可到达的NFA状态的集合 */
QList<int> doMove(QList<int> list, QChar transSymbol, Graph nfa)
{
    QList<int> moveList;
    for(auto row : list)
    {
        for(int col=1; col<=nfa.vertexNum; col++)
        {
            if(nfa.edges[row][col] == transSymbol && !moveList.contains(col))
                moveList.append(col);
        }
    }
    return moveList;
}
```

### 3.3 将NFA确定化为DFA

```c++
struct Graph
{
    QList<QString> edges[MAX][MAX]; //保存边的转换符
    int vertexNum; //顶点数量
    QList<int> startStateList;  //初态集合
    QList<int> endStateList;    //dfa的终态集合
    QList<QString> transSymbolList;   //转换符列表
    QMap<int, QList<int>> map;  //状态名向集合的映射，A -> {1, 2, 3}
```



### 3.4 举例实现

一个NFA如图所示：

![img](https://www.pianshen.com/images/697/839c84d5d4204081e4fad6eaf8b1cab9.JPEG)

构造转化表的算法如下：

1、 从NFA的初始状态开始，把从初始状态跳空能到的状态以及该状态跳空能到的状态，设为T0，放到表格的第二行第一列，直到没有新的状态能够由已得到的状态跳空得到为止。T0={ 0,1,2,4,7 }。

2、第二列的状态分别对应NFA图来分别跳a，跳a后的状态填在第二列（Ia列）。第二行第二列由第二行第一列的T0状态跳a得到。T0集合的元素跳a可得到的元素{3, 8}，以及该元素{3,8}跳空可得到的元素设为T1，放到表格第二行第二列。 T1={1,2,3,4,6,7,8}。

3、跳b的情况也与跳a的情况类似，把第二步的跳a改成跳b即可。

4、经过2、3步得到的状态分别填在第一列，用2-3的步骤生成经跳a、跳b得到的状态分别填在Ia列Ib列。

5、重复2-4步。

6、当表中的所有状态都出现在第一列时说明表的构造完成。

构造完成的表如下：

![img](https://www.pianshen.com/images/858/17f9b822096afaea3f48442efc465bca.JPEG)

 

含0的为初态，含10的为终态，重新命名各个状态，令T0、T1、T2、T3、T4用0、1、2、3、4分别表示，该DFA的状态转换图如图所示：

![img](https://www.pianshen.com/images/489/cf9c90694e41b97f96a35c4fb3e77669.JPEG)

## 4、实现DFA最小化（合并法）

### 4.1 得到初始状态转换矩阵

```C++
 DFA状态       a   b   c
	1         2   3   4
	2         3   4   5
	3         2   3   4     ->  {1, 3}, {2}, {4} -> {1, 3}, {2, 4}
	4         1   4   5
```

采用状态逐个比较的方法来判定状态是否等价。从初始状态开始，逐个比较其他哪一个状态在输入字母表中的某一个字符时与初始状态转换到相间的状态。如果不能找到这样的状态，则表明没有与初始状态等价的状态。如果能够找到这样的状态,则再检查当输入其他字符时，这两个状态是否还能转换到相同的状态，如果对于所有的输入字符，两个状态都转换到相同的状态,则这两个状态等价,然后再重复检査其他的状态两两是否等价。检査完一遍后,进行状态合并，将两个等价状态合并为一个, 同时修改连接到这些状态的有向边。状态合并完成后,再迭代执行上述过程进行状态合并后新 DFA 状态的等价性判断和状态合并，直到在状态合并后的新 DFA 中找不到等价的状态为止。

 以 上述矩阵为例，从初始状态1开始，按上述算法检查后，发现状态1与状态3等价。重复这一过程，最终发现只有一对状态等价，为状态1和状态3。然后进行状态合并，设状态合并后的 DFA 为 Mˊ（将状态1和状态3合并为状态**1ˊ**）则Mˊ的转换矩阵如下所示。

 ```C++
DFA状态       a   b    c
	1ˊ       2   1ˊ   4
	2        1ˊ  4    5
	4        1   4    5
 ```

重复该过程查找上述矩阵中的等价状态并进行合并，最终得到的DFA设为Mˊˊ（将状态2和状态4合并为状态2ˊ），则Mˊˊ的状态转换矩阵如下所示。

```C++
DFA状态       a   b    c
	1ˊ       2   1ˊ   2ˊ
	2ˊ       1ˊ  2ˊ   5
```

### 4.2 代码实现

```C++
Graph toMinimizeDFA(Graph); //得到最小化DFA
QList<QList<int>> transform(QList<int>, Graph);    //得到状态转换矩阵
QList<QList<int>> splitStateList(QList<int>, Graph);   //划分状态组，其中有重写矩阵操作
```

## 5、由最小化DFA转化为词法分析程序

### 5.1 分析

根据文法分析课程的内容，舍弃了用`while()`循环读取字符的方法，改为使用左递归来实现。

### 5.2 生成一个结点的代码

```C++
QString getNodeCode(Graph minimizeDFA, int state)//获取一个节点的代码块
{
    QString codeBlock;
    QString line;

    line = QString("void Node%1()\n").arg(state);
    codeBlock.append(line);
    codeBlock.append("{\n");
    codeBlock.append("\tch == getchar();\n");

    for(int i=1; i<=minimizeDFA.vertexNum;i++)
    {
        QList<QString> transSymbolList = minimizeDFA.edges[state][i];

        if(!transSymbolList.empty())  //转换符不为空
        {
            line = QString("\tif(ch == '%1'").arg(transSymbolList[0]);
            for(int j=1; j<transSymbolList.length(); j++)
            {
                line = line + QString(" || ch == '%1'").arg(transSymbolList[j]);
            }
            line = line + ")\n\t{\n";
            codeBlock.append(line);

            line = QString("\t\tprintf(%c, %1);\n").arg("ch");
            codeBlock.append(line);

            line = QString("\t\tNode%1();\n\t}\n").arg(i);
            codeBlock.append(line);
        }
    }
    line=QString("\treturn;\n}\n");
    codeBlock.append(line);

    return codeBlock;
}
```

### 5.3 得到整个图的代码

```C++
QList<QString> getCode(Graph minimizeDFA)
{
    QList<QString> codeList;   //所有代码
    QString code;//接受每个函数代码

    codeList.append("#include <stdio.h>\n\n");  //添加头文件
    codeList.append("void main()\n{\n\tNode1();\n}\n"); //添加主函数
    for(int i=1; i<=minimizeDFA.vertexNum; i++)
    {
        code = getNodeCode(minimizeDFA, i);
        codeList.append(code);
    }
    return codeList;
}
```

### 5.4 生成的代码如下 （正则表达式为 (a|b)* )

```C
#include <stdio.h>

void main()
{
	Node1();
}

void Node1()
{
	ch == getchar();
	if(ch == 'a' || ch == 'b')
	{
		printf(%c, ch);
		Node1();
	}
	return;
}
```
