#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QVariant>

namespace Ui {
  class Form;
}

enum class TypeSet{
  Int,
  Double,
  QString,
  QHostAddress,
  QList,
  QVariant
};

//Структура данных настроек(параметров)
struct ConteinerParam {
  QString ParName="";
  TypeSet tpVal=TypeSet::Int;
  QString uniqueId="";
  QVariant value=0;
  QList<QString>lstValue;
  bool editing=false;
  //проверка соответствия типу
  bool verifyType(QVariant& value,TypeSet type);
  //Тип
  QString toString(TypeSet type);
};

class settingDialog : public QDialog
{
  Q_OBJECT
    Ui::Form *ui;
public:
  explicit settingDialog(QDialog *parent = nullptr);
    //получение параметров
  void getSettings();
  //загрузка параметра
  void setSettings(ConteinerParam* prm);

public slots:
private slots:
  //кнопка ок
  void on_pushButton_clicked(bool checked);
  //кнопка отмена
  void on_pushButton_2_clicked(bool checked);
   //добавление элемента комбо списка
  void on_pushButton_3_clicked(bool checked);
//выбор типа параметра
  void on_combType_currentIndexChanged(int index);
//удаление из комбо списка
  void on_pushButton_4_clicked(bool checked);

private:
  ConteinerParam* paramSetting;
};

Q_DECLARE_METATYPE(ConteinerParam*)

#endif // DIALOGSETTINGS_H
