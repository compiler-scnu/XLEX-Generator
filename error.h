#ifndef ERROR_H
#define ERROR_H

#include <QWidget>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class error;
}

class error : public QWidget
{
    Q_OBJECT

public:
    explicit error(QWidget *parent = nullptr);
    ~error();
    Ui::error *getUI();
private:
    Ui::error *ui;
};

#endif // ERROR_H
