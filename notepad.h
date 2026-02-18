#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <string>
#include <QMainWindow>

using std::string;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class notepad : public QMainWindow
{
    Q_OBJECT

public:
    notepad(QWidget *parent = nullptr);
    ~notepad();

private slots:
    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    string currentFile = "";
};

#endif // NOTEPAD_H
