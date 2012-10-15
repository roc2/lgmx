CONFIG += qt
CONFIG += debug

SOURCES += ./src/core/main.cpp ui_main_window.cpp ./src/core/code_editor.cpp src_container.cpp highlight/cpp_hl.cpp go_to_line.cpp ./src/core/src_file.cpp \
           config.cpp unsvFileDialog.cpp file_watcher.cpp recent_files.cpp search.cpp clipboard.cpp view_manager.cpp \
           view.cpp status_line.cpp ./src/utils/id.cpp
           
HEADERS += debug.h ./src/core/code_editor.h ui_main_window.h src_container.h highlight/cpp_hl.h go_to_line.h ui_go_to_line.h \
           ./src/core/src_file.h config.h unsvFileDialog.h file_watcher.h recent_files.h search.h clipboard.h view_manager.h \
           view.h status_line.h utils.h ./src/utils/id.h ./src/core/exception.h ./src/core/interfaces.h

INCLUDEPATH += ./src/utils ./src/core
           
TARGET = editor       

LIBS = -L./plugins -llgmx_cpp_highlighter

QT += xml
