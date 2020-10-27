#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thompson.h"
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

QStringList MainWindow::getTitle(QString regularExpression)
{
    QList<QString> list;
    QChar ch;
    int length = regularExpression.length();

    for(int i=0; i<length; i++)
    {
        ch = regularExpression.at(i);
        if(ch.isLetterOrNumber())
            list.append(ch);
    }

    list = QSet<QString>(list.begin(), list.end()).values();
    list.sort();
    list.append("ε");
    return list;
}


void MainWindow::createTable(QString regularExpression)
{
    QString addSymbolExpression = addJoinSymbol(regularExpression);
    QString postfixExpression = postfix(addSymbolExpression);
    Cell cell = postfixExpressToNFA(postfixExpression);

    int rowCount = cell.endState.stateName; //获取节点数，即行数

    /* 获取转换符标题 */
    QStringList titleList = getTitle(regularExpression);

    /* 创建表格视图*/
    QTableView *tableView = new QTableView;
    tableView->resize(this->width(), this->height());
    tableView->setWindowTitle("NFA状态表");

    /* 创建数据模型 */
    QStandardItemModel* model = new QStandardItemModel();

    /* 设置表格标题行 */
    model->setHorizontalHeaderLabels(titleList);

    /* 自适应所有列，让它布满空间 */
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int colCount = titleList.length();

    for (int i = 0; i < rowCount; i++)
    {
        for(int j=0; j <colCount; j++)
        {
            model->setItem(i, j, new QStandardItem(QString("Φ")));
            model->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }

    for(int i=0; i < cell.edgeCount; i++)
    {
        QString transSymbol = cell.edgeSet[i].transSymbol;  //得到当前边的转换符
        int row = cell.edgeSet[i].startState.stateName; //当前边开始状态的名字，即表格中的"行号+1"
        //转换符在表格中的列号
        int col = transSymbol == "#" ? titleList.indexOf("ε") : titleList.indexOf(transSymbol);
        QString item = model->item(row-1, col)->text(); //得到表格中的值
        if(item == "Φ")
        {

            item = QString::number(cell.edgeSet[i].endState.stateName); //原本为空直接添加
        }
        else
        {
            item = item + ", " + QString::number(cell.edgeSet[i].endState.stateName);   //多个状态拼接
        }

        model->setItem(row-1, col, new QStandardItem(item));
        model->item(row-1, col)->setTextAlignment(Qt::AlignCenter);
    }

    /* 设置表格视图数据 */
    tableView->setModel(model);

    /* 显示 */
    tableView->show();
}


void MainWindow::on_confirmBtn_clicked()
{

    QString re = "a+";
    createTable(re);

}
