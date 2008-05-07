## Process this file with automake to produce Makefile.in

SUBDIRS = po

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = file-shrimp

file_shrimp_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.h

file_shrimp_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

EXTRA_DIST = \
	autogen.sh \
	file-shrimp.glade \
	file-shrimp.gladep

