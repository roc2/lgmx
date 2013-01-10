#############################################################################
# Makefile for building: editor
# Generated by qmake (2.01a) (Qt 4.8.1) on: Thu Jan 10 02:20:34 2013
# Project:  lgmx.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile lgmx.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Isrc/utils -Isrc/core -Isrc/core/highlight -I/usr/local/include -Isrc/core/vjson -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -L./plugins -llgmx_cpp_highlighter -lfl -L/usr/local/lib -lsource-highlight-qt4 -lQtXml -lQtGui -lQtCore -lpthread 
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

SOURCES       = src/core/main.cpp \
		ui_main_window.cpp \
		src/core/code_editor.cpp \
		src/core/src_container.cpp \
		src/core/highlight/cpp/cpp_hl.cpp \
		go_to_line.cpp \
		src/core/src_file.cpp \
		config.cpp \
		unsvFileDialog.cpp \
		file_watcher.cpp \
		recent_files.cpp \
		search.cpp \
		src/core/clipboard.cpp \
		view_manager.cpp \
		src/core/view.cpp \
		status_line.cpp \
		src/utils/id.cpp \
		src/core/file_type.cpp \
		src/core/plugin_manager.cpp \
		src/core/highlight/cpp/lex.yy.cpp \
		src/core/highlight/highlight_manager.cpp \
		src/core/vjson/block_allocator.cpp \
		src/core/vjson/json.cpp \
		src/core/settings.cpp \
		src/core/c_label.cpp \
		src/core/text_manip.cpp \
		cli.cpp \
		command.cpp moc_code_editor.cpp \
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
		moc_status_line.cpp \
		moc_highlighter.cpp \
		moc_c_label.cpp \
		moc_text_manip.cpp \
		moc_cli.cpp
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
		id.o \
		file_type.o \
		plugin_manager.o \
		lex.yy.o \
		highlight_manager.o \
		block_allocator.o \
		json.o \
		settings.o \
		c_label.o \
		text_manip.o \
		cli.o \
		command.o \
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
		moc_status_line.o \
		moc_highlighter.o \
		moc_c_label.o \
		moc_text_manip.o \
		moc_cli.o
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
		/usr/share/qt4/mkspecs/features/debug.prf \
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
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9]\+\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

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
		/usr/share/qt4/mkspecs/features/debug.prf \
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
/usr/share/qt4/mkspecs/features/debug.prf:
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
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/editor1.0.0/ && $(COPY_FILE) --parents debug.h src/core/code_editor.h ui_main_window.h src/core/src_container.h src/core/highlight/cpp/cpp_hl.h go_to_line.h ui_go_to_line.h src/core/src_file.h config.h unsvFileDialog.h file_watcher.h recent_files.h search.h src/core/clipboard.h view_manager.h src/core/view.h status_line.h utils.h src/utils/id.h src/core/exception.h src/core/interfaces.h src/core/file_type.h src/core/plugin_manager.h src/core/highlight/highlight_manager.h src/core/highlight/highlighter.h src/core/vjson/block_allocator.h src/core/vjson/json.h src/core/settings.h src/core/c_label.h src/core/text_manip.h cli.h command.h .tmp/editor1.0.0/ && $(COPY_FILE) --parents src/core/main.cpp ui_main_window.cpp src/core/code_editor.cpp src/core/src_container.cpp src/core/highlight/cpp/cpp_hl.cpp go_to_line.cpp src/core/src_file.cpp config.cpp unsvFileDialog.cpp file_watcher.cpp recent_files.cpp search.cpp src/core/clipboard.cpp view_manager.cpp src/core/view.cpp status_line.cpp src/utils/id.cpp src/core/file_type.cpp src/core/plugin_manager.cpp src/core/highlight/cpp/lex.yy.cpp src/core/highlight/highlight_manager.cpp src/core/vjson/block_allocator.cpp src/core/vjson/json.cpp src/core/settings.cpp src/core/c_label.cpp src/core/text_manip.cpp cli.cpp command.cpp .tmp/editor1.0.0/ && (cd `dirname .tmp/editor1.0.0` && $(TAR) editor1.0.0.tar editor1.0.0 && $(COMPRESS) editor1.0.0.tar) && $(MOVE) `dirname .tmp/editor1.0.0`/editor1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/editor1.0.0


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

compiler_moc_header_make_all: moc_code_editor.cpp moc_ui_main_window.cpp moc_src_container.cpp moc_cpp_hl.cpp moc_go_to_line.cpp moc_src_file.cpp moc_unsvFileDialog.cpp moc_file_watcher.cpp moc_recent_files.cpp moc_search.cpp moc_clipboard.cpp moc_view_manager.cpp moc_view.cpp moc_status_line.cpp moc_highlighter.cpp moc_c_label.cpp moc_text_manip.cpp moc_cli.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_code_editor.cpp moc_ui_main_window.cpp moc_src_container.cpp moc_cpp_hl.cpp moc_go_to_line.cpp moc_src_file.cpp moc_unsvFileDialog.cpp moc_file_watcher.cpp moc_recent_files.cpp moc_search.cpp moc_clipboard.cpp moc_view_manager.cpp moc_view.cpp moc_status_line.cpp moc_highlighter.cpp moc_c_label.cpp moc_text_manip.cpp moc_cli.cpp
moc_code_editor.cpp: src/core/code_editor.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/code_editor.h -o moc_code_editor.cpp

moc_ui_main_window.cpp: go_to_line.h \
		ui_go_to_line.h \
		file_watcher.h \
		recent_files.h \
		search.h \
		view_manager.h \
		cli.h \
		command.h \
		ui_main_window.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ui_main_window.h -o moc_ui_main_window.cpp

