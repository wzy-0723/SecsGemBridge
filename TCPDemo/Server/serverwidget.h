#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>//监听套接字
#include <QTcpSocket>//通信套接字

QT_BEGIN_NAMESPACE
namespace Ui {
class ServerWidget;
}
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget() override;
    void Init();

private slots:
    void on_buttonsend_clicked();
    void on_buttonclose_clicked();

private:
    Ui::ServerWidget *ui;
    //声明两种套接字
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
};
#endif // SERVERWIDGET_H
