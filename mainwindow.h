#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <vector>
#include <stack>
#include <fstream>

#include "directory.h"
#include "file.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,int);
    void on_treeWidget_itemClicked(QTreeWidgetItem*,int);
    void on_treeView_itemClicked(QTreeWidgetItem*,int);

    void showContextMenu(QPoint pos);

    void on_backButton_clicked();
    void on_ForwardButton_clicked();
    void on_goToFatherButton_clicked();

private:

    Ui::MainWindow *ui;

    Directory* root;
    Directory* currentDir;
    Directory* trashDir;

    std::vector<Directory*> favorites;

    std::stack<Directory*> backHistory;
    std::stack<Directory*> forwardHistory;

    void refreshTree();
    void refreshNavigation();
    void updatePath();

    void addToTree(Directory* dir,QTreeWidgetItem* parent);

    void createFolder();
    void createFile();

    bool nameExists(std::string name);

    void saveBinary();
    bool loadBinary();

    void collectFavorites(Directory* dir);

    void saveNode(std::ofstream& out, OriginFile* node);
    OriginFile* loadNode(std::ifstream& in);
};

#endif
