QT += widgets network

FORMS       = mainwindow.ui \
    settingdialog.ui
HEADERS     = mainwindow.h \
              settingdialog.h \
              settingsioclass.h \
              treeitem.h \
              treemodel.h
RESOURCES   = editabletreemodel.qrc
SOURCES     = mainwindow.cpp \
              settingdialog.cpp \
              settingsioclass.cpp \
              treeitem.cpp \
              treemodel.cpp \
              main.cpp

INSTALLS += target
