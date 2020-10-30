#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thompson.h"
#include "subsetconstruction.h"
#include "hopcroft.h"
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

/* 得到转换符列表 */
QStringList MainWindow::getTitle(Graph FA, QString FAName)
{
    QList<QString> list = FA.transSymbolList;
    list.sort();

    if(FAName == "DFA")
    {
        list.insert(0, "DFA状态");
        list.insert(0, "NFA状态");
        return list;
    }
    if(FAName == "minimizeDFA")
    {
        list.insert(0, "最小化DFA状态");
        list.insert(0, "DFA状态");
        return list;
    }

    list.insert(0, "状态");
    return list;
}

/* 创建状态转换表 */
void MainWindow::createTable(Graph FA, QStringList titleList, QString graphName)
{
    /* 创建表格视图 */
    QFont font;
    font.setBold(true);
    QTableView *tableView = new QTableView;
    tableView->resize(this->width(), this->height());
    tableView->setWindowTitle(graphName+"状态转换表");
    if(graphName == "minimizeDFA")
        tableView->setWindowTitle("最小化DFA状态转换表");
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(240, 240, 240);}");
    tableView->horizontalHeader()->setFont(font);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStandardItemModel* model = new QStandardItemModel();   //创建数据模型
    model->setHorizontalHeaderLabels(titleList);    //设置表格标题行

    /* 填充表格内容 */
    int rowCount = FA.vertexNum; //获取节点数，即行数
    for (int i = 1; i <= rowCount; i++)
    {
        for(auto title : titleList)
        {
            QString item = "";
            for(int j = 1; j <= rowCount; j++)
            {
                if(FA.edges[i][j] == title)
                {
                    item = (item.isEmpty()) ? QString::number(j) : (item + ", " + QString::number(j));
                }
            }

            if(item.isEmpty())
            {
                model->setItem(i-1, titleList.indexOf(title), new QStandardItem(QString("Φ")));
            }
            else
            {
                model->setItem(i-1, titleList.indexOf(title), new QStandardItem(item));
            }
            model->item(i-1, titleList.indexOf(title))->setTextAlignment(Qt::AlignCenter);

        }
    }

    /* 第一列状态属性填写 */
    for(int i=0; i<rowCount; i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::number(i+1)));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    }

    /* DFA填写前两列 */
    if(graphName == "DFA" || graphName == "minimizeDFA")
    {
        for(int i=0; i<rowCount; i++)
        {
            QString setStr = "";
            setStr.append("{ ");
            for(auto value: FA.map[i+1])
            {
                setStr = setStr + QString::number(value) + ", ";
            }
            setStr.replace(setStr.length()-2, setStr.length()-2, " }");
            model->setItem(i, 0, new QStandardItem(setStr));
            model->item(i, 0)->setTextAlignment(Qt::AlignCenter);

            model->setItem(i, 1, new QStandardItem(QString::number(i+1)));
            model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        }
    }

    /* 设置表格视图数据并展示 */
    tableView->setModel(model);
    tableView->show();
}


void MainWindow::on_confirmBtn_clicked()
{
    QString re = "(a|b)*c";
    Graph NFA = toNFAGraph(postfixExpressToNFA(postfix(addJoinSymbol(re))));
    Graph DFA = toDFA(NFA);
    Graph minimizeDFA = toMinimizeDFA(DFA);
    QStringList nfaTitleList = getTitle(NFA, "NFA");
    QStringList dfaTitleList = getTitle(DFA, "DFA");
    QStringList minimizeDFATitleList = getTitle(minimizeDFA, "minimizeDFA");

    createTable(NFA, nfaTitleList, "NFA");
    createTable(DFA, dfaTitleList, "DFA");
    createTable(minimizeDFA, minimizeDFATitleList, "minimizeDFA");
}
