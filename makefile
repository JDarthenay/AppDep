###############################################################################
#  makefile                                  makefile for AppDep application  #
#  Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr  #
#  Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home  #
#  Copyright 2010-2016 Julien Darthenay                                       #
###############################################################################

###############################################################################
#  This file is part of AppDep                                                #
#                                                                             #
#  AppDep is free software: you can redistribute it and/or modify it          #
#  under the terms of the GNU General Public License as published by          #
#  the Free Software Foundation, either version 3 of the License, or (at      #
#  your option) any later version.                                            #
#                                                                             #
#  AppDep is distributed in the hope that it will be useful, but WITHOUT      #
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      #
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public               #
#  License for more details.                                                  #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with AppDep. If not, see <http://www.gnu.org/licenses/>.             #
###############################################################################

# this makefile is done for building with standard Windows shell
SHELL=C:\Windows\System32\cmd.exe

# path to be used for various commands
# the ligher, the better
# use semi-colon between paths
# use backslash as a directory separator
PATH=C:\Windows\system32;C:\Windows
PATH_BUILD_X86=
PATH_BUILD_X64=
PATH_ZT=
PATH_DOCGEN=
PATH_GETTEXT=
concatpath=$(if $1,$(if $2,$1;$2,$1),$2)

# Application name
APP=AppDep

# directories
# please use slash as a directory separator and end paths with slash
INSTALLDIR_X86=
INSTALLDIR_X64=
INSTALLDIR_SHARE=
BINDIR_X86=bin_x86/
BINDIR_X64=bin_x64/
OBJDIR_X86=obj_x86/
OBJDIR_X64=obj_x64/
SRCDIR=src/
RESDIR=res/
PODIR=po/
SHAREDIR=

# c compiler
CC=gcc
WINVER=0x0400
CFLAGS=-std=gnu99 -Wextra -Wall -Werror
CFLAGS_X86=-m32 -march=athlon-xp
CFLAGS_X64=-m64 -march=x86-64
CPPFLAGS=-D_WIN32_WINNT=$(WINVER) -D__USE_MINGW_ANSI_STDIO
LDFLAGS=

# resources compiler
RC=windres
RCFLAGS=
RCFLAGS_X86=--target=pe-i386
RCFLAGS_X64=--target=pe-x86-64

# compression tool
ZT=7z
ZCOM=u
ZFLAGS=-y

# documentation generation tool
DOCGEN=doxygen
DOCFLAGS=

# GNU gettext tools
XGT=xgettext
XGTFLAGS=--keyword=_ -d AppDep
MINIT=msginit
MIFLAGS=
MMERGE=msgmerge
MMFLAGS=--force-po
MFMT=msgfmt
MFFLAGS=-c -v

# files
OBJS_X86=$(OBJDIR_X86)$(APP).o $(OBJDIR_X86)gettext.o $(OBJDIR_X86)wgetopt.o $(OBJDIR_X86)$(APP)_res.o
OBJS_X64=$(OBJDIR_X64)$(APP).o $(OBJDIR_X64)gettext.o $(OBJDIR_X64)wgetopt.o $(OBJDIR_X64)$(APP)_res.o
OBJS=$(OBJS_X86) $(OBJS_X64)
EXE_X86=$(BINDIR_X86)$(APP).exe
EXE_X64=$(BINDIR_X64)$(APP).exe
EXE=$(EXE_X86) $(EXE_X64)
ZIP=$(APP)-src.7z $(APP)-bin32.7z $(APP)-bin64.7z $(APP)-htmldoc.7z
RES=$(SRCDIR)$(APP).rc $(SRCDIR)resource.h $(SRCDIR)version.h $(SRCDIR)xpmanifest.xml $(RESDIR)$(APP).ico
TXT=COPYING.txt lisezmoi.txt readme.txt makefile.txt
POT=$(PODIR)$(APP).pot
POS=$(wildcard $(PODIR)*.po)
GMOS=$(POS:%.po=%.gmo)
MOS=$(GMOS:$(PODIR)%.gmo=$(SHARE_DIR)share/locale/%/LC_MESSAGES/$(APP).mo)

