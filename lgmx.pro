CONFIG += qt
CONFIG += debug

SOURCES += ./src/core/main.cpp ./src/core/main_window.cpp ./src/core/code_editor.cpp ./src/core/src_container.cpp \
           ./src/core/highlight/cpp/cpp_hl.cpp go_to_line.cpp ./src/core/src_file.cpp \
           config.cpp unsvFileDialog.cpp file_watcher.cpp ./src/core/recent_files.cpp search.cpp ./src/core/clipboard.cpp ./src/core/view_manager.cpp \
           ./src/core/view.cpp ./src/core/status_line.cpp ./src/utils/id.cpp ./src/core/file_type.cpp ./src/core/plugin_manager.cpp \
           ./src/core/highlight/highlight_manager.cpp ./src/core/vjson/block_allocator.cpp \
           ./src/core/vjson/json.cpp ./src/core/settings.cpp ./src/core/c_label.cpp ./src/core/text_manip.cpp ./src/core/cli/cli.cpp \
           command.cpp ./src/core/highlight/syntax_highlighter.cpp ./src/core/tags/tags.cpp ./src/core/tags/readtags.cpp
           
HEADERS += debug.h ./src/core/code_editor.h ./src/core/main_window.h ./src/core/src_container.h ./src/core/highlight/cpp/cpp_hl.h \
           go_to_line.h ui_go_to_line.h \
           ./src/core/src_file.h config.h unsvFileDialog.h file_watcher.h ./src/core/recent_files.h search.h ./src/core/clipboard.h ./src/core/view_manager.h \
           ./src/core/view.h ./src/core/status_line.h utils.h ./src/utils/id.h ./src/core/exception.h ./src/core/interfaces.h ./src/core/file_type.h \
           ./src/core/plugin_manager.h ./src/core/highlight/highlight_manager.h \ 
           ./src/core/vjson/block_allocator.h ./src/core/vjson/json.h ./src/core/settings.h ./src/core/c_label.h ./src/core/text_manip.h ./src/core/cli/cli.h \
           command.h ./src/core/highlight/syntax_highlighter.h ./src/core/tags/tags.h ./src/core/tags/readtags.h

INCLUDEPATH += ./src/utils ./src/core ./src/core/highlight ./src/core/tags
INCLUDEPATH += /usr/local/include ./src/core/vjson ./src/core/cli
           
TARGET = editor       

LIBS = -L./plugins -llgmx_cpp_highlighter -lfl
LIBS += -L/usr/local/lib -lsource-highlight-qt4

#QT += xml
