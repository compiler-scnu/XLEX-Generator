#include "generatingCode.h"

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
