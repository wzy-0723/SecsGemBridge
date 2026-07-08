#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "FileUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化模型，绑定TreeView
    m_treeModelSF = new QStandardItemModel(this);
    // 设置表头名称
    m_treeModelSF->setHorizontalHeaderLabels(QStringList() << "目录结构");
    ui->treeViewSF->setModel(m_treeModelSF);
    ui->treeViewSF->setHeaderHidden(true); // 隐藏表头

    // 关键：开启自定义右键菜单
    ui->treeViewSF->setContextMenuPolicy(Qt::CustomContextMenu);
    // 绑定右键信号
    connect(ui->treeViewSF, &QTreeView::customContextMenuRequested,
            this, &MainWindow::onTreeRightClick);

    // 2. 构建多级树形目录
    buildTreeDir();

    // 可选：默认全部展开
    ui->treeViewSF->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildTreeDir()
{
    QStandardItem* root = m_treeModelSF->invisibleRootItem();
    QStandardItem* item1 = new QStandardItem("文件夹A");
    root->appendRow(item1);
    item1->appendRow(new QStandardItem("文件1.txt"));
    item1->appendRow(new QStandardItem("文件2.txt"));

    QStandardItem* item2 = new QStandardItem("文件夹B");
    root->appendRow(item2);
}

// 右键触发
void MainWindow::onTreeRightClick(const QPoint& pos)
{
    // 获取点击位置对应的索引
    QModelIndex index = ui->treeViewSF->indexAt(pos);
    if (!index.isValid())
        return; // 空白区域右键，不弹出菜单

    m_currentIndex = index; // 记录当前选中节点

    // 创建右键菜单
    QMenu menu(this);
    QAction* actEdit = menu.addAction("编辑");
    QAction* actDelete = menu.addAction("删除");
    menu.addSeparator();
    QAction* actCancel = menu.addAction("取消");

    // 菜单点击处理
    QAction* selectedAct = menu.exec(ui->treeViewSF->viewport()->mapToGlobal(pos));
    if (selectedAct == actEdit)
    {
        // 获取原有文字
        QStandardItem* item = m_treeModelSF->itemFromIndex(m_currentIndex);
        QString oldName = item->text();
        QString newName = showEditDialog(oldName);

        if (!newName.isEmpty())
        {
            item->setText(newName);
        }
    }
    else if (selectedAct == actDelete)
    {
        m_treeModelSF->removeRow(m_currentIndex.row(), m_currentIndex.parent());
    }
    else if(selectedAct == actCancel)
    {

    }
}

// 自定义编辑弹窗
QString MainWindow::showEditDialog(const QString& oldText)
{
    QDialog dialog(this);
    dialog.setWindowTitle("编辑节点名称");
    dialog.setModal(true);
    dialog.resize(300, 120);

    QVBoxLayout* lay = new QVBoxLayout(&dialog);
    QLineEdit* edit = new QLineEdit(oldText);
    QPushButton* btnOk = new QPushButton("确定");
    QPushButton* btnCancel = new QPushButton("取消");

    QHBoxLayout* btnLay = new QHBoxLayout;
    btnLay->addWidget(btnOk);
    btnLay->addWidget(btnCancel);

    lay->addWidget(edit);
    lay->addLayout(btnLay);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    int ret = dialog.exec();
    if (ret == QDialog::Accepted)
        return edit->text().trimmed();
    return "";
}

