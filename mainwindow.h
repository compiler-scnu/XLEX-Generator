#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createTable(QString);
    QStringList getTitle(QString);
private slots:
    void on_confirmBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
