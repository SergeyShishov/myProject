/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include <QFile>
#include "settingsioclass.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    QStringList headers;
    QStringList headersSet;
    headers <<tr("Наименование")<<tr("Идентификатор")<<tr("Тип")<<tr("Значение по умолчанию");
    QString patchParam="Params.ini";
    model = new TreeModel(headers,patchParam);
    view->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
      view->resizeColumnToContents(column);
    stDlg=new settingDialog(nullptr);
    stDlg->hide();
    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateActions);
    connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);
    connect(view, &SqlTreeViewT::insertRow, this, &MainWindow::insertRow);
    connect(view, &SqlTreeViewT::removeRow, this, &MainWindow::removeRow);
    connect(view, &SqlTreeViewT::insertChild, this, &MainWindow::insertChild);
    connect(view, &SqlTreeViewT::editRows, this, &MainWindow::editRows);
    GlobalSettings.loadSettings();
    updateActions();
}

MainWindow::~MainWindow(){
    delete model;
    GlobalSettings.saveSettings();
}

ConteinerParam* MainWindow::getDialog(ConteinerParam* param){
  bool editCurrent=false;
  if(!param)
    param =new ConteinerParam();
  else
    editCurrent=true;
  //строим модель
  param->editing=false;
  stDlg->setSettings(param);
  stDlg->setWindowFlags(Qt::Dialog);
  stDlg->setWindowModality( Qt::WindowModal);
  stDlg->exec();
  if(!param->editing&&!editCurrent) {
    delete param;
    return nullptr;
  };
  return param;
}

void MainWindow::insertChild(QTreeView* tabs)
{
  ConteinerParam* param=getDialog();
  if(!param) return ;
  QModelIndex index = tabs->selectionModel()->currentIndex();
  TreeModel* tm=reinterpret_cast<TreeModel*>(tabs->model());
  tm->Add(index,0,QVariant::fromValue(param));
  view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
  updateActions();
}

void MainWindow::insertRow(QTreeView* tabs)
{
  ConteinerParam* param=getDialog();
  if(!param) return ;
  QModelIndex index = tabs->selectionModel()->currentIndex();
  TreeModel* tm=reinterpret_cast<TreeModel*>(tabs->model());
  tm->Add(index.parent(),index.row()+1,QVariant::fromValue(param));
  updateActions();
}

void MainWindow::editRows(QTreeView* tabs)
{
  ConteinerParam* param;
  QModelIndex index = tabs->selectionModel()->currentIndex();
  TreeModel* tm=reinterpret_cast<TreeModel*>(tabs->model());
  param=getDialog(tm->getItem(index)->itemParam);
  if(!param) return ;
  tm->setData(index,QVariant::fromValue(param),Qt::EditRole);
  updateActions();
}

void MainWindow::removeRow(QTreeView* tabs)
{
  QMessageBox *msgBox = new QMessageBox(QMessageBox::Information,"Внимание!","Вы действительно хотите удалить ветку?",QMessageBox::Ok|QMessageBox::Cancel);
  int mbok= msgBox->exec();
  delete msgBox;
  if(mbok==QMessageBox::Cancel) return;
  QModelIndex index = tabs->selectionModel()->currentIndex();
  TreeModel* model=reinterpret_cast<TreeModel*>(tabs->model());
  if (model->removeRow(index.row(), index.parent()))
    updateActions();
}

void MainWindow::updateActions()
{
  view->updateActions();
    bool hasCurrent = view->selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        int row = view->selectionModel()->currentIndex().row();
        int column = view->selectionModel()->currentIndex().column();
        if (view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

void MainWindow::on_action_triggered()
{
  TreeModel* tm=reinterpret_cast<TreeModel*>(view->model());
  tm->REloadSEttings();
}
