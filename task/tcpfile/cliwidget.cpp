#include "cliwidget.h"
#include "ui_cliwidget.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

CliWidget::CliWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CliWidget)
{
    ui->setupUi(this);

    tcpCliSock = new QTcpSocket(this);

    isStart = true;

    //进度条
    ui->progressBar->setValue(0);

    setWindowTitle("客户端");

    connect(tcpCliSock,&QTcpSocket::readyRead,
            [=]()
            {
                QByteArray buffer = tcpCliSock->readAll();
                //qDebug() << "D" << buffer;
                //接收文件头
                if(isStart == true)
                {
                    filename = QString(buffer).section("##",0,0);

                    filesize = QString(buffer).section("##",1,1).toInt();
                    //qDebug() << "接收的文件大小为:"<<filesize;

                    recvsize = 0;
                    //关联文件名
                    file.setFileName(filename);

                    //以只写的方式打开文件
                    bool isok = file.open(QIODevice::WriteOnly);
                    if(isok == false)
                    {
                        qDebug() << "文件写入失败";

                        tcpCliSock->disconnectFromHost();
                        tcpCliSock->close();

                        return ;
                    }

                    //弹出对话框，显示接收信息
                    QString str = QString("接收的文件 ：[%1 : %2kb]")
                            .arg(filename)
                            .arg(filesize/1024);

                    QMessageBox::information(this,"文件信息",str);

                    //设置进度条
                    ui->progressBar->setMinimum(0);
                    ui->progressBar->setMaximum(filesize/1024);
                    ui->progressBar->setValue(0);

                    isStart = false;
                    //qDebug() <<"调试"<< isStart;
                }
                //接收文件体
                else
                {
                    //qDebug() << 55555;
                    qint64 len = file.write(buffer);

                    if(len > 0)
                    {
                        recvsize += len;
                        qDebug() << recvsize;
                    }
                    //更新进度条
                    ui->progressBar->setValue(recvsize/1024);
                   // qDebug() << buffer;
                    if(recvsize == filesize)
                    {
                    //先给服务器发送接收完毕消息
                    tcpCliSock->write("file done");

                    //提示
                    QMessageBox::information(this,"完成","文件接收完成");

                    //关闭文件
                    file.close();

                    //断开连接
                    tcpCliSock->disconnectFromHost();
                    tcpCliSock->close();

                    ui->progressBar->setValue(0);
                    }
                }
            }
            );
}

CliWidget::~CliWidget()
{
    delete ui;
}

void CliWidget::on_pushButton_clicked()
{
    //获取服务器IP和端口
    QString IP = ui->lineEditIP->text();
    qint64 port = ui->lineEdit_port->text().toInt();

    tcpCliSock->connectToHost(QHostAddress(IP),port);
    isStart = true;
}
