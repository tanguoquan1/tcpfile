#include "serverwidget.h"
#include "ui_serverwidget.h"

#include <QHostAddress>
#include <QString>
#include <QByteArray>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>


ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    //指定父对象，分配空间
    serversock = new QTcpServer(this);

    //监听端口
    serversock->listen(QHostAddress::Any,8888);

    //设置按键不能被按下
    ui->pushButton_chose->setEnabled(false);
    ui->pushButton_send->setEnabled(false);

    setWindowTitle("服务器端口为：8888");

    //如果有客户端连接进来
    connect(serversock, QTcpServer::newConnection,
            [=]()
            {
        //取出套接字
        connectsock = serversock->nextPendingConnection();

        //获取对方的IP和端口号
        QString IP = connectsock->peerAddress().toString();
        qint16 port = connectsock->peerPort();

        //显示成功连接
        QString str = QString("[%1 : %2]成功连接").arg(IP).arg(port);
        ui->textEdit->setText(str);

        //成功连接后才能打开文件
        ui->pushButton_chose->setEnabled(true);

        //监听客户端消息
        connect(connectsock, &QTcpSocket::readyRead,
                [=]()
                {
                    //获取客户端传送的信息
                    QByteArray buffer = connectsock->readAll();

                    //判断是否接收完毕
                    if(QString(buffer) == "file done")
                    {
                        //文件接收完毕后
                        ui->textEdit->append("文件接收完毕");
                        file.close();

                        //断开客户端
                        connectsock->disconnectFromHost();
                        connectsock->close();
                    }
                }
                );

            }
            );

    connect(&timer, &QTimer::timeout,
            [=]()
            {
                timer.stop();
                 sendDate();
            }
            );

}

ServerWidget::~ServerWidget()
{
    delete ui;
}
void ServerWidget::sendDate()
{
    ui->textEdit->append("正在发送数据");

    qint64 len = 0;
    do
    {
        //存放消息体
        char buffer[4 * 1024] = {0};

        //将文件的内容读取到buffer里面
        len = file.read(buffer,sizeof(buffer));

        //发送信息
        len = connectsock->write(buffer,len);

       ui->textEdit->append("发送");
        //记录已经发送的数据大小
        sendSize +=len;
        qDebug() <<sendSize;

    }while(len > 0);

}

void ServerWidget::on_pushButton_chose_clicked()
{
    //获取文件路径
    QString filepath = QFileDialog::getOpenFileName(this,"open","../");

    //如果文件路径是有效的
    if(false == filepath.isEmpty())
    {
        fileName.clear();

        sendSize = 0;
        fileSize = 0;

       //获取文件信息
        QFileInfo info(filepath);
        fileName = info.fileName();
        fileSize = info.size();

        //指定文件
        file.setFileName(filepath);

        //打开文件
        bool isok = file.open(QIODevice::ReadOnly);
        if(isok == false)
        {
            qDebug() << "打开文件失败";
        }

        //提示打开的文件路径
        ui->textEdit->append(filepath);

        ui->pushButton_chose->setEnabled(false);
        ui->pushButton_send->setEnabled(true);
    }
}

void ServerWidget::on_pushButton_send_clicked()
{
    ui->pushButton_send->setEnabled(false);

    //组装一个文件头，以##分开
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);

    //传输数据
    qint64 len = connectsock->write(head.toUtf8());

    if(len > 0)
    {
        timer.start(100);
    }
    else
    {
        qDebug() << "发送头失败";
        file.close();

        ui->pushButton_chose->setEnabled(true);
        ui->pushButton_send->setEnabled(false);
    }
}
