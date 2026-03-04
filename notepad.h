#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include "file.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit notepad(File* file, QWidget *parent = nullptr);
    ~notepad();

private slots:
    void on_actionSave_triggered();

private:
    Ui::Notepad *ui;
    File* currentFile;
};

#endif
