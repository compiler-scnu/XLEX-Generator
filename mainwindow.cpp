#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NFA.h"
#include "DFA.h"
#include "minimizeDFA.h"
#include "generatingCode.h"
#include <QErrorMessage>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "error.h"
#include "ui_error.h"

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
    tableView->resize(850, 500);
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
                if(FA.edges[i][j].contains(title))
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
    QString re = ui->regularText->toPlainText();    //拿到正则表达式
    NFA = Graph();
    DFA = Graph();
    minimizeDFA = Graph();

    if(!checkLegal(re))
    {
        //QErrorMessage *error = new QErrorMessage(this);
        error *e = new error();

        QImage* img=new QImage;
        QString fileName = "./images/a.png";


        e->getUI()->webLabel->setText(tr("<a href = \"https://www.runoob.com/regexp/regexp-syntax.html\">点击这里，学习正则表达式的书写</a>"));
        e->getUI()->webLabel->setOpenExternalLinks(true);

        img->load(fileName);    //加载图像
        e->getUI()->imageLabel->setPixmap(QPixmap::fromImage(*img));
        e->setWindowTitle("正则表达式都能打错？");
        e->setWindowModality(Qt::ApplicationModal);
        e->show();
    }
    else
    {
        NFA = toNFAGraph(postfixExpressToNFA(postfix(addJoinSymbol(re))));    //生成NFA
        DFA = toDFA(NFA);   //生成DFA
        minimizeDFA = toMinimizeDFA(DFA);   //生成最小化DFA
    }
}

void MainWindow::on_showNFABtn_clicked()
{
    if(NFA.vertexNum != 0)
    {
        QStringList nfaTitleList = getTitle(NFA, "NFA");    //NFA表格标题
        createTable(NFA, nfaTitleList, "NFA");
    }
}

void MainWindow::on_showDFABtn_clicked()
{
    if(DFA.vertexNum != 0)
    {
        QStringList dfaTitleList = getTitle(DFA, "DFA");    //DFA表格标题
        createTable(DFA, dfaTitleList, "DFA");
    }
}

void MainWindow::on_showMiniMizeDFABtn_clicked()
{
    if(minimizeDFA.vertexNum != 0)
    {
        QStringList minimizeDFATitleList = getTitle(minimizeDFA, "minimizeDFA");    //最小化DFA表格标题
        createTable(minimizeDFA, minimizeDFATitleList, "minimizeDFA");
    }
}

void MainWindow::on_showCodeBtn_clicked()
{
    ui->codeText->clear();
    if(minimizeDFA.vertexNum != 0)
    {
        for(auto code: getCode(minimizeDFA))
        {
            ui->codeText->append(code);
        }
    }
}

void MainWindow::on_openFileBtn_clicked()
{
    //---获取文件名
    QString fileName = QFileDialog :: getOpenFileName(this, NULL, NULL, "*.txt");

    //---打开文件并读取文件内容
    QFile file(fileName);

    //--打开文件成功
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text))
    {
        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            ui->regularText->setText(textStream.readLine());
        }
    }
    else	//---打开文件失败
    {
        //        QMessageBox ::information(NULL, NULL, "open file error");
    }

}

void MainWindow::on_saveRegularBtn_clicked()
{
    QString re = ui->regularText->toPlainText();
    if(re.isEmpty())
    {
        QMessageBox ::information(NULL, NULL, "没有正则表达式，保存失败！");
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存当前正则表达式"),
                                                        "",
                                                        tr("正则表达式文件(*.txt)"));
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::critical(this, "critical", tr("文件保存失败！"),
                                  QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            QTextStream stream(&file);

            stream << re;
            stream.flush();

            file.close();
        }
    }
}

void MainWindow::on_saveCodeBtn_clicked()
{
    QString code = ui->regularText->toPlainText();
    if(code.isEmpty())
    {
        QMessageBox ::information(NULL, NULL, "没有代码，保存失败！");
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存当前代码"),
                                                        "",
                                                        tr("代码文件(*.txt *.c)"));
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::critical(this, "critical", tr("文件保存失败！"),
                                  QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            QTextStream stream(&file);

            stream << code;
            stream.flush();

            file.close();
        }
    }
}
