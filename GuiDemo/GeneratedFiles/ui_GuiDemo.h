/********************************************************************************
** Form generated from reading UI file 'GuiDemo.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIDEMO_H
#define UI_GUIDEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GuiDemoClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *imageLabel;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QSlider *filterLevelSlider;
    QFrame *line;
    QLabel *label_3;
    QSlider *whiteLevelSlider;
    QSpacerItem *verticalSpacer;
    QTextBrowser *textBrowser;

    void setupUi(QMainWindow *GuiDemoClass)
    {
        if (GuiDemoClass->objectName().isEmpty())
            GuiDemoClass->setObjectName(QStringLiteral("GuiDemoClass"));
        GuiDemoClass->resize(830, 467);
        centralWidget = new QWidget(GuiDemoClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        imageLabel = new QLabel(centralWidget);
        imageLabel->setObjectName(QStringLiteral("imageLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(imageLabel->sizePolicy().hasHeightForWidth());
        imageLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(imageLabel, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, -1);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        filterLevelSlider = new QSlider(centralWidget);
        filterLevelSlider->setObjectName(QStringLiteral("filterLevelSlider"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(filterLevelSlider->sizePolicy().hasHeightForWidth());
        filterLevelSlider->setSizePolicy(sizePolicy1);
        filterLevelSlider->setMinimumSize(QSize(200, 0));
        filterLevelSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(filterLevelSlider);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        whiteLevelSlider = new QSlider(centralWidget);
        whiteLevelSlider->setObjectName(QStringLiteral("whiteLevelSlider"));
        sizePolicy1.setHeightForWidth(whiteLevelSlider->sizePolicy().hasHeightForWidth());
        whiteLevelSlider->setSizePolicy(sizePolicy1);
        whiteLevelSlider->setMinimumSize(QSize(200, 0));
        whiteLevelSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(whiteLevelSlider);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy2);
        textBrowser->setMinimumSize(QSize(200, 0));

        verticalLayout->addWidget(textBrowser);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        GuiDemoClass->setCentralWidget(centralWidget);

        retranslateUi(GuiDemoClass);

        QMetaObject::connectSlotsByName(GuiDemoClass);
    } // setupUi

    void retranslateUi(QMainWindow *GuiDemoClass)
    {
        GuiDemoClass->setWindowTitle(QApplication::translate("GuiDemoClass", "GuiDemo", Q_NULLPTR));
        imageLabel->setText(QString());
        label_2->setText(QApplication::translate("GuiDemoClass", "\347\243\250\347\232\256\347\250\213\345\272\246", Q_NULLPTR));
        label_3->setText(QApplication::translate("GuiDemoClass", "\347\276\216\347\231\275\347\250\213\345\272\246", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GuiDemoClass: public Ui_GuiDemoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIDEMO_H
