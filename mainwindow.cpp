#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notepad.h"

#include <QStyle>
#include <QVariant>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QKeyEvent>
#include <QCloseEvent>
#include <sstream>
using std::string;


//uso quintptr porque qt lo prefiere usar para evitar un error de consola que crasheaba programa no se porque QVariant::save: unable to save type 'void*'
//(eso es direccion de memoria implicita)
//Fue la uncia solucion que me encontre en vez de usar node directamente
//Aparte , el reinterpret_cast evita otro crashe oque me salio respectivo a lo de node = , entonces ni modo a usar estos dos. Sorry inge -> <-

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    draggedNode = NULL;
    root = NULL;

    clipboardNode = NULL;
    clipboardCut = false;


    trashDir = new Directory("TRASH");

    if(!loadBinary()){
        root = new Directory("/");
    }
    currentDir = root;
    history.visit(root);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setAcceptDrops(true);
    ui->listWidget->viewport()->setAcceptDrops(true);
    ui->listWidget->setDropIndicatorShown(true);
    ui->listWidget->setMovement(QListView::Snap);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->viewport()->installEventFilter(this);


    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(on_listWidget_itemClicked(QListWidgetItem*)));

    connect(ui->listWidget, SIGNAL(itemPressed(QListWidgetItem*)),
            this, SLOT(on_listWidget_itemPressed(QListWidgetItem*)));



    //botones

    connect(ui->pathLabel, SIGNAL(returnPressed()),
            this, SLOT(goToPath()));

    connect(ui->goButton, SIGNAL(clicked()),
            this, SLOT(goToPath()));

    connect(ui->incrementSize, SIGNAL(clicked()),
            this, SLOT(incrementSize()));

    connect(ui->decrementSize, SIGNAL(clicked()),
            this, SLOT(decrementSize()));


    currentIconSize = 48;
    setViewMode(0);
    refreshNavigation();
    refreshTree();
}

MainWindow::~MainWindow()
{
    delete root;
    delete trashDir;
    delete ui;
}

void MainWindow::incrementSize()
{
    currentIconSize += 16;

    if(currentIconSize > maxSize)
        currentIconSize = maxSize;

    ui->listWidget->setIconSize(QSize(currentIconSize, currentIconSize));
    if(ui->listWidget->viewMode() == QListView::IconMode)
    {
        ui->listWidget->setGridSize(QSize(currentIconSize + 20, currentIconSize + 30));
    }
}

void MainWindow::decrementSize()
{
    currentIconSize -= 16;

    if(currentIconSize < minSize)
        currentIconSize = minSize;

    ui->listWidget->setIconSize(QSize(currentIconSize, currentIconSize));
    if(ui->listWidget->viewMode() == QListView::IconMode)
    {
        ui->listWidget->setGridSize(QSize(currentIconSize + 20, currentIconSize + 30));
    }
}

void MainWindow::refreshNavigation()
{
    ui->treeView->clear();

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->treeView);
    rootItem->setText(0,"ROOT");
    rootItem->setData(0,Qt::UserRole,"root");
    rootItem->setData(0,Qt::UserRole+1,QVariant::fromValue((void*)root));
    rootItem->setIcon(0,style()->standardIcon(QStyle::SP_DriveHDIcon));



    QTreeWidgetItem* trashItem = new QTreeWidgetItem(ui->treeView);
    trashItem->setText(0,"TRASH");
    trashItem->setData(0,Qt::UserRole,"trash");
    trashItem->setData(0,Qt::UserRole+1,QVariant::fromValue((void*)trashDir));
    trashItem->setIcon(0,style()->standardIcon(QStyle::SP_TrashIcon));



    for(int i=0;i<favorites.size();i++)
    {
        Directory* fav = favorites[i];

        if(!fav){
            continue;
        }

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeView);

        item->setText(0,QString::fromStdString(fav->name));

        item->setData(0,Qt::UserRole,"dir");
        item->setData(0,Qt::UserRole+1,QVariant::fromValue((quintptr)fav));

        item->setIcon(0,style()->standardIcon(QStyle::SP_DirIcon));
    }

}

void MainWindow::on_listWidget_itemPressed(QListWidgetItem* item)
{
    if(!item) return;

    quintptr ptr = item->data(Qt::UserRole).value<quintptr>();
    if(ptr == 0) return;

    draggedNode = reinterpret_cast<OriginFile*>(ptr);
}

