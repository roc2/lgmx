
SOURCES += main.cpp \
		   ui_main_window.cpp \
		   editor.cpp \
		   code_editor.cpp \
		   file.cpp \
		   src_area.cpp \
           highlight/cpp_hl.cpp \
           go_to_line.cpp \
           src_file.cpp \
           config.cpp
HEADERS += editor.h \
		   code_editor.h \
           ui_main_window.h \
           file.h \
           src_area.h \
           highlight/cpp_hl.h \
           go_to_line.h \
           ui_go_to_line.h \
           src_file.h \
           config.h
           
TARGET = editor       

QT += xml
