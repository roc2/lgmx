#############################################################################
# Makefile for building: editor
# Generated by qmake (2.01a) (Qt 4.8.1) on: Thu Jun 21 22:21:41 2012
# Project:  lgmx.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile lgmx.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtXml -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		ui_main_window.cpp \
		code_editor.cpp \
		src_container.cpp \
		highlight/cpp_hl.cpp \
		go_to_line.cpp \
		src_file.cpp \
		config.cpp \
		unsvFileDialog.cpp \
		file_watcher.cpp \
		recent_files.cpp \
		search.cpp \
		clipboard.cpp \
		view_manager.cpp \
		view.cpp \
		status_line.cpp moc_code_editor.cpp \
		moc_ui_main_window.cpp \
		moc_src_container.cpp \
		moc_cpp_hl.cpp \
		moc_go_to_line.cpp \
		moc_src_file.cpp \
		moc_unsvFileDialog.cpp \
		moc_file_watcher.cpp \
		moc_recent_files.cpp \
		moc_search.cpp \
		moc_clipboard.cpp \
		moc_view_manager.cpp \
		moc_view.cpp \
		moc_status_line.cpp
OBJECTS       = main.o \
		ui_main_window.o \
		code_editor.o \
		src_container.o \
		cpp_hl.o \
		go_to_line.o \
		src_file.o \
		config.o \
		unsvFileDialog.o \
		file_watcher.o \
		recent_files.o \
		search.o \
		clipboard.o \
		view_manager.o \
		view.o \
		status_line.o \
		moc_code_editor.o \
		moc_ui_main_window.o \
		moc_src_container.o \
		moc_cpp_hl.o \
		moc_go_to_line.o \
		moc_src_file.o \
		moc_unsvFileDialog.o \
		moc_file_watcher.o \
		moc_recent_files.o \
		moc_search.o \
		moc_clipboard.o \
		moc_view_manager.o \
		moc_view.o \
		moc_status_line.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		lgmx.pro
QMAKE_TARGET  = editor
DESTDIR       = 
TARGET        = editor

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: lgmx.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtXml.prl \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile lgmx.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtXml.prl:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile lgmx.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/editor1.0.0 || $(MKDIR) .tmp/editor1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/editor1.0.0/ && $(COPY_FILE) --parents debug.h code_editor.h ui_main_window.h src_container.h highlight/cpp_hl.h go_to_line.h ui_go_to_line.h src_file.h config.h unsvFileDialog.h file_watcher.h recent_files.h search.h clipboard.h view_manager.h view.h status_line.h .tmp/editor1.0.0/ && $(COPY_FILE) --parents main.cpp ui_main_window.cpp code_editor.cpp src_container.cpp highlight/cpp_hl.cpp go_to_line.cpp src_file.cpp config.cpp unsvFileDialog.cpp file_watcher.cpp recent_files.cpp search.cpp clipboard.cpp view_manager.cpp view.cpp status_line.cpp .tmp/editor1.0.0/ && (cd `dirname .tmp/editor1.0.0` && $(TAR) editor1.0.0.tar editor1.0.0 && $(COMPRESS) editor1.0.0.tar) && $(MOVE) `dirname .tmp/editor1.0.0`/editor1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/editor1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_code_editor.cpp moc_ui_main_window.cpp moc_src_container.cpp moc_cpp_hl.cpp moc_go_to_line.cpp moc_src_file.cpp moc_unsvFileDialog.cpp moc_file_watcher.cpp moc_recent_files.cpp moc_search.cpp moc_clipboard.cpp moc_view_manager.cpp moc_view.cpp moc_status_line.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_code_editor.cpp moc_ui_main_window.cpp moc_src_container.cpp moc_cpp_hl.cpp moc_go_to_line.cpp moc_src_file.cpp moc_unsvFileDialog.cpp moc_file_watcher.cpp moc_recent_files.cpp moc_search.cpp moc_clipboard.cpp moc_view_manager.cpp moc_view.cpp moc_status_line.cpp
moc_code_editor.cpp: highlight/cpp_hl.h \
		code_editor.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) code_editor.h -o moc_code_editor.cpp

moc_ui_main_window.cpp: code_editor.h \
		highlight/cpp_hl.h \
		src_container.h \
		src_file.h \
		status_line.h \
		go_to_line.h \
		ui_go_to_line.h \
		file_watcher.h \
		recent_files.h \
		search.h \
		view_manager.h \
		view.h \
		ui_main_window.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ui_main_window.h -o moc_ui_main_window.cpp

moc_src_container.cpp: code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		src_container.h \
		src_container.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src_container.h -o moc_src_container.cpp

moc_cpp_hl.cpp: highlight/cpp_hl.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) highlight/cpp_hl.h -o moc_cpp_hl.cpp

