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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QTreeView>
#include <QSettings>
#include "settingdialog.h"
#include <QAction>

//представление
class SqlTreeViewT: public QTreeView
{
  Q_OBJECT
public:
  SqlTreeViewT(QWidget *parent = 0);
  void drawTree(QPainter *painter, const QRegion &region) const;
  void drawRow(QPainter *painter,
                       const QStyleOptionViewItem &options,
                       const QModelIndex &index) const;
  void drawBranches(QPainter *painter,
                            const QRect &rect,
                            const QModelIndex &index) const;
  void updateActions();
signals:
  void insertRow(QTreeView* tabs);
  void removeRow(QTreeView* tabs);
  void insertChild(QTreeView* tabs);
  void editRows(QTreeView* tabs);
private:
  QAction* insertRowAction;
  QAction* removeRowAction;
  QAction* insertChildAction;
  QAction* editRow;
};

//элемент узла
class TreeItem
{
public:
    ConteinerParam* itemParam;
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(int column, const QVariant &value);
    void setCheckState(Qt::CheckState st);
    Qt::CheckState checkState() { return checkSt; }
    void setTreeValue(TreeItem*  root,QSettings& s,int& level);
    //поиск корневого элемента
    inline TreeItem* getRoot()
    {
      TreeItem* parent=this->parentItem;
      if(parent)
      while (parent->parentItem)
       {
         parent=parent->parentItem;
       }
      return (parent? parent:this);
    }
    //поиск элемента
    inline TreeItem* find(const QString &unique, TreeItem*  root=nullptr){
      TreeItem* att=nullptr;
      if(!root)
         root = getRoot();
      if(root)
        for(auto it=root->childItems.begin();it!=root->childItems.end()&&(!att);it++)
        {
          if((*it)->itemParam->uniqueId==unique){
            att=(*it);
            return att;
          }
          att=find(unique, *it);
          if(att) break;
        }
      return att;
    }
    //построение названия
    inline void buildIdName(QString&att,TreeItem*  root=nullptr){
      att="";
      if(root)
        while (root->parent()){
          if(root->itemParam)
            att+=root->itemParam->ParName;
          root=root->parent();
        }
    }
    inline QList<TreeItem*>* getChild(){return &this->childItems;}
    QVariant headData(int column) const;
    //очистка узла
    inline void clear(){
      int size=childItems.size()-1;
      for(int it=size;it>=0; it--){
        TreeItem* item=childItems.at(it);
        delete (item);
        item=nullptr;
      }
      childItems.clear();
    }
    //количество дочерних элементов в узле
    inline int childCount(){return childItems.size();}
private:
    Qt::CheckState checkSt;    
    QList<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;
};
//! [0]

#endif // TREEITEM_H
