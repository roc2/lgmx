#############################################################################
# Makefile for building: lgmx
# Generated by qmake (2.01a) (Qt 4.8.1) on: Wed Apr 24 00:38:30 2013
# Project:  lgmx.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile lgmx.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -Isrc/utils -Isrc/core -Isrc/core/highlight -Isrc/core/tags -I/usr/local/include -Isrc/core/vjson -Isrc/core/cli -Isrc/core/settings -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtGui -lQtCore -lpthread 
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
		src/core/main_window.cpp \
		src/core/code_editor.cpp \
		src/core/src_container.cpp \
		src/core/highlight/c/c_hl.cpp \
		src/core/highlight/cpp/cpp_hl.cpp \
		src/core/go_to_line.cpp \
		src/core/src_file.cpp \
		src/core/config.cpp \
		src/core/file_watcher.cpp \
		src/core/recent_files.cpp \
		src/core/search.cpp \
		src/core/clipboard.cpp \
		src/core/view_manager.cpp \
		src/core/view.cpp \
		src/core/status_line.cpp \
		src/utils/id.cpp \
		src/core/file_type.cpp \
		src/core/plugin_manager.cpp \
		src/core/highlight/highlight_manager.cpp \
		src/core/vjson/block_allocator.cpp \
		src/core/vjson/json.cpp \
		src/core/settings.cpp \
		src/core/c_label.cpp \
		src/core/text_manip.cpp \
		src/core/cli/cli.cpp \
		src/core/cli/command.cpp \
		src/core/highlight/syntax_highlighter.cpp \
		src/core/tags/tags.cpp \
		src/core/tags/readtags.cpp \
		src/core/settings/tab_settings.cpp moc_code_editor.cpp \
		moc_main_window.cpp \
		moc_src_container.cpp \
		moc_c_hl.cpp \
		moc_cpp_hl.cpp \
		moc_go_to_line.cpp \
		moc_src_file.cpp \
		moc_file_watcher.cpp \
		moc_recent_files.cpp \
		moc_search.cpp \
		moc_clipboard.cpp \
		moc_view_manager.cpp \
		moc_view.cpp \
		moc_status_line.cpp \
		moc_c_label.cpp \
		moc_text_manip.cpp \
		moc_cli.cpp \
		moc_syntax_highlighter.cpp \
		moc_tags.cpp
OBJECTS       = main.o \
		main_window.o \
		code_editor.o \
		src_container.o \
		c_hl.o \
		cpp_hl.o \
		go_to_line.o \
		src_file.o \
		config.o \
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
		highlight_manager.o \
		block_allocator.o \
		json.o \
		settings.o \
		c_label.o \
		text_manip.o \
		cli.o \
		command.o \
		syntax_highlighter.o \
		tags.o \
		readtags.o \
		tab_settings.o \
		moc_code_editor.o \
		moc_main_window.o \
		moc_src_container.o \
		moc_c_hl.o \
		moc_cpp_hl.o \
		moc_go_to_line.o \
		moc_src_file.o \
		moc_file_watcher.o \
		moc_recent_files.o \
		moc_search.o \
		moc_clipboard.o \
		moc_view_manager.o \
		moc_view.o \
		moc_status_line.o \
		moc_c_label.o \
		moc_text_manip.o \
		moc_cli.o \
		moc_syntax_highlighter.o \
		moc_tags.o
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
QMAKE_TARGET  = lgmx
DESTDIR       = 
TARGET        = lgmx

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
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile lgmx.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/lgmx1.0.0 || $(MKDIR) .tmp/lgmx1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/lgmx1.0.0/ && $(COPY_FILE) --parents src/core/debug.h src/core/code_editor.h src/core/main_window.h src/core/src_container.h src/core/highlight/c/c_hl.h src/core/highlight/cpp/cpp_hl.h src/core/go_to_line.h src/core/ui_go_to_line.h src/core/src_file.h src/core/config.h src/core/file_watcher.h src/core/recent_files.h src/core/search.h src/core/clipboard.h src/core/view_manager.h src/core/view.h src/core/status_line.h src/utils/id.h src/core/exception.h src/core/interfaces.h src/core/file_type.h src/core/plugin_manager.h src/core/highlight/highlight_manager.h src/core/vjson/block_allocator.h src/core/vjson/json.h src/core/settings.h src/core/c_label.h src/core/text_manip.h src/core/cli/cli.h src/core/cli/command.h src/core/highlight/syntax_highlighter.h src/core/tags/tags.h src/core/tags/readtags.h src/core/settings/tab_settings.h .tmp/lgmx1.0.0/ && $(COPY_FILE) --parents src/core/main.cpp src/core/main_window.cpp src/core/code_editor.cpp src/core/src_container.cpp src/core/highlight/c/c_hl.cpp src/core/highlight/cpp/cpp_hl.cpp src/core/go_to_line.cpp src/core/src_file.cpp src/core/config.cpp src/core/file_watcher.cpp src/core/recent_files.cpp src/core/search.cpp src/core/clipboard.cpp src/core/view_manager.cpp src/core/view.cpp src/core/status_line.cpp src/utils/id.cpp src/core/file_type.cpp src/core/plugin_manager.cpp src/core/highlight/highlight_manager.cpp src/core/vjson/block_allocator.cpp src/core/vjson/json.cpp src/core/settings.cpp src/core/c_label.cpp src/core/text_manip.cpp src/core/cli/cli.cpp src/core/cli/command.cpp src/core/highlight/syntax_highlighter.cpp src/core/tags/tags.cpp src/core/tags/readtags.cpp src/core/settings/tab_settings.cpp .tmp/lgmx1.0.0/ && (cd `dirname .tmp/lgmx1.0.0` && $(TAR) lgmx1.0.0.tar lgmx1.0.0 && $(COMPRESS) lgmx1.0.0.tar) && $(MOVE) `dirname .tmp/lgmx1.0.0`/lgmx1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/lgmx1.0.0


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

