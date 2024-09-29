#ifndef CLASS2_H
#define CLASS2_H

#include <QWidget>

namespace Ui {
class class2;
}

class class2 : public QWidget
{
    Q_OBJECT

public:
    explicit class2(QWidget *parent = nullptr);
    ~class2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::class2 *ui;
};

#endif // CLASS2_H
