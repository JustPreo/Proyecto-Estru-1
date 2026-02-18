/********************************************************************************
** Form generated from reading UI file 'explorer.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPLORER_H
#define UI_EXPLORER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Explorer
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *Botones;
    QPushButton *BackButton;
    QPushButton *NextButton;
    QPushButton *UpButton;
    QPushButton *RefreshButton;
    QComboBox *ListBox;
    QScrollArea *Carpetas;
    QWidget *scrollAreaWidgetContents;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QLineEdit *CosoPath;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Explorer)
    {
        if (Explorer->objectName().isEmpty())
            Explorer->setObjectName("Explorer");
        Explorer->resize(800, 600);
        centralwidget = new QWidget(Explorer);
        centralwidget->setObjectName("centralwidget");
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 402, 31));
        Botones = new QHBoxLayout(horizontalLayoutWidget);
        Botones->setObjectName("Botones");
        Botones->setContentsMargins(0, 0, 0, 0);
        BackButton = new QPushButton(horizontalLayoutWidget);
        BackButton->setObjectName("BackButton");

        Botones->addWidget(BackButton);

        NextButton = new QPushButton(horizontalLayoutWidget);
        NextButton->setObjectName("NextButton");

        Botones->addWidget(NextButton);

        UpButton = new QPushButton(horizontalLayoutWidget);
        UpButton->setObjectName("UpButton");

        Botones->addWidget(UpButton);

        RefreshButton = new QPushButton(horizontalLayoutWidget);
        RefreshButton->setObjectName("RefreshButton");

        Botones->addWidget(RefreshButton);

        ListBox = new QComboBox(horizontalLayoutWidget);
        ListBox->addItem(QString());
        ListBox->addItem(QString());
        ListBox->setObjectName("ListBox");

        Botones->addWidget(ListBox);

        Carpetas = new QScrollArea(centralwidget);
        Carpetas->setObjectName("Carpetas");
        Carpetas->setGeometry(QRect(0, 30, 211, 531));
        Carpetas->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 209, 529));
        verticalLayoutWidget = new QWidget(scrollAreaWidgetContents);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 0, 211, 531));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(horizontalSpacer_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        Carpetas->setWidget(scrollAreaWidgetContents);
        CosoPath = new QLineEdit(centralwidget);
        CosoPath->setObjectName("CosoPath");
        CosoPath->setGeometry(QRect(410, 0, 391, 31));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(210, 30, 591, 551));
        Explorer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Explorer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        Explorer->setMenuBar(menubar);
        statusbar = new QStatusBar(Explorer);
        statusbar->setObjectName("statusbar");
        Explorer->setStatusBar(statusbar);

        retranslateUi(Explorer);

        QMetaObject::connectSlotsByName(Explorer);
    } // setupUi

    void retranslateUi(QMainWindow *Explorer)
    {
        Explorer->setWindowTitle(QCoreApplication::translate("Explorer", "MainWindow", nullptr));
        BackButton->setText(QCoreApplication::translate("Explorer", "<-", nullptr));
        NextButton->setText(QCoreApplication::translate("Explorer", "->", nullptr));
        UpButton->setText(QCoreApplication::translate("Explorer", "^", nullptr));
        RefreshButton->setText(QCoreApplication::translate("Explorer", "Refresh", nullptr));
        ListBox->setItemText(0, QCoreApplication::translate("Explorer", "List", nullptr));
        ListBox->setItemText(1, QCoreApplication::translate("Explorer", "Icons", nullptr));

        CosoPath->setText(QCoreApplication::translate("Explorer", "C:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Explorer: public Ui_Explorer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPLORER_H