void MainWindow::refreshTree()
{
    if(currentDir == NULL){
        return;
    }

    if(!currentDir->isDirectory()){
        return;
    }

    ui->listWidget->clear();

    ui->pathLabel->setText(QString::fromStdString(getCurrentPath()));

    addToTree(currentDir);
}

void MainWindow::addToTree(Directory* dir)
{
    if(dir == NULL)
        return;

    for(int i=0;i<dir->children.size();i++)
    {
        OriginFile* child = dir->children[i];

        if(child == NULL){
            continue;
        }


        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

        item->setText(QString::fromStdString(child->name));

        item->setData(Qt::UserRole, QVariant::fromValue((quintptr)child));

        if(child->isDirectory())
            item->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
        else
            item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    }
}
bool MainWindow::nameExists(string name) //lo dice el nombre XD
{
    for(int i=0;i<currentDir->children.size();i++)
        if(currentDir->children[i]->name==name){
            return true;
        }

    return false;
}

void MainWindow::createFolder()
{
    bool ok;

    QString text = QInputDialog::getText(this,"Nueva Carpeta", "Nombre:", QLineEdit::Normal,"", &ok);

    if(!ok || text.isEmpty())
        return;

    string name = trim(text.toStdString());

    if(!isValidName(name, false))
    {
        QMessageBox::warning(this, "Error", "Nombre de carpeta invalido");
        return;
    }

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

    QString text = QInputDialog::getText(this,"New File","File name:",QLineEdit::Normal,"",&ok);

    if(!ok || text.isEmpty())
        return;

    string name = trim(text.toStdString());

    if(!isValidName(name, false))
    {
        QMessageBox::warning(this, "Error", "Nombre de archivo invalido");
        return;
    }

    // agregar .txt
    if(name.size() < 4 || name.substr(name.size()-4) != ".txt")
    {
        name += ".txt";
    }

    File* file = new File(name);

    file->parent = currentDir;

    currentDir->children.push_back(file);

    refreshTree();
}

void MainWindow::restoreNode(OriginFile* node)
{
    if(!node)
        return;

    for(int i=0;i<trashDir->children.size();i++)
    {
        if(trashDir->children[i] == node)
        {
            trashDir->children.erase(trashDir->children.begin()+i);
            break;
        }
    }

    Directory* dir = findDirectoryByPath(node->originalPath);

    if(!dir)
        dir = root;

    node->parent = dir;

    string newName = node->name;

    bool exists = true;

    while(exists)
    {
        exists = false;

        for(int i=0;i<dir->children.size();i++)
        {
            if(dir->children[i]->name == newName)
            {
                exists = true;
                newName = getUniqueName(newName);
                break;
            }
        }
    }

    node->name = newName;

    dir->children.push_back(node);

    node->modifiedDate = time(NULL);

    if(node->isDirectory())
    {
        restoreFavoritesRecursive((Directory*)node);
        refreshNavigation();
    }


    refreshTree();
}

