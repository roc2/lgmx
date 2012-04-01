
SOURCES += main.cpp \
		   ui_main_window.cpp \
		   code_editor.cpp \
		   src_area.cpp \
           highlight/cpp_hl.cpp \
           go_to_line.cpp \
           src_file.cpp \
           config.cpp \
           unsvFileDialog.cpp \
           file_watcher.cpp \
           recent_files.cpp \
           search.cpp \
           clipboard.cpp
HEADERS += code_editor.h \
           ui_main_window.h \
           src_area.h \
           highlight/cpp_hl.h \
           go_to_line.h \
           ui_go_to_line.h \
           src_file.h \
           config.h \
           unsvFileDialog.h \
           file_watcher.h \
           recent_files.h \
           search.h \
           clipboard.h
           
TARGET = editor       

QT += xml
