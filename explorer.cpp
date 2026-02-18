#include "explorer.h"
#include "ui_explorer.h"

#include <QMenu>
#include <QAction>
#include <QInputDialog>

Explorer::Explorer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Explorer)
{
    ui->setupUi(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    connect(ui->listWidget, &QListWidget::itemDoubleClicked,
            this, &Explorer::onItemDoubleClicked);

    // Cargar sistema si existe
    fs.loadFromBin();

    // Iniciar historial en root
    history.visit(fs.getRoot());

    refreshView();
}

Explorer::~Explorer()
{
    delete ui;
}

// ---------------- NAVIGATION ----------------

void Explorer::on_BackButton_clicked()
{
    if (history.canGoBack()) {
        Node* node = history.goBack();
        fs.changeDirectory(node);
        refreshView();
    }
}

void Explorer::on_NextButton_clicked()
{
    if (history.canGoForward()) {
        Node* node = history.goForward();
        fs.changeDirectory(node);
        refreshView();
    }
}

// ---------------- CONTEXT MENU ----------------

void Explorer::showContextMenu(const QPoint &pos)
{
    //Tiene que crearse porque si no seria poner visible y not visible y que hueva con esos errores

    QMenu menu(this);

    QAction *createFolderAction = menu.addAction("Crear carpeta");
    QAction *createFileAction   = menu.addAction("Crear archivo");

    menu.addSeparator();

    QAction *selected = menu.exec(this->mapToGlobal(pos));

    if (selected == createFolderAction) {
        createFolder();
    }
    else if (selected == createFileAction) {
        createFile();
    }
}

// ---------------- CREATE FOLDER ----------------

void Explorer::createFolder()
{
    bool ok;
    QString name = QInputDialog::getText(
        this,
        "Nueva carpeta",
        "Nombre:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || name.isEmpty())
        return;

    fs.createFolder(name);
    fs.saveToBin();
    refreshView();
}

// ---------------- CREATE FILE ----------------

void Explorer::createFile()
{
    bool ok;
    QString name = QInputDialog::getText(
        this,
        "Nuevo archivo",
        "Nombre:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || name.isEmpty())
        return;

    fs.createFile(name);
    fs.saveToBin();
    refreshView();
}

// ---------------- REFRESH VIEW ----------------

void Explorer::refreshView()
{
    ui->listWidget->clear();

    Node* current = fs.getCurrent();

    for (int i = 0; i < current->children.size(); i++) {
        Node* child = current->children[i];

        QString displayName = child->name;

        if (child->isFolder()) {
            displayName = "[DIR] " + displayName;
        }

        ui->listWidget->addItem(displayName);
    }

    ui->CosoPath->setText(current->name);
}

// ---------------- DOUBLE CLICK ----------------

void Explorer::onItemDoubleClicked(QListWidgetItem* item)
{
    QString text = item->text();

    if (text.startsWith("[DIR] ")) {
        text = text.mid(6);
    }

    Node* current = fs.getCurrent();

    for (int i = 0; i < current->children.size(); i++) {
        Node* child = current->children[i];

        if (child->name == text && child->isFolder()) {
            fs.changeDirectory(child);
            history.visit(child);
            refreshView();
            break;
        }
    }
}