void MainWindow::showContextMenu(QPoint pos)
{
    QListWidgetItem* item = ui->listWidget->itemAt(pos);


    OriginFile* node = NULL;

    // si se hizo click en item
    if(item)
    {
        QVariant data = item->data(Qt::UserRole);

        if(data.isValid())
        {
            quintptr ptr = data.value<quintptr>();

            if(ptr != 0)
                node = reinterpret_cast<OriginFile*>(ptr);
        }
    }

    QMenu menu(this);

    QAction* newFolder = NULL;
    QAction* newFile = NULL;
    QAction* rename = NULL;
    QAction* copy = NULL;
    QAction* cut = NULL;
    QAction* paste = NULL;
    QAction* remove = NULL;
    QAction* restore = NULL;
    QAction* favorite = NULL;

    // crear carpeta y archivo siempre disponibles




    menu.addSeparator();

    QMenu* viewMenu = menu.addMenu("View");

    QAction* lista = viewMenu->addAction("List View");
    QAction* iconos = viewMenu->addAction("Icon View");
    QAction* grandes = viewMenu->addAction("Big Icon View");

    if (currentDir != trashDir){
        newFolder = menu.addAction("New Folder");
        newFile = menu.addAction("New File");
        rename = menu.addAction("Rename");
        copy = menu.addAction("Copy");
        cut = menu.addAction("Cut");

    }

    if(node != NULL)
    {

        // solo carpetas pueden ser favoritas

        // si esta en trash , restore
        if(currentDir == trashDir)
        {
            restore = menu.addAction("Restore");
        }
        else
        {
            remove = menu.addAction("Delete");

            if(node->isDirectory())
            {
                Directory* dir = (Directory*)node;
                if(dir->isFavorite)
                {
                    favorite = menu.addAction("Remove from Favorites");
                }
                else
                {
                    favorite = menu.addAction("Add to Favorites");
                }
            }
        }
    }

    // paste solo si hay algo en clipboard
    if(clipboardNode != NULL)
    {
        menu.addSeparator();
        paste = menu.addAction("Paste");
    }

    QAction* selected = menu.exec(ui->listWidget->viewport()->mapToGlobal(pos));

    if(selected == NULL)
        return;

    if(selected == newFolder)
    {
        createFolder();
    }

    else if(selected == newFile)
    {
        createFile();
    }

    else if(selected == rename && node)
    {
        renameNode(node);
        refreshNavigation();
        //refreshTree();
    }

    else if(selected == copy && node)
    {
        copyNode(node);
    }

    else if(selected == cut && node)
    {
        cutNode(node);
    }

    else if(selected == paste)
    {
        pasteNode();
    }

    else if(selected == remove && node)
    {
        Directory* parent = (Directory*)node->parent;



        if(parent)
        {
            for(int i=0;i<parent->children.size();i++)
            {
                if(parent->children[i] == node)
                {
                    parent->children.erase(parent->children.begin()+i);
                    break;
                }
            }
        }

        if(node->isDirectory())
        {
            removeFavoritesRecursive((Directory*)node);
        }
        refreshNavigation();

        // guardar path original
        node->originalPath = getCurrentPath();

        node->parent = trashDir;
        trashDir->children.push_back(node);

        refreshTree();
    }

    else if(selected == restore && node)
    {
        restoreNode(node);
        refreshNavigation();
    }

    else if(selected == favorite && node)
    {
        Directory* dir = (Directory*)node;

        if(dir->isFavorite)
        {
            // quitar fav
            for(int i = 0; i < favorites.size(); i++)
            {
                if(favorites[i] == dir)
                {
                    favorites.erase(favorites.begin() + i);
                    break;
                }
            }

            dir->isFavorite = false;
        }
        else
        {
            favorites.push_back(dir);
            dir->isFavorite = true;
        }

        refreshNavigation();
    }

    if(selected == lista) setViewMode(0);
    else if(selected == iconos) setViewMode(1);
    else if(selected == grandes) setViewMode(2);

}


//auxiliar para lo de favoritos (bug encontrado con inge)
void MainWindow::removeFavoritesRecursive(Directory* dir)
{
    for(int i = 0; i < favorites.size(); i++)
    {
        if(favorites[i] == dir)
        {
            favorites.erase(favorites.begin() + i);
            break;
        }
    }

    // recorre hijos
    for(int i = 0; i < dir->children.size(); i++)
    {
        if(dir->children[i]->isDirectory())
        {
            removeFavoritesRecursive((Directory*)dir->children[i]);
        }
    }
}