compiler_moc_header_make_all: moc_code_editor.cpp moc_main_window.cpp moc_src_container.cpp moc_c_hl.cpp moc_cpp_hl.cpp moc_go_to_line.cpp moc_src_file.cpp moc_file_watcher.cpp moc_recent_files.cpp moc_search.cpp moc_clipboard.cpp moc_view_manager.cpp moc_view.cpp moc_status_line.cpp moc_c_label.cpp moc_text_manip.cpp moc_cli.cpp moc_syntax_highlighter.cpp moc_tags.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_code_editor.cpp moc_main_window.cpp moc_src_container.cpp moc_c_hl.cpp moc_cpp_hl.cpp moc_go_to_line.cpp moc_src_file.cpp moc_file_watcher.cpp moc_recent_files.cpp moc_search.cpp moc_clipboard.cpp moc_view_manager.cpp moc_view.cpp moc_status_line.cpp moc_c_label.cpp moc_text_manip.cpp moc_cli.cpp moc_syntax_highlighter.cpp moc_tags.cpp
moc_code_editor.cpp: src/core/code_editor.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/code_editor.h -o moc_code_editor.cpp

moc_main_window.cpp: src/core/main_window.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/main_window.h -o moc_main_window.cpp

moc_src_container.cpp: src/core/src_container.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/src_container.h -o moc_src_container.cpp

moc_c_hl.cpp: src/core/highlight/c/c_hl.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/highlight/c/c_hl.h -o moc_c_hl.cpp

moc_cpp_hl.cpp: src/core/highlight/cpp/cpp_hl.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/highlight/cpp/cpp_hl.h -o moc_cpp_hl.cpp

moc_go_to_line.cpp: src/core/ui_go_to_line.h \
		src/core/src_container.h \
		src/core/go_to_line.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/go_to_line.h -o moc_go_to_line.cpp

moc_src_file.cpp: src/core/src_file.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/src_file.h -o moc_src_file.cpp

moc_file_watcher.cpp: src/core/file_watcher.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/file_watcher.h -o moc_file_watcher.cpp

moc_recent_files.cpp: src/core/recent_files.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/recent_files.h -o moc_recent_files.cpp

moc_search.cpp: src/core/src_container.h \
		src/core/search.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/search.h -o moc_search.cpp

moc_clipboard.cpp: src/core/clipboard.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/clipboard.h -o moc_clipboard.cpp

moc_view_manager.cpp: src/core/view_manager.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/view_manager.h -o moc_view_manager.cpp

moc_view.cpp: src/core/view.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/view.h -o moc_view.cpp

moc_status_line.cpp: src/core/status_line.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/status_line.h -o moc_status_line.cpp

