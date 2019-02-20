#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QString>
#include <QTimer>


namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();

    void sendDate();

private slots:
    void on_pushButton_chose_clicked();

    void on_pushButton_send_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *serversock;
    QTcpSocket *connectsock;

    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;

    QTimer timer;
};

#endif // WIDGET_H