void MainWindow::restoreFavoritesRecursive(Directory* dir)
{
    // si era favorito, agregar
    if(dir->isFavorite)
    {
        bool exists = false;

        for(int i = 0; i < favorites.size(); i++)
        {
            if(favorites[i] == dir)
            {
                exists = true;
                break;
            }
        }

        if(!exists){
            favorites.push_back(dir);
        }
    }

    // recorrer hijos
    for(int i = 0; i < dir->children.size(); i++)
    {
        if(dir->children[i]->isDirectory())
        {
            restoreFavoritesRecursive((Directory*)dir->children[i]);
        }
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem* item)
{
    if(!item){
        return;
    }

    quintptr ptr = item->data(Qt::UserRole).value<quintptr>();

    if(ptr == 0){
        return;
    }

    OriginFile* node = reinterpret_cast<OriginFile*>(ptr);

    if(!node){
        return;
    }

    QString info;

    if(node->isDirectory())
    {
        Directory* dir = (Directory*)node;

        size_t size = getFolderSize(dir);

        info += "Type: Directory\n";
        info += "Children: ";
        info += QString::number(dir->children.size());

        info += "\nSize: ";
        info += QString::number(size);
        info += " bytes";
    }
    else
    {
        File* file = (File*)node;

        info += "Type: File\n";

        info += "Size: ";
        info += QString::number(file->content.length());
        info += " bytes\n";
    }

    // convertir fechas
    char created[100];
    char modified[100];

    strftime(created,100,"%Y-%m-%d %H:%M",localtime(&node->createdDate));
                        //Year/Month/Day  Hora/Minuto

    strftime(modified,100,"%Y-%m-%d %H:%M",localtime(&node->modifiedDate));

    info += "\nCreated: ";
    info += created;

    info += "\nModified: ";
    info += modified;

    ui->plainTextEdit->setPlainText(info);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
    if(!item){
        return;
    }
    QVariant data = item->data(Qt::UserRole);

    if(!data.isValid()){
        return;
    }

    // recuperar puntero guardado
    quintptr ptr = data.value<quintptr>();

    if(ptr == 0){
        return;
    }

    OriginFile* node = reinterpret_cast<OriginFile*>(ptr);

    if(!node){
        return;
    }

    // si es carpeta entrar
    if(node->isDirectory())
    {
        Directory* dir = static_cast<Directory*>(node);

        if(!dir){
            return;
        }

        if(dir == currentDir){
            return;
        }

        history.visit(dir);

        currentDir = dir;

        refreshTree();
    }
    else
    {
        // si es archivo abrir editor
        File* file = static_cast<File*>(node);

        if(!file){
            return;
        }

        notepad* editor = new notepad(file,this);
        editor->show();
    }
}

void MainWindow::on_treeView_itemClicked(QTreeWidgetItem* item,int)
{
    //es para el lado izquierdo
    QVariant type = item->data(0,Qt::UserRole);

    if(!type.isValid()){
        return;
    }

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
        quintptr ptr = item->data(0,Qt::UserRole+1).value<quintptr>();
        Directory* dir = reinterpret_cast<Directory*>(ptr);
        currentDir=dir;
        refreshTree();
    }
}



///aqui empeiza el historial

void MainWindow::on_backButton_clicked()
{
    if(!history.canGoBack()){
        return;
    }

    Directory* dir = history.goBack();

    if(dir == NULL)
    {
        return;
    }

    currentDir = dir;

    refreshTree();
}
void MainWindow::on_ForwardButton_clicked()
{
    if(!history.canGoForward()) {
        return;
    }

    Directory* dir = history.goForward();

    if(dir)
    {
        currentDir = dir;
        refreshTree();
    }
}
void MainWindow::on_goToFatherButton_clicked()
{
    if(currentDir->parent==NULL){
        return;
    }

    Directory* parent = (Directory*)currentDir->parent;

    history.visit(parent);

    currentDir = parent;

    refreshTree();
}

//sistema de guardado
void MainWindow::saveBinary()
{
    std::ofstream out("filesystem.bin", std::ios::binary);

    if(!out.is_open()){
        return;
    }

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

    if(!in.is_open()){
        return false;
    }

    if(root) {
        delete root;
    }

    root = (Directory*)loadNode(in);

    if(!root) {
        return false;
    }

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
    refreshNavigation();
    return true;
}

void MainWindow::saveNode(std::ofstream& out,OriginFile* node)
{
    if(!node){
        return;
    }

    bool isDir = node->isDirectory();

    out.write((char*)&isDir,sizeof(bool));

    int nameSize = node->name.size();

    out.write((char*)&nameSize,sizeof(int));
    out.write(node->name.c_str(),nameSize);

    // guardar fechas
    out.write((char*)&node->createdDate,sizeof(time_t));
    out.write((char*)&node->modifiedDate,sizeof(time_t));

    bool fav = node->isFavorite;
    out.write((char*)&fav,sizeof(bool));

    int pathSize = node->originalPath.size();
    out.write((char*)&pathSize, sizeof(int));
    out.write(node->originalPath.c_str(), pathSize);

    if(isDir)
    {
        Directory* dir = (Directory*)node;

        int count = dir->children.size();

        out.write((char*)&count,sizeof(int));

        for(int i=0;i<count;i++)
        {
            saveNode(out,dir->children[i]);
        }
    }
    else
    {
        File* file = (File*)node;

        int size = file->content.size();

        out.write((char*)&size,sizeof(int));
        out.write(file->content.c_str(),size);
    }
}

