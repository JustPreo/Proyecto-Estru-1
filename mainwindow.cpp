#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notepad.h"

#include <QStyle>
#include <QVariant>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

using std::string;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    root = NULL;
    trashDir = new Directory("TRASH");

    if(!loadBinary())
        root = new Directory("/");

    currentDir = root;

    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDropIndicatorShown(true);
    ui->treeWidget->setDragDropMode(QAbstractItemView::InternalMove);

    ui->treeView->setDragEnabled(false);
    ui->treeView->setAcceptDrops(false);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(showContextMenu(QPoint)));

    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,int)));

    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(on_treeWidget_itemClicked(QTreeWidgetItem*,int)));

    connect(ui->treeView,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(on_treeView_itemClicked(QTreeWidgetItem*,int)));

    refreshNavigation();
    refreshTree();
}

MainWindow::~MainWindow()
{
    delete root;
    delete trashDir;
    delete ui;
}

void MainWindow::refreshNavigation()
{
    ui->treeView->clear();

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->treeView);
    rootItem->setText(0,"ROOT");
    rootItem->setData(0,Qt::UserRole,"root");
    rootItem->setData(0,Qt::UserRole+1,QVariant::fromValue((void*)root));
    rootItem->setIcon(0,style()->standardIcon(QStyle::SP_DriveHDIcon));

    QTreeWidgetItem* favItem = new QTreeWidgetItem(ui->treeView);
    favItem->setText(0,"Favorites");
    favItem->setIcon(0,style()->standardIcon(QStyle::SP_DirHomeIcon));
    favItem->setData(0,Qt::UserRole,"favorites");
    favItem->setExpanded(true);
    favItem->setFlags(Qt::ItemIsEnabled);

    for(int i=0;i<favorites.size();i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(favItem);
        item->setText(0,QString::fromStdString(favorites[i]->name));
        item->setData(0,Qt::UserRole,"dir");
        item->setData(0,Qt::UserRole+1,QVariant::fromValue((void*)favorites[i]));
        item->setIcon(0,style()->standardIcon(QStyle::SP_DirIcon));
    }

    QTreeWidgetItem* trashItem = new QTreeWidgetItem(ui->treeView);
    trashItem->setText(0,"TRASH");
    trashItem->setData(0,Qt::UserRole,"trash");
    trashItem->setData(0,Qt::UserRole+1,QVariant::fromValue((void*)trashDir));
    trashItem->setIcon(0,style()->standardIcon(QStyle::SP_TrashIcon));
}

void MainWindow::refreshTree()
{
    ui->treeWidget->clear();
    addToTree(currentDir,NULL);
}

void MainWindow::addToTree(Directory* dir,QTreeWidgetItem* parent) //es el arbol , oel insert de un arbol
{   //funcion que recorre todo

    QTreeWidgetItem* item;

    if(parent==NULL)
        item = new QTreeWidgetItem(ui->treeWidget);
    else
        item = new QTreeWidgetItem(parent);

    item->setText(0,QString::fromStdString(dir->name));
    item->setIcon(0,style()->standardIcon(QStyle::SP_DirIcon));
    item->setData(0,Qt::UserRole,QVariant::fromValue((void*)dir));

    for(int i=0;i<dir->children.size();i++)
    {
        OriginFile* child = dir->children[i];

        if(child->isDirectory())
        {
            addToTree((Directory*)child,item);
        }
        else
        {
            QTreeWidgetItem* fileItem = new QTreeWidgetItem(item);

            fileItem->setText(0,QString::fromStdString(child->name));
            fileItem->setIcon(0,style()->standardIcon(QStyle::SP_FileIcon));
            fileItem->setData(0,Qt::UserRole,QVariant::fromValue((void*)child));
        }
    }
}

bool MainWindow::nameExists(string name) //lo dice el nombre XD
{
    for(int i=0;i<currentDir->children.size();i++)
        if(currentDir->children[i]->name==name)
            return true;

    return false;
}

