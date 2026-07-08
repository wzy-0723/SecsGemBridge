#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMainWindow>
#include <QStandardItem>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    // 递归创建多级目录节点
    void buildTreeDir();
    // 弹出编辑对话框，返回新名称
    QString showEditDialog(const QString& oldText);
private slots:
    // 右键菜单触发槽
    void onTreeRightClick(const QPoint& pos);

private:
    Ui::MainWindow *ui;
    QStandardItemModel* m_treeModelSF;
    // 保存当前右键选中的节点
    QModelIndex m_currentIndex;
};
#endif // MAINWINDOW_H
