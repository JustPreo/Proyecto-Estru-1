#include "notepad.h"
#include "ui_notepad.h"

#include <QMessageBox>

notepad::notepad(File* file, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Notepad),
    currentFile(file)
{
    ui->setupUi(this);

    if (currentFile != NULL)
    {
        ui->textEdit->setPlainText(
            QString::fromStdString(currentFile->content)
            );

        setWindowTitle(
            QString::fromStdString(currentFile->name)
            );
    }
}

notepad::~notepad()
{
    delete ui;
}

void notepad::on_actionSave_triggered()
{
    if (currentFile == NULL)
        return;

    currentFile->content = //guardar
        ui->textEdit->toPlainText().toStdString();

    currentFile->modifiedDate = time(NULL);

    QMessageBox::information(this, "Guardado", "Archivo guardado");
}
