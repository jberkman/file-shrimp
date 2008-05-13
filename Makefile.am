## Process this file with automake to produce Makefile.in

SUBDIRS = po

pkglibexecdir=$(libexecdir)/file-shrimp

INCLUDES = \
	-DPACKAGE_LIBEXEC_DIR=\""$(pkglibexecdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = file-shrimp

pkglibexec_SCRIPTS = file-shrimp.sh

file_shrimp_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.h

file_shrimp_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

EXTRA_DIST = \
	autogen.sh \
	file-shrimp.glade \
	file-shrimp.gladep \
	file-shrimp.sh