OriginFile* MainWindow::loadNode(std::ifstream& in)
{
    bool isDir;

    in.read((char*)&isDir,sizeof(bool));

    int nameSize;

    in.read((char*)&nameSize,sizeof(int));

    string name(nameSize,' ');
    in.read(&name[0],nameSize);

    OriginFile* node;

    if(isDir){
        node = new Directory(name);
    }
    else{
        node = new File(name);
    }

    // leer fechas
    in.read((char*)&node->createdDate,sizeof(time_t));
    in.read((char*)&node->modifiedDate,sizeof(time_t));

    bool fav;
    in.read((char*)&fav,sizeof(bool));
    node->isFavorite = fav;

    int pathSize;
    in.read((char*)&pathSize, sizeof(int));

    string path(pathSize, ' ');
    in.read(&path[0], pathSize);

    node->originalPath = path;

    if(isDir)
    {
        Directory* dir = (Directory*)node;

        int count;

        in.read((char*)&count,sizeof(int));

        for(int i=0;i<count;i++)
        {
            OriginFile* child = loadNode(in);

            child->parent = dir;

            dir->children.push_back(child);
        }
    }
    else
    {
        File* file = (File*)node;

        int size;

        in.read((char*)&size,sizeof(int));

        string content(size,' ');
        in.read(&content[0],size);

        file->content = content;
    }

    return node;
}

string MainWindow::getUniqueName(string name)
{
    string base = name;
    string extension = "";

    size_t pos = name.find_last_of('.');

    if(pos != string::npos)
    {
        base = name.substr(0,pos);
        extension = name.substr(pos);
    }

    string newName = base + " copy" + extension;

    int count = 2;

    bool exists = true;

    while(exists)
    {
        exists = false;

        for(int i=0;i<currentDir->children.size();i++)
        {
            if(currentDir->children[i]->name == newName)
            {
                exists = true;
                newName =base + " copy" +std::to_string(count) +extension;
                count++;

                break;
            }
        }
    }

    return newName;
}

//sistema de cargado adicional

void MainWindow::collectFavorites(Directory* dir)
{
    if(dir->isFavorite && dir->isDirectory()){
        favorites.push_back(dir);
    }

    for(int i=0;i<dir->children.size();i++)
    {
        if(dir->children[i]->isDirectory()){
            collectFavorites((Directory*)dir->children[i]);
        }
    }
}


