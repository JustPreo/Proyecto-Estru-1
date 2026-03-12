/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *goToFatherButton;
    QPushButton *backButton;
    QPushButton *ForwardButton;
    QPushButton *incrementSize;
    QPushButton *decrementSize;
    QLineEdit *pathLabel;
    QPushButton *goButton;
    QHBoxLayout *horizontalLayout_3;
    QTreeWidget *treeView;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(649, 368);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        goToFatherButton = new QPushButton(centralwidget);
        goToFatherButton->setObjectName("goToFatherButton");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::GoUp));
        goToFatherButton->setIcon(icon);

        horizontalLayout->addWidget(goToFatherButton);

        backButton = new QPushButton(centralwidget);
        backButton->setObjectName("backButton");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));
        backButton->setIcon(icon1);

        horizontalLayout->addWidget(backButton);

        ForwardButton = new QPushButton(centralwidget);
        ForwardButton->setObjectName("ForwardButton");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
        ForwardButton->setIcon(icon2);

        horizontalLayout->addWidget(ForwardButton);

        incrementSize = new QPushButton(centralwidget);
        incrementSize->setObjectName("incrementSize");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn));
        incrementSize->setIcon(icon3);

        horizontalLayout->addWidget(incrementSize);

        decrementSize = new QPushButton(centralwidget);
        decrementSize->setObjectName("decrementSize");
        QIcon icon4(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut));
        decrementSize->setIcon(icon4);

        horizontalLayout->addWidget(decrementSize);

        pathLabel = new QLineEdit(centralwidget);
        pathLabel->setObjectName("pathLabel");

        horizontalLayout->addWidget(pathLabel);

        goButton = new QPushButton(centralwidget);
        goButton->setObjectName("goButton");

        horizontalLayout->addWidget(goButton);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        treeView = new QTreeWidget(centralwidget);
        treeView->setObjectName("treeView");

        horizontalLayout_3->addWidget(treeView);

        treeWidget = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setSortingEnabled(true);
        treeWidget->setWordWrap(false);
        treeWidget->setHeaderHidden(true);

        horizontalLayout_3->addWidget(treeWidget);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout_4->addWidget(label);

        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setReadOnly(true);

        verticalLayout_4->addWidget(plainTextEdit);


        horizontalLayout_3->addLayout(verticalLayout_4);


        verticalLayout_3->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 649, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        goToFatherButton->setText(QString());
        backButton->setText(QString());
        ForwardButton->setText(QString());
        incrementSize->setText(QString());
        decrementSize->setText(QString());
        goButton->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
