/********************************************************************************
** Form generated from reading UI file 'settingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGDIALOG_H
#define UI_SETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *editName;
    QLabel *label_2;
    QComboBox *combType;
    QLabel *label_3;
    QLineEdit *defLine;
    QComboBox *editDef;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_4;
    QLineEdit *editID;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(377, 215);
        gridLayout_2 = new QGridLayout(Form);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_2->addWidget(pushButton, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 1, 0, 1, 1);

        groupBox = new QGroupBox(Form);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        editName = new QLineEdit(groupBox);
        editName->setObjectName(QStringLiteral("editName"));

        gridLayout->addWidget(editName, 0, 1, 1, 3);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        combType = new QComboBox(groupBox);
        combType->setObjectName(QStringLiteral("combType"));

        gridLayout->addWidget(combType, 1, 1, 1, 3);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        defLine = new QLineEdit(groupBox);
        defLine->setObjectName(QStringLiteral("defLine"));

        gridLayout->addWidget(defLine, 2, 1, 1, 1);

        editDef = new QComboBox(groupBox);
        editDef->setObjectName(QStringLiteral("editDef"));

        gridLayout->addWidget(editDef, 2, 2, 1, 2);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 3, 1, 1, 2);

        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 3, 3, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        editID = new QLineEdit(groupBox);
        editID->setObjectName(QStringLiteral("editID"));

        gridLayout->addWidget(editID, 4, 1, 1, 3);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 2);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Form", "Ok", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Form", "Cancel", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Form", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", Q_NULLPTR));
        label->setText(QApplication::translate("Form", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", Q_NULLPTR));
        label_2->setText(QApplication::translate("Form", "\320\242\320\270\320\277", Q_NULLPTR));
        combType->clear();
        combType->insertItems(0, QStringList()
         << QApplication::translate("Form", "Int", Q_NULLPTR)
         << QApplication::translate("Form", "Double", Q_NULLPTR)
         << QApplication::translate("Form", "QString", Q_NULLPTR)
         << QApplication::translate("Form", "QHostAddress", Q_NULLPTR)
         << QApplication::translate("Form", "QList", Q_NULLPTR)
         << QApplication::translate("Form", "QVariant", Q_NULLPTR)
        );
        label_3->setText(QApplication::translate("Form", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265 \320\277\320\276 \321\203\320\274\320\276\320\273\321\207\320\260\320\275\321\216", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Form", "+", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Form", "-", Q_NULLPTR));
        label_4->setText(QApplication::translate("Form", "\320\230\320\264\320\265\320\275\321\202\320\270\321\204\320\270\320\272\320\260\321\202\320\276\321\200 ID", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGDIALOG_H
