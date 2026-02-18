#include "notepad.h"
#include "ui_notepad.h"

#include <QFileDialog>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

notepad::notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists("data")) {
        dir.mkdir("data");
    }
}

notepad::~notepad()
{
    delete ui;
}

void notepad::on_actionSave_triggered()
{
    QString basePath = QCoreApplication::applicationDirPath();

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Guardar archivo",
        basePath,
        "Archivos de texto (*.txt)"
        );

    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo guardar el archivo");
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    QFileInfo info(filePath);
    setWindowTitle(info.fileName());
}