moc_go_to_line.cpp: ui_go_to_line.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		go_to_line.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) go_to_line.h -o moc_go_to_line.cpp

moc_src_file.cpp: code_editor.h \
		highlight/cpp_hl.h \
		src_file.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src_file.h -o moc_src_file.cpp

moc_unsvFileDialog.cpp: unsvFileDialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) unsvFileDialog.h -o moc_unsvFileDialog.cpp

moc_file_watcher.cpp: file_watcher.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) file_watcher.h -o moc_file_watcher.cpp

moc_recent_files.cpp: recent_files.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) recent_files.h -o moc_recent_files.cpp

moc_search.cpp: src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		search.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) search.h -o moc_search.cpp

moc_clipboard.cpp: src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		clipboard.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) clipboard.h -o moc_clipboard.cpp

moc_view_manager.cpp: view.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		recent_files.h \
		view_manager.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) view_manager.h -o moc_view_manager.cpp

moc_view.cpp: src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		view.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) view.h -o moc_view.cpp

moc_status_line.cpp: src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		status_line.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) status_line.h -o moc_status_line.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp ui_main_window.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_container.h \
		src_file.h \
		status_line.h \
		go_to_line.h \
		ui_go_to_line.h \
		file_watcher.h \
		recent_files.h \
		search.h \
		view_manager.h \
		view.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

ui_main_window.o: ui_main_window.cpp ui_main_window.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_container.h \
		src_file.h \
		status_line.h \
		go_to_line.h \
		ui_go_to_line.h \
		file_watcher.h \
		recent_files.h \
		search.h \
		view_manager.h \
		view.h \
		config.h \
		unsvFileDialog.h \
		clipboard.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ui_main_window.o ui_main_window.cpp

code_editor.o: code_editor.cpp code_editor.h \
		highlight/cpp_hl.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o code_editor.o code_editor.cpp

src_container.o: src_container.cpp src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o src_container.o src_container.cpp

cpp_hl.o: highlight/cpp_hl.cpp highlight/cpp_hl.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cpp_hl.o highlight/cpp_hl.cpp

go_to_line.o: go_to_line.cpp go_to_line.h \
		ui_go_to_line.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o go_to_line.o go_to_line.cpp

src_file.o: src_file.cpp src_file.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_container.h \
		status_line.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o src_file.o src_file.cpp

config.o: config.cpp config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o config.o config.cpp

unsvFileDialog.o: unsvFileDialog.cpp unsvFileDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o unsvFileDialog.o unsvFileDialog.cpp

file_watcher.o: file_watcher.cpp file_watcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o file_watcher.o file_watcher.cpp

recent_files.o: recent_files.cpp recent_files.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o recent_files.o recent_files.cpp

search.o: search.cpp search.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		view_manager.h \
		view.h \
		recent_files.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o search.o search.cpp

clipboard.o: clipboard.cpp clipboard.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o clipboard.o clipboard.cpp

view_manager.o: view_manager.cpp view_manager.h \
		view.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		recent_files.h \
		debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view_manager.o view_manager.cpp

view.o: view.cpp view.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h \
		status_line.h \
		view_manager.h \
		recent_files.h \
		debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view.o view.cpp

status_line.o: status_line.cpp status_line.h \
		src_container.h \
		code_editor.h \
		highlight/cpp_hl.h \
		src_file.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o status_line.o status_line.cpp

moc_code_editor.o: moc_code_editor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_code_editor.o moc_code_editor.cpp

moc_ui_main_window.o: moc_ui_main_window.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ui_main_window.o moc_ui_main_window.cpp

moc_src_container.o: moc_src_container.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_src_container.o moc_src_container.cpp

moc_cpp_hl.o: moc_cpp_hl.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_cpp_hl.o moc_cpp_hl.cpp

moc_go_to_line.o: moc_go_to_line.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_go_to_line.o moc_go_to_line.cpp

moc_src_file.o: moc_src_file.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_src_file.o moc_src_file.cpp

moc_unsvFileDialog.o: moc_unsvFileDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_unsvFileDialog.o moc_unsvFileDialog.cpp

moc_file_watcher.o: moc_file_watcher.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_file_watcher.o moc_file_watcher.cpp

moc_recent_files.o: moc_recent_files.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_recent_files.o moc_recent_files.cpp

moc_search.o: moc_search.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_search.o moc_search.cpp

moc_clipboard.o: moc_clipboard.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_clipboard.o moc_clipboard.cpp

moc_view_manager.o: moc_view_manager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_view_manager.o moc_view_manager.cpp

moc_view.o: moc_view.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_view.o moc_view.cpp

moc_status_line.o: moc_status_line.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_status_line.o moc_status_line.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