moc_c_label.cpp: src/core/c_label.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/c_label.h -o moc_c_label.cpp

moc_text_manip.cpp: src/core/text_manip.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/text_manip.h -o moc_text_manip.cpp

moc_cli.cpp: src/core/cli/cli.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/cli/cli.h -o moc_cli.cpp

moc_syntax_highlighter.cpp: src/core/highlight/syntax_highlighter.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/highlight/syntax_highlighter.h -o moc_syntax_highlighter.cpp

moc_tags.cpp: src/core/tags/tags.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/core/tags/tags.h -o moc_tags.cpp

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

main.o: src/core/main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o src/core/main.cpp

main_window.o: src/core/main_window.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main_window.o src/core/main_window.cpp

code_editor.o: src/core/code_editor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o code_editor.o src/core/code_editor.cpp

src_container.o: src/core/src_container.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o src_container.o src/core/src_container.cpp

c_hl.o: src/core/highlight/c/c_hl.cpp src/core/highlight/c/c_hl.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o c_hl.o src/core/highlight/c/c_hl.cpp

cpp_hl.o: src/core/highlight/cpp/cpp_hl.cpp src/core/highlight/cpp/cpp_hl.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cpp_hl.o src/core/highlight/cpp/cpp_hl.cpp

go_to_line.o: src/core/go_to_line.cpp src/core/go_to_line.h \
		src/core/ui_go_to_line.h \
		src/core/src_container.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o go_to_line.o src/core/go_to_line.cpp

src_file.o: src/core/src_file.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o src_file.o src/core/src_file.cpp

config.o: src/core/config.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o config.o src/core/config.cpp

file_watcher.o: src/core/file_watcher.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o file_watcher.o src/core/file_watcher.cpp

recent_files.o: src/core/recent_files.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o recent_files.o src/core/recent_files.cpp

search.o: src/core/search.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o search.o src/core/search.cpp

clipboard.o: src/core/clipboard.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o clipboard.o src/core/clipboard.cpp

view_manager.o: src/core/view_manager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view_manager.o src/core/view_manager.cpp

view.o: src/core/view.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view.o src/core/view.cpp

status_line.o: src/core/status_line.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o status_line.o src/core/status_line.cpp

id.o: src/utils/id.cpp src/utils/id.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o id.o src/utils/id.cpp

file_type.o: src/core/file_type.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o file_type.o src/core/file_type.cpp

plugin_manager.o: src/core/plugin_manager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o plugin_manager.o src/core/plugin_manager.cpp

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

cli.o: src/core/cli/cli.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cli.o src/core/cli/cli.cpp

command.o: src/core/cli/command.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o command.o src/core/cli/command.cpp

syntax_highlighter.o: src/core/highlight/syntax_highlighter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o syntax_highlighter.o src/core/highlight/syntax_highlighter.cpp

tags.o: src/core/tags/tags.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tags.o src/core/tags/tags.cpp

readtags.o: src/core/tags/readtags.cpp src/core/tags/readtags.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o readtags.o src/core/tags/readtags.cpp

tab_settings.o: src/core/settings/tab_settings.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tab_settings.o src/core/settings/tab_settings.cpp

moc_code_editor.o: moc_code_editor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_code_editor.o moc_code_editor.cpp

moc_main_window.o: moc_main_window.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_main_window.o moc_main_window.cpp

moc_src_container.o: moc_src_container.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_src_container.o moc_src_container.cpp

moc_c_hl.o: moc_c_hl.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_c_hl.o moc_c_hl.cpp

moc_cpp_hl.o: moc_cpp_hl.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_cpp_hl.o moc_cpp_hl.cpp

moc_go_to_line.o: moc_go_to_line.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_go_to_line.o moc_go_to_line.cpp

moc_src_file.o: moc_src_file.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_src_file.o moc_src_file.cpp

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

moc_c_label.o: moc_c_label.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_c_label.o moc_c_label.cpp

moc_text_manip.o: moc_text_manip.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_text_manip.o moc_text_manip.cpp

moc_cli.o: moc_cli.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_cli.o moc_cli.cpp

moc_syntax_highlighter.o: moc_syntax_highlighter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_syntax_highlighter.o moc_syntax_highlighter.cpp

moc_tags.o: moc_tags.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_tags.o moc_tags.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

