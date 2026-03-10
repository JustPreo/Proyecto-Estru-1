#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <vector>

#include <fstream>

#include "HistoryList.h"
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

    HistoryList history;

    void refreshTree();
    void refreshNavigation();
    void updatePath();

    void addToTree(Directory* dir);

    void createFolder();
    void createFile();
    void restoreNode(OriginFile* node);

    OriginFile* clipboardNode;//referencia nodo copiado/cut
    bool clipboardCut;//solo si es cut

    void copyNode(OriginFile* node);
    void cutNode(OriginFile* node);
    void pasteNode();
    OriginFile* cloneNode(OriginFile* node);
    bool isInside(Directory* target, OriginFile* node);

    bool nameExists(std::string name);

    void saveBinary();
    bool loadBinary();

    string getUniqueName(string name);
    void collectFavorites(Directory* dir);
    string getCurrentPath();
    string buildPath(OriginFile* node);
    void saveNode(std::ofstream& out, OriginFile* node);
    OriginFile* loadNode(std::ifstream& in);
};

#endif
