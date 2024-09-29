#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "class2.h"
#include <iostream>
#include <QLineEdit>
#include <QString>
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

void MainWindow::on_page1_clicked()
{
    //m_pWidget1 = new QtWidgetsClass1(ui.class1);
    this ->close();
    class2 *pic = new class2();
    pic->show();
}

QVector<uchar> hexStringToByteArray(const QString &hexString) {
    QVector<uchar> byteArray;
    // 使用空格分割字符串
    QStringList hexParts = hexString.split(' ');
    for (const QString &hexPart : hexParts) {
        bool ok;
        // 尝试将十六进制字符串转换为uchar
        uchar byte = hexPart.toUInt(&ok, 16);
        if (ok) {
            // 如果转换成功，则添加到byteArray中
            byteArray.append(byte);
        } else {
            // 如果转换失败（例如，因为"08"），则可以选择跳过或记录错误
            qDebug() << "Invalid hex part:" << hexPart;
            // 注意：在这个例子中，我们选择了简单地跳过无效部分
        }
    }
    return byteArray;
}

QByteArray hexStringToByteArray_1(const QString &hexString) {
    QByteArray result;
    // 假设hexString是由空格分隔的十六进制字符串组成，如"00 01 02 ..."
    QStringList hexParts = hexString.split(' ');
    foreach (const QString &hexPart, hexParts) {
        bool ok;
        // 尝试将字符串转换为十六进制的byte（注意：这里忽略了前缀0和非法的十六进制字符）
        uchar byte = hexPart.toUInt(&ok, 16);
        if (ok) {
            result.append(byte);
        } else {
            // 处理非法输入，这里只是简单地跳过
            qDebug() << "Invalid hex part:" << hexPart;
        }
    }
    return result;
}

void MainWindow::on_BY001_button_clicked()
{
    QString hexText = ui->main_input->toPlainText();
    qDebug() << "The text in the line edit is:" << hexText;

    QVector<uchar> byteVector = hexStringToByteArray(hexText); // 转换为字节数组

    // 现在，byteVector包含了转换后的字节，你可以按需使用它们
    // 例如，打印出来查看
    uchar check_result = 0x00;

    //qDebug() << std::hex << static_cast<int>(check_result);

    for (uchar byte : byteVector) {
       // qDebug() << std::hex << static_cast<int>(byte); // 使用std::hex以十六进制形式打印
        check_result = ((byte + check_result) & (0xff));
    }
   // qDebug() << std::hex << static_cast<int>(check_result);

    // 将check_result转换为带有0x前缀的十六进制字符串
    QString hexString = QString("0x%1").arg(check_result, 2, 16, QChar('0'));
    // 注意：%1是占位符，用于插入arg()中的值；2表示最小宽度（如果需要的话会用0填充）；16表示基数（十六进制）；
    // QChar('0')是填充字符，但在这个情况下，由于我们指定了最小宽度为2（对于单个字节足够），它通常不会被使用。

    // 使用qDebug打印带有0x前缀的十六进制字符串
    qDebug() << hexString;

    // 将带有0x前缀的十六进制字符串设置到QLineEdit中
    ui->main_output->setText(hexString);

}

