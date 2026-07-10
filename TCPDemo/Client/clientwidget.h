#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui {
class Clientwidget;
}
QT_END_NAMESPACE

class Clientwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Clientwidget(QWidget *parent = nullptr);
    ~Clientwidget() override;

    void Init();
private slots:
    void on_buttonconnect_clicked();

    void on_buttonsend_clicked();

    void on_buttonclose_clicked();

private:
    Ui::Clientwidget *ui;
    QTcpSocket *tcpsocket;//声明套接字 客户端只有一个通信套接字
};
#endif // CLIENTWIDGET_H
