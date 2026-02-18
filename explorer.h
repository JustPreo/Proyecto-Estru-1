#ifndef EXPLORER_H
#define EXPLORER_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "HistoryList.h"
#include "filesystem.h"

namespace Ui {
class Explorer;
}

class Explorer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Explorer(QWidget *parent = nullptr);
    ~Explorer();

private slots:
    void on_BackButton_clicked();
    void on_NextButton_clicked();
    void showContextMenu(const QPoint &pos);
    void onItemDoubleClicked(QListWidgetItem* item);

private:
    void refreshView();
    void createFolder();
    void createFile();

    Ui::Explorer *ui;
    HistoryList history;
    FileSystem fs;
};

#endif