string MainWindow::getCurrentPath()
{
    Directory* temp = currentDir;

    if(temp == root){
        return "/";}

    string path = "";

    while(temp != NULL && temp != root)
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

void MainWindow::renameNode(OriginFile* node)
{
    if(!node){
        return;}

    bool ok;

    QString text = QInputDialog::getText(this,"Rename","New name:",QLineEdit::Normal,QString::fromStdString(node->name),&ok);
    string texttext = text.toStdString();
    if(!ok || text.isEmpty())
        return;


    if(!isValidName(texttext, false))
    {
        QMessageBox::warning(this, "Error", "Nombre de archivo invalido");
        return;
    }


    /*if(texttext.size() < 4 || texttext.substr(texttext.size()-4) != ".txt")
    {
        texttext += ".txt";
    }*/


    string name = texttext;

    // solo para archivos
    if(!node->isDirectory())
    {
        if(name.size() < 4 || name.substr(name.size()-4) != ".txt")
        {
            name += ".txt";
        }
    }

    if(nameExists(name))
    {
        name = getUniqueName(name);
    }

    node->name = name;

    node->modifiedDate = time(NULL);

    refreshTree();
}




void MainWindow::copyNode(OriginFile* node)
{
    clipboardNode = node;
    clipboardCut = false;
}

void MainWindow::cutNode(OriginFile* node)
{
    clipboardNode = node;
    clipboardCut = true;
}

void MainWindow::pasteNode()
{
    if(!clipboardNode){
        return;}

    if(clipboardCut)
    {
        Directory* parent = (Directory*)clipboardNode->parent;

        if(parent)
        {
            for(int i=0;i<parent->children.size();i++)
            {
                if(parent->children[i] == clipboardNode)
                {
                    parent->children.erase(parent->children.begin()+i);
                    break;
                }
            }
        }

        clipboardNode->parent = currentDir;

        currentDir->children.push_back(clipboardNode);

        clipboardNode->modifiedDate = time(NULL);

        clipboardNode = NULL;
        clipboardCut = false;
    }
    else
    {
        OriginFile* copy = cloneNode(clipboardNode);

        // generar nombre único
        copy->name = getUniqueName(copy->name);

        copy->parent = currentDir;

        currentDir->children.push_back(copy);

        copy->modifiedDate = time(NULL);
    }

    refreshTree();
}

OriginFile* MainWindow::cloneNode(OriginFile* node)
{
    if(node->isDirectory())
    {
        Directory* oldDir = (Directory*)node;

        Directory* newDir = new Directory(oldDir->name);

        for(int i=0;i<oldDir->children.size();i++)
        {
            OriginFile* childCopy = cloneNode(oldDir->children[i]);

            childCopy->parent = newDir;

            newDir->children.push_back(childCopy);
        }

        return newDir;
    }
    else
    {
        File* oldFile = (File*)node;

        File* newFile = new File(oldFile->name);

        newFile->content = oldFile->content;

        return newFile;
    }
}

bool MainWindow::isInside(Directory* target, OriginFile* node)//Talvez funciona , crashea si no reviso si esta iside
{
    OriginFile* temp = target;

    while(temp != NULL)
    {
        if(temp == node)
            return true;

        temp = temp->parent;
    }

    return false;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //ctrl = Qt::ControlModifier

    // ctrl c
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C)
    {
        QListWidgetItem* item = ui->listWidget->currentItem();

        if(!item){
            return;}

        quintptr ptr = item->data(Qt::UserRole).value<quintptr>();

        if(ptr == 0){
            return;}

        OriginFile* node = reinterpret_cast<OriginFile*>(ptr);

        if(!node){
            return;}

        copyNode(node);
    }

    // ctrl x
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_X)
    {
        QListWidgetItem* item = ui->listWidget->currentItem();

        if(!item){
            return;
        }

        quintptr ptr = item->data(Qt::UserRole).value<quintptr>();

        if(ptr == 0){
            return;
        }

        OriginFile* node = reinterpret_cast<OriginFile*>(ptr);

        if(!node){
            return;
        }

        cutNode(node);
    }

    // ctrl v
    else if(event->modifiers() == Qt::ControlModifier &&
             event->key() == Qt::Key_V)
    {
        pasteNode();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // guardar todo el filesystem antes de cerrar
    saveBinary();
    event->accept();
}

size_t MainWindow::getFolderSize(Directory* dir)//recusiva inge decia
{
    size_t total = 0;

    for(int i=0;i<dir->children.size();i++)
    {
        OriginFile* node = dir->children[i];

        if(node->isDirectory())
        {
            // si es carpeta, add from carpeta
            total += getFolderSize((Directory*)node);
        }
        else
        {
            File* file = (File*)node;

            total += file->content.size();
        }
    }

    return total;
}

Directory* MainWindow::findDirectoryByPath(string path)
{
    if(path == "/" || path == ""){
        return root;
    }

    Directory* current = root;

    std::stringstream ss(path);
    string segment;

    getline(ss,segment,'/'); // vacio antes del primer /

    while(getline(ss,segment,'/'))
    {
        bool found = false;

        for(int i=0;i<current->children.size();i++)
        {
            OriginFile* child = current->children[i];

            if(child->isDirectory() && child->name == segment)
            {
                current = (Directory*)child;
                found = true;
                break;
            }
        }

        if(!found){
            return NULL;}
    }

    return current;
}

void MainWindow::goToPath()
{
    QString text = ui->pathLabel->text();

    string path = text.toStdString();

    Directory* dir = findDirectoryByPath(path);

    if(dir)
    {
        history.visit(dir);
        currentDir = dir;
    }
    else
    {
        QMessageBox::warning(this, "Error", "Path no existe");

        currentDir = root;
        history.visit(root);
    }

    refreshTree();
    ui->pathLabel->setText(QString::fromStdString(getCurrentPath()));
}

