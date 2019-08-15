#include "settingdialog.h"
#include "ui_settingdialog.h"
#include  <QMessageBox>
#include <QHostAddress>
#include <QList>
#include <QInputDialog>

QString ConteinerParam::toString(TypeSet type){
  switch (type) {
    case TypeSet::Int:return "Integer";
    case TypeSet::QList:return "QList";
    case TypeSet::Double:return "Double";
    case TypeSet::QString:return "QString";
    case TypeSet::QHostAddress:return "QHostAddress";
    default:return "QVariant";
  }
}

bool ConteinerParam::verifyType(QVariant& value,TypeSet type){
  bool ok=false;
  switch (type) {
    case TypeSet::Int: {
        value.toInt(&ok);
        break;
      }
    case TypeSet::QList: {
        QList<QVariant> lst=value.toList();
        ok=lst.isEmpty();
        break;
      }
    case TypeSet::Double: {
        value.toDouble(&ok);
        break;
      }
    case TypeSet::QString: {
        value.toString();
        ok=true;
        break;
      }
    case TypeSet::QHostAddress: {
        QHostAddress addr=QHostAddress(value.toString());
        ok=!addr.isNull();
        break;
      }
    default: {
        ok=true;
      }
  }
  return ok;
}

settingDialog::settingDialog(QDialog *parent): QDialog(parent), ui(new Ui::Form)
{
  ui->setupUi(this);
}

void settingDialog::getSettings(){
  if(!paramSetting) return ;
  paramSetting->tpVal=static_cast<TypeSet>(ui->combType->currentIndex());
  paramSetting->ParName=ui->editName->text();
  if(static_cast<TypeSet>(ui->combType->currentIndex())==TypeSet::QList)
    paramSetting->value=ui->editDef->itemText(ui->editDef->currentIndex());
  else
    paramSetting->value=ui->defLine->text();
  paramSetting->uniqueId=ui->editID->text();
  paramSetting->editing=true;
}

void settingDialog::setSettings(ConteinerParam* prm){
  paramSetting=prm;
  ui->editDef->clear();
  ui->editDef->addItems(paramSetting->lstValue);
  ui->editName->setText(prm->ParName);
  ui->editID->setText(prm->uniqueId);
  if(prm->tpVal==TypeSet::QList){
  int pos=0;
  for (auto it=paramSetting->lstValue.begin();it!=paramSetting->lstValue.end();it++,pos++)
    if((*it)==paramSetting->value){
      break;
    }
  ui->editDef->setCurrentIndex(pos);
  } else {
    ui->defLine->setText(paramSetting->value.toString());
  }
  ui->combType->setCurrentIndex(static_cast<int>(prm->tpVal));
  on_combType_currentIndexChanged(ui->combType->currentIndex());
}

void settingDialog::on_pushButton_clicked(bool checked)
{
  QString et=ui->editID->text().trimmed();
  if(et==""){
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information,"Внимание!","Необходим ввод уникального идентификатора",QMessageBox::Ok);
    msgBox->exec();
    delete msgBox;
    ui->editID->setFocus();
    return ;
  }
    QVariant st=ui->defLine->text();
  if(static_cast<TypeSet>(ui->combType->currentIndex())==TypeSet::QList)
   st=ui->editDef->itemText(ui->editDef->currentIndex());
  if(!paramSetting->verifyType(st,static_cast<TypeSet>(ui->combType->currentIndex()))){
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information,"Внимание!",
                                          "Значение не соответствует типу "+paramSetting->toString(static_cast<TypeSet>(ui->combType->currentIndex())),QMessageBox::Ok);
    msgBox->exec();
    delete msgBox;
    ui->editDef->setFocus();
    return ;
  }
  if (ui->editID->text().size() != ui->editID->text().toUtf8().size()){
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information,"Внимание!","Идентификатор должен состоятьтолько из латинских символов!",QMessageBox::Ok);
    msgBox->exec();
    delete msgBox;
    ui->editID->setFocus();
    return ;
  }
  getSettings();
  this->close();
}

void settingDialog::on_pushButton_2_clicked(bool checked)
{
    this->close();
}

void settingDialog::on_pushButton_3_clicked(bool checked)
{
  bool bOk;
  QString str = QInputDialog::getText( 0, "Input", "Name:",
                                       QLineEdit::Normal,
                                       "",&bOk);
  if (bOk&&str.trimmed()!="") {
      ui->editDef->clear();
      paramSetting->lstValue.push_back(str);
      ui->editDef->addItems(paramSetting->lstValue);
  }
}

void settingDialog::on_combType_currentIndexChanged(int index)
{
  ui->pushButton_3->setVisible(static_cast<TypeSet>(index)==TypeSet::QList);
  ui->pushButton_4->setVisible(static_cast<TypeSet>(index)==TypeSet::QList);
  ui->editDef->setVisible(static_cast<TypeSet>(index)==TypeSet::QList);
  ui->defLine->setVisible(static_cast<TypeSet>(index)!=TypeSet::QList);
}

void settingDialog::on_pushButton_4_clicked(bool checked)
{
  paramSetting->lstValue.erase(paramSetting->lstValue.begin()+ui->editDef->currentIndex());
  ui->editDef->clear();
  ui->editDef->addItems(paramSetting->lstValue);
}