void MainWindow::createFolder()
{
    bool ok;

    QString text = QInputDialog::getText(this,"Nueva Carpeta", "Nombre:", QLineEdit::Normal,"", &ok);

    if(!ok || text.isEmpty())
        return;

    string name = text.toStdString();

    if(nameExists(name))
    {
        name = getUniqueName(name);
    }

    Directory* newDir = new Directory(name);
    newDir->parent = currentDir;
    currentDir->children.push_back(newDir);

    saveBinary();
    refreshTree();
}

void MainWindow::createFile()
{
    bool ok;

    QString text = QInputDialog::getText(this,
 "Nuevo Archivo", "Nombre:", QLineEdit::Normal, "", &ok);

    if(!ok || text.isEmpty())
        return;

    string name = text.toStdString();

    if(nameExists(name))
    {
        name = getUniqueName(name);
    }

    File* newFile = new File(name);
    newFile->parent = currentDir;
    currentDir->children.push_back(newFile);

    saveBinary();
    refreshTree();
}

void MainWindow::restoreNode(OriginFile* node)
{
    vector<string> pathParts;
    string temp = node->originalPath;
    string current = "";

    for(int i=0;i<temp.size();i++)
    {
        if(temp[i]=='/')
        {
            if(!current.empty())
            {
                pathParts.push_back(current);
                current="";
            }
        }
        else
            current+=temp[i];
    }

    if(!current.empty())
        pathParts.push_back(current);

    Directory* dir = root;

    if(pathParts.size() == 0)//osea si es /
        return;
    for(int i=0;i<pathParts.size()-1;i++)
    {
        bool found=false;

        for(int j=0;j<dir->children.size();j++)
        {
            if(dir->children[j]->isDirectory() &&
                dir->children[j]->name == pathParts[i])
            {
                dir = (Directory*)dir->children[j];
                found=true;
                break;
            }
        }

        if(!found)
        {
            Directory* newDir = new Directory(pathParts[i]);
            newDir->parent = dir;
            dir->children.push_back(newDir);
            dir = newDir;
        }
    }

    string newName = node->name;

    int copyIndex = 1;

    while(true)
    {
        bool exists=false;

        for(int i=0;i<dir->children.size();i++)
        {
            if(dir->children[i]->name == newName)
            {
                exists=true;
                break;
            }
        }

        if(!exists)
            break;

        newName = node->name + "_copy";
        if(copyIndex>1)
            newName += std::to_string(copyIndex);

        copyIndex++;
    }

    node->name = newName;
    node->parent = dir;
    dir->children.push_back(node);

    for(int i=0;i<trashDir->children.size();i++)
    {
        if(trashDir->children[i] == node)
        {
            trashDir->children.erase(trashDir->children.begin()+i);
            break;
        }
    }

    saveBinary();
    refreshTree();
}