void MainWindow::dropEvent(QDropEvent *event)
{
    OriginFile* dragged = draggedNode;
    if(!dragged) return;

    QListWidgetItem* targetItem = ui->listWidget->itemAt(event->position().toPoint());
    if(!targetItem) return;

    quintptr targetPtr = targetItem->data(Qt::UserRole).value<quintptr>();
    if(targetPtr == 0) return;

    OriginFile* targetNode = reinterpret_cast<OriginFile*>(targetPtr);
    if(!targetNode || !targetNode->isDirectory()) return;

    Directory* targetDir = (Directory*)targetNode;

    // no meter misma carpeta
    if(isInside(targetDir, dragged)) return;

    // no soltar dir trash
    if(targetDir == trashDir)
        return;

    // no mover trash dir
    if(dragged->parent == trashDir)
        return;

    // cambio de old gen a new gen
    Directory* oldParent = (Directory*)dragged->parent;
    if(oldParent)
    {
        for(int i=0;i<oldParent->children.size();i++)
        {
            if(oldParent->children[i] == dragged)
            {
                oldParent->children.erase(oldParent->children.begin()+i);
                break;
            }
        }
    }

    // evitar nombres duplicados
    string newName = dragged->name;
    bool exists = true;

    while(exists)
    {
        exists = false;

        for(int i=0;i<targetDir->children.size();i++)
        {
            if(targetDir->children[i]->name == newName)
            {
                exists = true;
                newName = getUniqueName(newName);
                break;
            }
        }
    }

    dragged->name = newName;

    // mover
    dragged->parent = targetDir;
    targetDir->children.push_back(dragged);

    dragged->modifiedDate = time(NULL);

    refreshTree();
    event->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->listWidget->viewport())
    {
        if(event->type() == QEvent::Drop)
        {
            dropEvent(static_cast<QDropEvent*>(event));
            return true;
        }
        if(event->type() == QEvent::DragEnter)
        {
            dragEnterEvent(static_cast<QDragEnterEvent*>(event));
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

//Mas o menos explicacion de lo que quiero hacer para mover nodos con drag'n drop
/*
        Revisa nodo primero , encuentra padre ,elimina old padre,lo mete al nuevo padre.
        Osea , es como que mueve el nodo en el arbol
*/

void MainWindow::setViewMode(int mode)
{
    if(mode == 0) // LISTA
    {
        ui->listWidget->setViewMode(QListView::ListMode);

        minSize = 16;
        maxSize = 32;

        currentIconSize = 16;

        ui->listWidget->setGridSize(QSize()); // 🔥 importante
        ui->listWidget->setSpacing(2);
    }
    else if(mode == 1) // ICONOS
    {
        ui->listWidget->setViewMode(QListView::IconMode);

        minSize = 32;
        maxSize = 96;

        currentIconSize = 48;

        ui->listWidget->setSpacing(5);
        ui->listWidget->setGridSize(QSize(currentIconSize + 20, currentIconSize + 30));
    }
    else if(mode == 2) // GRANDES
    {
        ui->listWidget->setViewMode(QListView::IconMode);

        minSize = 64;
        maxSize = 128;

        currentIconSize = 96;

        ui->listWidget->setSpacing(8);
        ui->listWidget->setGridSize(QSize(currentIconSize + 30, currentIconSize + 40));
    }

    if(currentIconSize > maxSize)
        currentIconSize = maxSize;

    if(currentIconSize < minSize)
        currentIconSize = minSize;

    ui->listWidget->setIconSize(QSize(currentIconSize, currentIconSize));
}

//cosas para nombre

string MainWindow::trim(string str)
{
    int start = 0;
    while(start < str.size() && isspace(str[start]))
        start++;

    int end = str.size() - 1;
    while(end >= 0 && isspace(str[end]))
        end--;

    if(start > end) return "";

    return str.substr(start, end - start + 1);
}

bool MainWindow::isValidName(string name, bool isFile)
{
    name = trim(name);

    if(name.empty()){
        return false;
    }

    bool soloPuntitos = true;

    for(char c : name)
    {
        if(c != '.')
        {
            soloPuntitos = false;
            break;
        }
    }

    if(soloPuntitos)
    {
        return false;
    }

    std::string invalid = "*?<>|/";

    for(int i = 0; i < name.size(); i++)
    {
        if(invalid.find(name[i]) != string::npos)
            return false;
    }

    /*if(!isFile)
    {
        if(name.find('.') != string::npos)
            return false;
    }*/

    if(name.size() >= 4 && name.substr(name.size() - 4) == ".txt")
    {
        return false;
    }

    return true;
}