moc_src_container.cpp: src/core/src_container.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/src_container.h -o moc_src_container.cpp

moc_cpp_hl.cpp: src/core/highlight/cpp/cpp_hl.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/highlight/cpp/cpp_hl.h -o moc_cpp_hl.cpp

moc_go_to_line.cpp: ui_go_to_line.h \
		go_to_line.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) go_to_line.h -o moc_go_to_line.cpp

moc_src_file.cpp: src/core/src_file.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/src_file.h -o moc_src_file.cpp

moc_unsvFileDialog.cpp: unsvFileDialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) unsvFileDialog.h -o moc_unsvFileDialog.cpp

moc_file_watcher.cpp: file_watcher.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) file_watcher.h -o moc_file_watcher.cpp

moc_recent_files.cpp: recent_files.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) recent_files.h -o moc_recent_files.cpp

moc_search.cpp: search.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) search.h -o moc_search.cpp

moc_clipboard.cpp: view_manager.h \
		recent_files.h \
		src/core/clipboard.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/clipboard.h -o moc_clipboard.cpp

moc_view_manager.cpp: recent_files.h \
		view_manager.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) view_manager.h -o moc_view_manager.cpp

moc_view.cpp: src/core/view.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/view.h -o moc_view.cpp

moc_status_line.cpp: status_line.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) status_line.h -o moc_status_line.cpp

moc_highlighter.cpp: src/core/highlight/highlighter.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/highlight/highlighter.h -o moc_highlighter.cpp

moc_c_label.cpp: src/core/c_label.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/c_label.h -o moc_c_label.cpp

moc_text_manip.cpp: view_manager.h \
		recent_files.h \
		src/core/text_manip.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/text_manip.h -o moc_text_manip.cpp

moc_cli.cpp: command.h \
		cli.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) cli.h -o moc_cli.cpp

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

main.o: src/core/main.cpp ui_main_window.h \
		go_to_line.h \
		ui_go_to_line.h \
		file_watcher.h \
		recent_files.h \
		search.h \
		view_manager.h \
		cli.h \
		command.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o src/core/main.cpp

ui_main_window.o: ui_main_window.cpp ui_main_window.h \
		go_to_line.h \
		ui_go_to_line.h \
		file_watcher.h \
		recent_files.h \
		search.h \
		view_manager.h \
		cli.h \
		command.h \
		config.h \
		unsvFileDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ui_main_window.o ui_main_window.cpp

code_editor.o: src/core/code_editor.cpp debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o code_editor.o src/core/code_editor.cpp

src_container.o: src/core/src_container.cpp debug.h \
		view_manager.h \
		recent_files.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o src_container.o src/core/src_container.cpp

cpp_hl.o: src/core/highlight/cpp/cpp_hl.cpp src/core/highlight/cpp/cpp_hl.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cpp_hl.o src/core/highlight/cpp/cpp_hl.cpp

go_to_line.o: go_to_line.cpp go_to_line.h \
		ui_go_to_line.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o go_to_line.o go_to_line.cpp

src_file.o: src/core/src_file.cpp debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o src_file.o src/core/src_file.cpp

config.o: config.cpp config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o config.o config.cpp

unsvFileDialog.o: unsvFileDialog.cpp unsvFileDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o unsvFileDialog.o unsvFileDialog.cpp

file_watcher.o: file_watcher.cpp file_watcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o file_watcher.o file_watcher.cpp

recent_files.o: recent_files.cpp recent_files.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o recent_files.o recent_files.cpp

search.o: search.cpp search.h \
		view_manager.h \
		recent_files.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o search.o search.cpp

clipboard.o: src/core/clipboard.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o clipboard.o src/core/clipboard.cpp

view_manager.o: view_manager.cpp view_manager.h \
		recent_files.h \
		debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view_manager.o view_manager.cpp

view.o: src/core/view.cpp view_manager.h \
		recent_files.h \
		debug.h \
		status_line.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view.o src/core/view.cpp

status_line.o: status_line.cpp status_line.h \
		debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o status_line.o status_line.cpp

id.o: src/utils/id.cpp src/utils/id.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o id.o src/utils/id.cpp

file_type.o: src/core/file_type.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o file_type.o src/core/file_type.cpp

plugin_manager.o: src/core/plugin_manager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o plugin_manager.o src/core/plugin_manager.cpp

lex.yy.o: src/core/highlight/cpp/lex.yy.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o lex.yy.o src/core/highlight/cpp/lex.yy.cpp

highlight_manager.o: src/core/highlight/highlight_manager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o highlight_manager.o src/core/highlight/highlight_manager.cpp

block_allocator.o: src/core/vjson/block_allocator.cpp src/core/vjson/block_allocator.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o block_allocator.o src/core/vjson/block_allocator.cpp

json.o: src/core/vjson/json.cpp src/core/vjson/json.h \
		src/core/vjson/block_allocator.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o json.o src/core/vjson/json.cpp

settings.o: src/core/settings.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o settings.o src/core/settings.cpp

c_label.o: src/core/c_label.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o c_label.o src/core/c_label.cpp

text_manip.o: src/core/text_manip.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o text_manip.o src/core/text_manip.cpp

cli.o: cli.cpp cli.h \
		command.h \
		view_manager.h \
		recent_files.h \
		debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cli.o cli.cpp

command.o: command.cpp command.h \
		view_manager.h \
		recent_files.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o command.o command.cpp

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

moc_highlighter.o: moc_highlighter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_highlighter.o moc_highlighter.cpp

moc_c_label.o: moc_c_label.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_c_label.o moc_c_label.cpp

moc_text_manip.o: moc_text_manip.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_text_manip.o moc_text_manip.cpp

moc_cli.o: moc_cli.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_cli.o moc_cli.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

