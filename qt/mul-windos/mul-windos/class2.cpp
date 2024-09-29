#include "class2.h"
#include "ui_class2.h"
#include "mainwindow.h"

class2::class2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::class2)
{
    ui->setupUi(this);
}

class2::~class2()
{
    delete ui;
}

void class2::on_pushButton_clicked()
{
    this ->close();
    MainWindow *main_win = new MainWindow();
    main_win->show();
}

