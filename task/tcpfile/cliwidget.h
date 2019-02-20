#ifndef CLIWIDGET_H
#define CLIWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>

namespace Ui {
class CliWidget;
}

class CliWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CliWidget(QWidget *parent = 0);
    ~CliWidget();

private slots:
    void on_pushButton_clicked();
//signals:


private:
    Ui::CliWidget *ui;

    QTcpSocket *tcpCliSock;

    QFile file;
    QString filename;
    qint64 filesize;
    qint64 recvsize;

    bool isStart;
};

#endif // CLIWIDGET_H
