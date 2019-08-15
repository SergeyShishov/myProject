#ifndef SETTINGSIOCLASS_H
#define SETTINGSIOCLASS_H

#include <vector>
#include <QVector>
#include <QString>
#include <QList>
#include <QSettings>
#include <QVariant>
#include <QHostAddress>

using namespace std;
//настройки окна
class SettingsIOClass
{
public:
  // пути к каталогам
  static QString settingsPath;
  static QString devicesPath;
  static void initPaths();
  // загрузка настроек
  void loadSettings();
  void saveSettings();
  // параметры преобразования даты/времени
  QByteArray splitterState;
  QByteArray mainWindowState;
  QByteArray mainWindowGeometry;
};

extern SettingsIOClass GlobalSettings;
#endif // SETTINGSIOCLASS_H
