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

appsdir = $(datadir)/applications
apps_DATA = file-shrimp.desktop

file_shrimp_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

EXTRA_DIST =					\
	autogen.sh				\
	intltool-extract.in			\
	intltool-merge.in			\
	intltool-update.in			\
	file-shrimp.desktop.in			\
	file-shrimp.glade			\
	file-shrimp.gladep			\
	file-shrimp.sh				\
	file-shrimp.spec

rpm: dist-gzip
	rpmbuild -tb $(distdir).tar.gz

srpm: dist-gzip
	rpmbuild -ts $(distdir).tar.gz

rpms: dist-gzip
	rpmbuild -ta $(distdir).tar.gz

@INTLTOOL_DESKTOP_RULE@