all:compile_x86 compile_x64

compile_x86:$(EXE_X86)

compile_x64:$(EXE_X64)

compile_translation:$(GMOS)

merge_translation:$(POS)

extract_translation:$(POT)

install:install_x86 install_x64

install_x86:compile_x86|$(INSTALLDIR_X86)
	for %%i in ($(subst /,\,$(EXE_X86))) do copy %%i $(subst /,\,$(INSTALLDIR_X86))

install_x64:compile_x64|$(INSTALLDIR_X64)
	for %%i in ($(subst /,\,$(EXE_X64))) do copy %%i $(subst /,\,$(INSTALLDIR_X64))

install_translation:share_translation|$(INSTALLDIR_SHARE)
	xcopy /E /I /Y $(subst /,\,$(SHAREDIR)share) $(subst /,\,$(INSTALLDIR_SHARE)share)

share_translation:$(MOS)

uninstall:uninstall_x86 uninstall_x64

uninstall_x86:
	for %%i in ($(subst /,\,$(EXE:$(BINDIR_X86)%.exe=$(INSTALLDIR_X86)%.exe))) do if exist %%i del %%i

uninstall_x64:
	for %%i in ($(subst /,\,$(EXE:$(BINDIR_X64)%.exe=$(INSTALLDIR_X64)%.exe))) do if exist %%i del %%i

uninstall_translation:
	for %%i in ($(subst /,\,$(MOS:$(SHAREDIR)%.mo=$(INSTALLDIR_SHARE)%.mo))) do if exist %%i del %%i

archive:$(ZIP)

rebuild:clean all

rebuild_x86:clean_x86 compile_x86

rebuild_x64:clean_x64 compile_x64

clean:clean_x86 clean_x64

clean_x86:
	for %%i in ($(subst /,\,$(OBJS_X86))) do if exist %%i del %%i

clean_x64:
	for %%i in ($(subst /,\,$(OBJS_X64))) do if exist %%i del %%i

zclean:
	for %%i in ($(subst /,\,$(ZIP))) do if exist %%i del %%i

clean_translation:
	for %%i in ($(subst /,\,$(SHAREDIR)share)) do if exist %%i rmdir /S /Q %%i
	for %%i in ($(subst /,\,$(PODIR)*.po~)) do if exist %%i del %%i

mrproper:clean clean_translation
	for %%i in ($(subst /,\,$(EXE_X86) $(EXE_X64))) do if exist %%i del %%i
	if exist htmldoc rmdir /S /Q htmldoc

$(ZIP):PATH:=$(call concatpath,$(PATH),$(PATH_ZT))
$(APP)-src.7z:$(TXT) $(RES) $(SRCDIR)$(APP).c $(SRCDIR)gettext.h $(SRCDIR)gettext.c $(SRCDIR)wgetopt.h $(SRCDIR)wgetopt.c $(POT) $(POS) $(GMOS) update-ver.bat makefile Doxyfile README.md
	$(ZT) $(ZCOM) $@ $(ZFLAGS) $^

$(APP)-bin32.7z:$(TXT) $(EXE_X86) $(MOS)
	$(ZT) $(ZCOM) $@ $(ZFLAGS) $^

$(APP)-bin64.7z:$(TXT) $(EXE_X64) $(MOS)
	$(ZT) $(ZCOM) $@ $(ZFLAGS) $^

$(APP)-htmldoc.7z:$(TXT) htmldoc
	$(ZT) $(ZCOM) $@ $(ZLAGS) $^

$(EXE_X86) $(OBJS_X86):PATH:=$(call concatpath,$(PATH),$(PATH_BUILD_X86))
$(EXE_X86):$(OBJS_X86)|$(BINDIR_X86)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X86) -o $@ $(OBJS_X86) $(LDFLAGS)

