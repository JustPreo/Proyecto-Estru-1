#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <vector>
#include <string>

#include <fstream>

#include "HistoryList.h"
#include "directory.h"
#include "file.h"

using std::string;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


protected:
    void keyPressEvent(QKeyEvent *event);//Esto es para conseguir teclas - no olvidar agregarlo despues yo del futuro
    void closeEvent(QCloseEvent *event);//cuando cierra app
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem*);
    void on_listWidget_itemClicked(QListWidgetItem*);
    void on_treeView_itemClicked(QTreeWidgetItem*,int);
    void showContextMenu(QPoint pos);

    void on_listWidget_itemPressed(QListWidgetItem*);


    void goToPath();
    void on_backButton_clicked();
    void on_ForwardButton_clicked();
    void on_goToFatherButton_clicked();

    void incrementSize();
    void decrementSize();

private:

    Ui::MainWindow *ui;

    Directory* root;
    Directory* currentDir;
    Directory* trashDir;

    std::vector<Directory*> favorites;

    HistoryList history;


    Directory* findDirectoryByPath(string path);
    void refreshTree();
    void refreshNavigation();
    void updatePath();


    void removeFavoritesRecursive(Directory* dir);
    void restoreFavoritesRecursive(Directory* dir);
    void addToTree(Directory* dir);

    void setViewMode(int mode);
    size_t getFolderSize(Directory* dir);
    void createFolder();
    void createFile();
    void restoreNode(OriginFile* node);

    OriginFile* draggedNode;//oara el drag drop
    OriginFile* clipboardNode;//referencia nodo copiado/cut
    bool clipboardCut;//solo si es cut

    //void on_treeWidget_itemPressed(QTreeWidgetItem*,int);
    void renameNode(OriginFile* node);
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

    int currentIconSize;

    bool isValidName(string name, bool isFile);
    string trim(string str);
    int minSize;
    int maxSize;
};

#endif