void MainWindow::showContextMenu(QPoint pos) //context menu shows
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(!item) return;

    QVariant data = item->data(0,Qt::UserRole);
    OriginFile* node = (OriginFile*)data.value<void*>();

    QMenu menu(this);

    QAction* createFileAction = menu.addAction("Create File");
    QAction* createFolderAction = menu.addAction("Create Folder");
    QAction* deleteAction = NULL;
    QAction* restoreAction = NULL;
    QAction* favoriteAction = NULL;

    if(node->isDirectory())
        favoriteAction = menu.addAction("Add to Favorites");

    if(currentDir == trashDir)
        restoreAction = menu.addAction("Restore");

    if (currentDir != trashDir)
        deleteAction = menu.addAction("Delete");
    QAction* selected = menu.exec(ui->treeWidget->mapToGlobal(pos));

    if(selected==createFileAction)
        createFile();

    else if(selected==createFolderAction)
        createFolder();

    else if(selected==favoriteAction)
    {
        Directory* dir = (Directory*)node;
        if(!dir->isFavorite)
        {
            dir->isFavorite = true;
            favorites.push_back(dir);
        }
        refreshNavigation();
    }

    else if(selected == restoreAction)
    {
        restoreNode(node);
    }

    else if(selected==deleteAction)
    {
        /*if(currentDir==trashDir)
        {
            for(int i=0;i<trashDir->children.size();i++)
            {
                if(trashDir->children[i]==node)
                {
                    delete node;
                    trashDir->children.erase(trashDir->children.begin()+i);
                    break;
                }
            }
        }*/

            if(currentDir == trashDir){
                return;
            }

            else//Soft delete solamente
            {
                Directory* parent = (Directory*)node->parent;

                if(parent == NULL)
                    return;

                for(int i=0;i<parent->children.size();i++)
                {
                    if(parent->children[i]==node)
                    {
                        node->originalPath = buildPath(node);

                        for(int j=0;j<favorites.size();j++)
                        {
                            if(favorites[j] == node)
                            {
                                favorites.erase(favorites.begin()+j);
                                break;
                            }
                        }

                        parent->children.erase(parent->children.begin()+i);

                        node->parent = trashDir;
                        trashDir->children.push_back(node);

                        break;
                    }
                }
            }

        saveBinary();
        refreshTree();
        refreshNavigation();
        return;

}
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* item,int) //como en java , el on item clicked
{
    QVariant data = item->data(0,Qt::UserRole);

    if(!data.isValid())
        return;

    OriginFile* node = (OriginFile*)data.value<void*>();

    ui->label->setText(QString::fromStdString(node->name));

    QString info;

    if(node->isDirectory())
    {
        Directory* dir = (Directory*)node;
        info += "Type: Directory\n";
        info += "Children: " + QString::number(dir->children.size());
    }
    else
    {
        File* file = (File*)node;
        info += "Type: File\n";
        info += "Size: " + QString::number(file->content.length()) + " bytes";
    }

    ui->plainTextEdit->setPlainText(info);
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item,int) //estructura java
//se encarga de meterse
{
    QVariant data = item->data(0,Qt::UserRole);

    if(!data.isValid())
        return;

    OriginFile* node = (OriginFile*)data.value<void*>();

    if(node->isDirectory())
    {
        Directory* dir = (Directory*)node;

        if(dir == currentDir)
            return;

        backHistory.push(currentDir);

        currentDir = dir;

        while(!forwardHistory.empty())
            forwardHistory.pop();

        refreshTree();
    }
    else
    {
        File* file = (File*)node;

        notepad* editor = new notepad(file,this);
        editor->show();
    }
}
void MainWindow::on_treeView_itemClicked(QTreeWidgetItem* item,int)
{
    //es para el lado izquierdo
    QVariant type = item->data(0,Qt::UserRole);

    if(!type.isValid())
        return;

    QString t = type.toString();

    if(t=="root")
    {
        currentDir=root;
        refreshTree();
        return;
    }

    if(t=="trash")
    {
        currentDir=trashDir;
        refreshTree();
        return;
    }

    if(t=="dir")
    {
        Directory* dir = (Directory*)item->data(0,Qt::UserRole+1).value<void*>();
        currentDir=dir;
        refreshTree();
    }
}

///aqui empeiza el historial

void MainWindow::on_backButton_clicked()
{
    if(backHistory.empty()) return;

    forwardHistory.push(currentDir);

    currentDir = backHistory.top();
    backHistory.pop();

    refreshTree();
}

void MainWindow::on_ForwardButton_clicked()
{
    if(forwardHistory.empty()) return;

    backHistory.push(currentDir);

    currentDir = forwardHistory.top();
    forwardHistory.pop();

    refreshTree();
}

void MainWindow::on_goToFatherButton_clicked()
{
    if(currentDir->parent==NULL) return;

    backHistory.push(currentDir);

    currentDir = (Directory*)currentDir->parent;

    refreshTree();
}

//sistema de guardado
void MainWindow::saveBinary()
{
    std::ofstream out("filesystem.bin", std::ios::binary);

    if(!out.is_open()) return;

    saveNode(out,root);

    int trashCount = trashDir->children.size();
    out.write((char*)&trashCount,sizeof(int));

    for(int i=0;i<trashCount;i++)
        saveNode(out,trashDir->children[i]);

    out.close();
}

