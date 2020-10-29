#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thompson.h"
#include "subsetconstruction.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::getTitle(Graph FA, QString FAName)
{
    QList<QString> list = FA.transSymbolList;
    list.sort();

    if(FAName == "NFA")
    {
        list.append("ε");
    }

    return list;
}


void MainWindow::createTable(Graph FA, QStringList titleList, QString windowTitle)
{
    int rowCount = FA.vertexNum; //获取节点数，即行数


    /* 创建表格视图*/
    QTableView *tableView = new QTableView;
    tableView->resize(this->width(), this->height());
    tableView->setWindowTitle(windowTitle.append("状态转换表"));

    /* 创建数据模型 */
    QStandardItemModel* model = new QStandardItemModel();

    /* 设置表格标题行 */
    model->setHorizontalHeaderLabels(titleList);

    /* 自适应所有列，让它布满空间 */
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int colCount = titleList.length();

    for (int i = 1; i <= rowCount; i++)
    {
        for(int j = 1; j <= rowCount; j++)
        {
            for(auto title : titleList)
            {
                if(FA.edges[i][j] == title)
                {
                    model->setItem(i-1, titleList.indexOf(title), new QStandardItem(QString::number(j)));
                }
                else if(FA.edges[i][j] == '#')
                {
                    model->setItem(i-1, titleList.indexOf("ε"), new QStandardItem(QString::number(j)));
                }
                else
                {

                }
                model->setItem(i, j, new QStandardItem(QString("Φ")));
                model->item(i, j)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

//    for(int i=1; i <= rowCount; i++)
//    {
//        for(int j=1; j <= rowCount; j++)
//        {
//            QString transSymbol = FA.edges[i][j];  //得到当前边的转换符
//            //int row = cell.edgeSet[i].startState.stateName; //当前边开始状态的名字，即表格中的"行号+1"
//            //转换符在表格中的列号
//            int col = transSymbol == "#" ? titleList.indexOf("ε") : titleList.indexOf(transSymbol);
//            QString item = model->item(i-1, col)->text(); //得到表格中的值
//            if(item == "Φ")
//            {

//                //item = QString::number(cell.edgeSet[i].endState.stateName); //原本为空直接添加
//                item = QString::number(j);
//            }
//            else
//            {
//                //item = item + ", " + QString::number(cell.edgeSet[i].endState.stateName);   //多个状态拼接
//                item = item + ", " + QString::number(j);
//            }

//            model->setItem(row-1, col, new QStandardItem(item));
//            model->item(row-1, col)->setTextAlignment(Qt::AlignCenter);
//        }
//    }

    /* 设置表格视图数据 */
    tableView->setModel(model);

    /* 显示 */
    tableView->show();
}


void MainWindow::on_confirmBtn_clicked()
{
    QString re = "a|b";
    Graph NFA = toNFAGraph(postfixExpressToNFA(postfix(addJoinSymbol(re))));
    QStringList nfaTitleList = getTitle(re, "NFA");

    //createTable(NFA, nfaTitleList);

}
