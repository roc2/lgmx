CONFIG += qt
CONFIG += debug

SOURCES += ./src/core/main.cpp ui_main_window.cpp ./src/core/code_editor.cpp ./src/core/src_container.cpp \
           ./src/core/highlight/cpp/cpp_hl.cpp go_to_line.cpp ./src/core/src_file.cpp \
           config.cpp unsvFileDialog.cpp file_watcher.cpp recent_files.cpp search.cpp ./src/core/clipboard.cpp view_manager.cpp \
           ./src/core/view.cpp status_line.cpp ./src/utils/id.cpp ./src/core/file_type.cpp ./src/core/plugin_manager.cpp \
           ./src/core/highlight/highlight_manager.cpp ./src/core/vjson/block_allocator.cpp \
           ./src/core/vjson/json.cpp ./src/core/settings.cpp ./src/core/c_label.cpp ./src/core/text_manip.cpp cli.cpp \
           command.cpp ./src/core/highlight/syntax_highlighter.cpp
           
HEADERS += debug.h ./src/core/code_editor.h ui_main_window.h ./src/core/src_container.h ./src/core/highlight/cpp/cpp_hl.h \
           go_to_line.h ui_go_to_line.h \
           ./src/core/src_file.h config.h unsvFileDialog.h file_watcher.h recent_files.h search.h ./src/core/clipboard.h view_manager.h \
           ./src/core/view.h status_line.h utils.h ./src/utils/id.h ./src/core/exception.h ./src/core/interfaces.h ./src/core/file_type.h \
           ./src/core/plugin_manager.h ./src/core/highlight/highlight_manager.h ./src/core/highlight/highlighter.h \ 
           ./src/core/vjson/block_allocator.h ./src/core/vjson/json.h ./src/core/settings.h ./src/core/c_label.h ./src/core/text_manip.h cli.h \
           command.h ./src/core/highlight/syntax_highlighter.h

INCLUDEPATH += ./src/utils ./src/core ./src/core/highlight
INCLUDEPATH += /usr/local/include ./src/core/vjson
           
TARGET = editor       

LIBS = -L./plugins -llgmx_cpp_highlighter -lfl
LIBS += -L/usr/local/lib -lsource-highlight-qt4

QT += xml