bool MainWindow::loadBinary()
{
    std::ifstream in("filesystem.bin",std::ios::binary);

    if(!in.is_open())
        return false;

    if(root) delete root;

    root = (Directory*)loadNode(in);

    if(!root) return false;

    int trashCount;

    in.read((char*)&trashCount,sizeof(int));

    for(int i=0;i<trashCount;i++)
    {
        OriginFile* node = loadNode(in);
        node->parent = trashDir;
        trashDir->children.push_back(node);
    }

    currentDir=root;

    in.close();

    favorites.clear();
    collectFavorites(root);

    return true;
}

void MainWindow::saveNode(std::ofstream& out,OriginFile* node)
{
    bool isDir = node->isDirectory();
    out.write((char*)&isDir,sizeof(bool));

    int nameSize = node->name.size();
    out.write((char*)&nameSize,sizeof(int));
    out.write(node->name.c_str(),nameSize);
    bool fav = node->isFavorite;
    out.write((char*)&fav,sizeof(bool));

    int pathSize = node->originalPath.size();
    out.write((char*)&pathSize,sizeof(int));
    out.write(node->originalPath.c_str(),pathSize);

    if(isDir)
    {
        Directory* dir=(Directory*)node;

        int count=dir->children.size();
        out.write((char*)&count,sizeof(int));

        for(int i=0;i<count;i++)
            saveNode(out,dir->children[i]);
    }
    else
    {
        File* file=(File*)node;

        int contentSize=file->content.size();
        out.write((char*)&contentSize,sizeof(int));
        out.write(file->content.c_str(),contentSize);
    }
}

OriginFile* MainWindow::loadNode(std::ifstream& in)
{
    bool isDir;

    if(!in.read((char*)&isDir,sizeof(bool)))
        return NULL;

    int nameSize;
    in.read((char*)&nameSize,sizeof(int));

    char* buffer=new char[nameSize+1];
    in.read(buffer,nameSize);
    buffer[nameSize]='\0';

    string name(buffer);

    bool fav;
    in.read((char*)&fav,sizeof(bool));

    int pathSize;
    in.read((char*)&pathSize,sizeof(int));

    char* pathBuffer = new char[pathSize+1];
    in.read(pathBuffer,pathSize);
    pathBuffer[pathSize]='\0';

    string originalPath(pathBuffer);
    delete[] pathBuffer;

    if(isDir)
    {
        Directory* dir=new Directory(name);
        dir->isFavorite = fav;
        dir->originalPath = originalPath;
        int count;
        in.read((char*)&count,sizeof(int));

        for(int i=0;i<count;i++)
        {
            OriginFile* child=loadNode(in);
            child->parent = dir;
            dir->children.push_back(child);
        }

        return dir;
    }
    else
    {
        File* file=new File(name);
        file->isFavorite = fav;
        file->originalPath = originalPath;
        int contentSize;
        in.read((char*)&contentSize,sizeof(int));

        char* contentBuffer=new char[contentSize+1];
        in.read(contentBuffer,contentSize);
        contentBuffer[contentSize]='\0';

        file->content=string(contentBuffer);
        delete[] contentBuffer;

        return file;
    }
}

string MainWindow::getUniqueName(string name)
{
    string newName = name;
    int copyIndex = 1;

    while(nameExists(newName))
    {
        newName = name + "_copy";
        if(copyIndex>1)
            newName += std::to_string(copyIndex);
        copyIndex++;
    }

    return newName;
}

//sistema de cargado adicional

void MainWindow::collectFavorites(Directory* dir)
{
    if(dir->isFavorite && dir->isDirectory())
        favorites.push_back(dir);

    for(int i=0;i<dir->children.size();i++)
    {
        if(dir->children[i]->isDirectory())
            collectFavorites((Directory*)dir->children[i]);
    }
}


string MainWindow::getCurrentPath()
{
    Directory* temp = currentDir;
    string path = "";

    while(temp != NULL)
    {
        path = "/" + temp->name + path;
        temp = (Directory*)temp->parent;
    }

    return path;
}

string MainWindow::buildPath(OriginFile* node)
{
    string path = node->name;

    Directory* p = (Directory*)node->parent;

    while(p != NULL)
    {
        path = p->name + "/" + path;
        p = (Directory*)p->parent;
    }

    return path;
}
