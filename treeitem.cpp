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

/*
    A container for items of data supplied by the simple tree model.
*/

#include "treeitem.h"
#include <QtWidgets>
#include <QStringList>

//! [0]
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    itemParam=nullptr;
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
    delete itemParam;
    itemParam=nullptr;
}

TreeItem *TreeItem::child(int number)
{
    return childItems.value(number);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
  return itemData.count();
}

QVariant TreeItem::headData(int column) const {
  return itemData.value(column);
}

QVariant TreeItem::data(int column) const
{
  if(itemParam)
  switch(column)
    {
      case 0: return itemParam->ParName;
      case 1: return itemParam->uniqueId;
      case 2: return itemParam->toString(itemParam->tpVal);
      case 3: return itemParam->value;
      default: return QVariant();
    }
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool TreeItem::setData(int column, const QVariant &value)
{
    /*if (column < 0 || column >= itemData.size())
        return false;*/
    ConteinerParam* prm=qvariant_cast<ConteinerParam*>(value);
    if(!prm) return false;
    if(itemParam&&itemParam!=prm){
      delete itemParam;
      itemParam=nullptr;
    }
    itemParam=prm;
    itemParam->editing=false;
    return true;
}

void TreeItem::setCheckState(Qt::CheckState st)
{
  checkSt=st;
}

SqlTreeViewT::SqlTreeViewT(QWidget *parent)
{

  // Показать контекстное меню
      setContextMenuPolicy( Qt::ActionsContextMenu );
      setSelectionMode(QTreeView::ExtendedSelection);
      setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
      setSelectionBehavior(QAbstractItemView::SelectRows);
      setAlternatingRowColors(true);
      setEditTriggers(QAbstractItemView::DoubleClicked
                                             | QAbstractItemView::SelectedClicked);

      setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
      insertRowAction = new QAction(tr("Insert new row"), this);
      connect(insertRowAction, &QAction::triggered,this,[this]{
      emit this->insertRow(this);
      } );
      removeRowAction=new QAction(tr("Remove row"), this);
      connect(removeRowAction, &QAction::triggered,this,[this]{
      emit removeRow(this);
      } );
      insertChildAction=new QAction(tr("Insert new Child"), this);
      connect(insertChildAction, &QAction::triggered,this,[this]{
      emit insertChild(this);
      } );
      editRow=new QAction(tr("Edit Row"), this);
      connect(editRow, &QAction::triggered,this,[this]{
      emit editRows(this);
      } );
      // формируем контекстное меню
      QAction* sep=new QAction(this);
      sep->setSeparator(true);
      this->addAction(sep);
      this->addAction(insertRowAction);
      this->addAction(removeRowAction);
      this->addAction(insertChildAction);
      this->addAction(editRow);
}

void SqlTreeViewT::drawTree(QPainter *painter, const QRegion &region) const
{}

void SqlTreeViewT::drawRow(QPainter *painter,
                     const QStyleOptionViewItem &options,
                     const QModelIndex &index) const
{
  QTreeView::drawRow(painter, options, index);
}

void SqlTreeViewT::drawBranches(QPainter *painter,
                          const QRect &rect,
                          const QModelIndex &index) const
{
  QTreeView::drawBranches(painter, rect, index);
}

void SqlTreeViewT::updateActions()
{
    bool hasSelection = !this->selectionModel()->selection().isEmpty();
    removeRowAction->setEnabled(hasSelection);

    bool hasCurrent = this->selectionModel()->currentIndex().isValid();
    insertRowAction->setEnabled(hasCurrent);

    if (hasCurrent)
        this->closePersistentEditor(this->selectionModel()->currentIndex());
}
