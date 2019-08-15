#include "settingsioclass.h"

#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QSettings>

SettingsIOClass GlobalSettings;

QString SettingsIOClass::settingsPath;

void SettingsIOClass::loadSettings()
{
  QSettings s;
  s.setIniCodec( "UTF-8" );
  s.beginGroup("mainwidget");
  splitterState=s.value("splitterState", QByteArray()).toByteArray();
  mainWindowState=s.value("mainWindowState", QByteArray()).toByteArray();
  mainWindowGeometry=s.value("mainWindowGeometry", QByteArray()).toByteArray();
  s.endGroup();  
}

void SettingsIOClass::saveSettings()
{
  QSettings s;
  s.setIniCodec( "UTF-8" );
  s.beginGroup("mainwidget");
  s.setValue("splitterState", splitterState);
  s.setValue("mainWindowState", mainWindowState);
  s.setValue("mainWindowGeometry", mainWindowGeometry);
  s.endGroup();
  int level=0;
  s.sync();
}

void SettingsIOClass::initPaths()
{
  QDir appDir( qApp->applicationDirPath() );
  QDir shareDir=appDir;
  shareDir.cdUp();
  QString sharePath=shareDir.path();
  settingsPath=sharePath+"/libraruSettings";
  QSettings settings( "settings_demo.conf", QSettings::IniFormat );
}