$(EXE_X64) $(OBJS_X64):PATH:=$(call concatpath,$(PATH),$(PATH_BUILD_X64))
$(EXE_X64):$(OBJS_X64)|$(BINDIR_X64)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X64) -o $@ $(OBJS_X64) $(LDFLAGS)

$(OBJDIR_X86)$(APP).o:$(SRCDIR)$(APP).c $(SRCDIR)gettext.h $(SRCDIR)wgetopt.h $(SRCDIR)version.h|$(OBJDIR_X86)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X86) -o $@ -c $<

$(OBJDIR_X64)$(APP).o:$(SRCDIR)$(APP).c $(SRCDIR)gettext.h $(SRCDIR)wgetopt.h $(SRCDIR)version.h|$(OBJDIR_X64)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X64) -o $@ -c $<

$(OBJDIR_X86)gettext.o:$(SRCDIR)gettext.c $(SRCDIR)gettext.h $(SRCDIR)version.h|$(OBJDIR_X86)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X86) -o $@ -c $<

$(OBJDIR_X64)gettext.o:$(SRCDIR)gettext.c $(SRCDIR)gettext.h $(SRCDIR)version.h|$(OBJDIR_X64)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X64) -o $@ -c $<

$(OBJDIR_X86)wgetopt.o:$(SRCDIR)wgetopt.c $(SRCDIR)wgetopt.h|$(OBJDIR_X86)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X86) -o $@ -c $<

$(OBJDIR_X64)wgetopt.o:$(SRCDIR)wgetopt.c $(SRCDIR)wgetopt.h|$(OBJDIR_X64)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_X64) -o $@ -c $<

$(OBJDIR_X86)$(APP)_res.o:$(RES)|$(OBJDIR_X86)
	$(RC) $(RFCLAGS) $(RCFLAGS_X86) -o $@ $<

$(OBJDIR_X64)$(APP)_res.o:$(RES)|$(OBJDIR_X64)
	$(RC) $(RFCLAGS) $(RCFLAGS_X64) -o $@ $<

$(POT) $(PODIR)%.po $(GMOS) $(MOS):PATH:=$(call concatpath,$(PATH),$(PATH_GETTEXT))
$(POT):$(SRCDIR)$(APP).c $(SRCDIR)gettext.c $(SRCDIR)wgetopt.c|$(PODIR)
	$(XGT) $(XGTFLAGS) -o $@ $^

$(PODIR)%.po:$(POT)
	if not exist $(subst /,\,$@) ($(MINIT) $(MIFLAGS) -l $(@:$(PODIR)%.po=%) -o $@ -i $<) else $(MMERGE) $(MMFLAGS) -U $@ $<

$(PODIR)%.gmo:$(PODIR)%.po
	$(MFMT) $(MFFLAGS) -o $@ $<

$(SHARE_DIR)share/locale/%/LC_MESSAGES/$(APP).mo:$(PODIR)%.gmo
	@if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	copy $(subst /,\,$<) $(subst /,\,$@)

$(INSTALLDIR_X86) $(INSTALLDIR_X64) $(INSTALLDIR_SHARE) $(BINDIR_X86) $(BINDIR_X64) $(OBJDIR_X86) $(OBJDIR_X64) $(PODIR):
	mkdir $(subst /,\,$@)

htmldoc:PATH:=$(call concatpath,$(PATH),$(PATH_DOCGEN))
htmldoc:
	$(DOCGEN) $(DOCFLAGS)

.PHONY:install install_x86 install_x64 install_translation share_translation uninstall uninstall_x86 uninstall_x64 uninstall_translation archive rebuild rebuild_x86 rebuild_x64 all compile_x86 compile_x64 compile_translation merge_translation extract_translation clean clean_x86 clean_x64 zclean clean_translation mrproper htmldoc
