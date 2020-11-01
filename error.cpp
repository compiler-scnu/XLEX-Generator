#include "error.h"
#include "ui_error.h"

#include <QApplication>

error::error(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::error)
{
    ui->setupUi(this);
}

error::~error()
{
    delete ui;
}

Ui::error* error::getUI()
{
    return this->ui;
}

