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

#include <QtWidgets>
#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>
#include "settingsioclass.h"
#include <QVariant>
#define MARKER "-"

//Группы в файле ini
void getSettingGroup(QSettings& setting, QSet<QString>& list){
  QString key;
  QStringList keylist=setting.allKeys();
  foreach(key,keylist){
    if(key.trimmed()!=""){
      QString str= key.split("/").takeAt(0);
      list.insert(str);
    }
  }
}

TreeModel::TreeModel(const QStringList &headers,const QString& stpth, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;
    setting=new QSettings( stpth, QSettings::IniFormat );
    rootItem = new TreeItem(rootData);    
    REloadSEttings();
}

TreeModel::~TreeModel()
{
  save(getItem(QModelIndex()),"",2);
  delete rootItem;
}

void TreeModel::REloadSEttings(){
  // загрузка настроек
  setting->setIniCodec( "UTF-8" );
  setupModelData(getItem(QModelIndex()));
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
      return QVariant();
  switch (role)
    {
   /* case Qt::CheckStateRole:
      if (index.column() == 0) {
        TreeItem *item = getItem(index);
          return item->checkState();
      } else return QVariant();*/
    case Qt::DisplayRole:
    case Qt::EditRole:
    //case Qt::DecorationRole:
      {
        TreeItem *item = getItem(index);
        return item->data(index.column());
      }
    default : {
       // qDebug()<<"role: "<<role;
        return QVariant();
      };
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
      Qt::ItemFlags flags = Qt::ItemIsEditable | QAbstractItemModel::flags(index);
      if (index.column() == 0) flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
      return flags;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->headData(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();
    return success;
}

bool TreeModel::Add(QModelIndex index,int position,const QVariant& param){
  TreeItem *parentItem = getItem(index);
  ConteinerParam* prm=qvariant_cast<ConteinerParam*>(param);
  if(rootItem->find(prm->uniqueId)){
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information,"Внимание!","Повторение уникального идентификатора!",QMessageBox::Ok);
    msgBox->exec();
    delete msgBox;
    return false;
  }
  if(!insertRow(parentItem->childCount(), index))
    return false;
  TreeItem *childItem = parentItem->child(parentItem->childCount()-1);  
  bool result = childItem->setData(index.column(), param);
  if(result)
    save(childItem,childItem->itemParam->uniqueId,0);

  if(result)
    emit dataChanged(index, index);
  return true;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;
    QString key="";
    if(parentItem->child(position)->itemParam)
      key= parentItem->child(position)->itemParam->uniqueId;
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    save(parentItem,key,1);
    return success;
}

void TreeModel::save(TreeItem*  root,const QString& key,int kind){
  switch(kind){
  case 0:{
        setSettingsToFile(root);
        break;
      }
  case 1:{
        setting->remove(key);
        break;
      }
  case 2:{
        setting->clear();
        QSet<QString> keylist;
        getSettingGroup(*setting, keylist);
        for (auto it = keylist.begin(); it != keylist.end(); ++it)
        {
          setting->remove(*it);
        }
        setTreeValue(rootItem);
        break;
      }
  }
  QStringList lst;
  serialize(rootItem, lst);
  setting->beginGroup("treeSettings");
  setting->setValue("TREE_STRUCT",lst.join("\n"));
  setting->endGroup();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

void TreeModel::setTreeFlag(TreeItem *item, const QModelIndex &index, Qt::CheckState flag,int level)
{
  if (item==nullptr) return ;
  int n = item->childCount();
  item->setCheckState(flag);
  for (int i=0; i<n; i++)
    {
      setTreeFlag(item->child(i),index, flag,++level);
    }
  TreeItem* parent = item->parent();
  if (parent!=nullptr&&level==0)
    {
      n = parent->childCount();
      int k=1;
      for (int i=0; i<n; i++)
        {
          if (parent->child(i)!=item&&parent->child(i)->checkState()==flag)
            k++;
        }
      if (k==n||(k==1&&flag==Qt::Checked)) parent->setCheckState(flag);
    }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole&&role!= Qt::CheckStateRole)
       return false;
  TreeItem *item = getItem(index);

  if (role == Qt::CheckStateRole&&index.column()==0)
     {
      setTreeFlag(item,index, (item->checkState()==Qt::Checked? Qt::Unchecked:Qt::Checked));
      QModelIndex valueIndex = this->index(index.row(), 1, index.parent());
      emit dataChanged(index, valueIndex);
       return true;
     }
  bool result = item->setData(index.column(), value);
  if (result)
    save(item, item->itemParam->uniqueId,0);

  if (result)
      emit dataChanged(index, index);
  return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

//сохранение дерева параметров
void TreeModel::setTreeValue(TreeItem*  root){
  if(root)
    for(auto it=root->getChild()->begin(); it!=root->getChild()->end();it++)
    {
      setSettingsToFile(*it);
      setTreeValue(*it);
    }
}

void TreeModel::setSettingsToFile(TreeItem*  it){
  if(it->itemParam){
    setting->beginGroup(it->itemParam->uniqueId);
    //QMap<QString,QVariant> qmaps;
    setting->setValue("uniqueID",it->itemParam->uniqueId);
    setting->setValue("paramName",it->itemParam->ParName);
    setting->setValue("typeValue",static_cast<int>(it->itemParam->tpVal));
    setting->setValue("paramValue",it->itemParam->value);
    setting->setValue("paramList",it->itemParam->lstValue.join("\n"));
    //setting->setValue((*it)->itemParam->uniqueId, qmaps);
    setting->endGroup();
  }
}

ConteinerParam* TreeModel::getKeySettings(const QString& Key){
  ConteinerParam* param =new ConteinerParam();
  //строим модель
  param->editing=false;
  setting->beginGroup(Key);
  //QMap<QString, QVariant> readMap = setting->value(key).toMap();
  param->ParName=setting->value("paramName").toString();
  param->tpVal=static_cast<TypeSet>(setting->value("tpVal").toInt());
  param->uniqueId=setting->value("uniqueId").toString();
  param->value=setting->value("paramValue");
  param->lstValue=setting->value("paramList").toString().split("\n");
  setting->endGroup();
  return param;
}

void TreeModel::setupModelData(TreeItem *parent)
{
  if(parent){
    beginRemoveRows(QModelIndex(), 0, parent->childCount());
    bool success = parent->removeChildren(0, parent->childCount());
    endRemoveRows();
  }
  parent->clear();  
  setting->beginGroup("treeSettings");
  QStringList lst =setting->value("TREE_STRUCT").toString().split("\n");
  lst.push_back("END_LIST");
  QList<QString>::const_iterator fp=lst.begin();
  setting->endGroup();
  deSerialize(rootItem, fp);
}

void TreeModel::serialize(TreeItem *root, QStringList &fp)
{
    if(root->itemParam)
      fp.push_back(tr(root->itemParam->uniqueId.toUtf8()));
    for(auto it=root->getChild()->begin(); it!=root->getChild()->end();it++)
    {
       serialize(*it, fp);
    }
    fp<<tr(MARKER);
}

void TreeModel::deSerialize(TreeItem *&root, QList<QString>::const_iterator &fp)
{
    QString val=(*fp);
    TreeItem* node=root;
    while(val!="END_LIST"&&(node)){
      if(val == MARKER){
        node=node->parent();
      }
      else {
        node->insertChildren(node->childCount(), 1, rootItem->columnCount());
        node= node->child(node->childCount()-1);
        node->itemParam=getKeySettings(val);
      }
     fp++;
     val=(*fp);
    }
}
