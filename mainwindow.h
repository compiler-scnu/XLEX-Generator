#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QString>
#include "node.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createTable(Graph, QStringList, QString);  //创建状态转换表
    QStringList getTitle(Graph, QString);    //得到转换符列表并生成表格标题
private slots:
    void on_confirmBtn_clicked();

    void on_showNFABtn_clicked();

    void on_showDFABtn_clicked();

    void on_showMiniMizeDFABtn_clicked();

    void on_showCodeBtn_clicked();

    void on_openFileBtn_clicked();

    void on_saveRegularBtn_clicked();

    void on_saveCodeBtn_clicked();

private:
    Ui::MainWindow *ui;
    Graph NFA;
    Graph DFA;
    Graph minimizeDFA;
};
#endif // MAINWINDOW_H
