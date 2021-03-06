===============================================================================
=  readme.txt                           AppDep application files description  =
=  Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr  =
=  Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home  =
=  Copyright 2010-2016 Julien Darthenay                                       =
===============================================================================

===============================================================================
=  This file is part of AppDep                                                =
=                                                                             =
=  AppDep is free software: you can redistribute it and/or modify it          =
=  under the terms of the GNU General Public License as published by          =
=  the Free Software Foundation, either version 3 of the License, or (at      =
=  your option) any later version.                                            =
=                                                                             =
=  AppDep is distributed in the hope that it will be useful, but WITHOUT      =
=  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      =
=  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public               =
=  License for more details.                                                  =
=                                                                             =
=  You should have received a copy of the GNU General Public License          =
=  along with AppDep. If not, see <http://www.gnu.org/licenses/>.             =
===============================================================================

The archive file containing this file is available at
<http://julien.darthenay.free.fr> or
<https://github.com/JDarthenay/AppDep>.

AppDep - version 2.2.1

AppDep.exe is a tool you can use to append dependencies at the end of a file.
It searches the demarcation line (being a given one or the default one) in the
target file (supposed to be a makefile). If it is found, all following data are
erased. If it is not, it is appended at end of file. Next all data received on
standard input are appended (those are supposed to be dependencies, as gcc
could generate them for instance).

Description of the various files.

AppDep-src.7z - archive including all source file for the application.
Includes:
  src/resource.h
  src/xpmanifest.xml
  src/AppDep.rc
  src/AppDep.c
  src/gettext.c
  src/gettext.h
  src/version.h
  src/wgetopt.c
  src/wgetopt.h
  po/AppDep.pot
  po/fr.po
  po/fr.gmo
  res/AppDep.ico
  update-ver.bat
  makefile
  Doxyfile
  COPYING.txt
  lisezmoi.txt
  readme.txt
  makefile.txt
  README.md

AppDep-bin32.7z - archive including only the application itself as a 32-bits
executable.
Includes:
  bin_x86/AppDep.exe
  share/
  COPYING.txt
  lisezmoi.txt
  readme.txt
  makekile.txt

AppDep-bin64.7z - archive including only the application itself as a 64-bits
executable.
Includes:
  bin_x64/AppDep.exe
  share/
  COPYING.txt
  lisezmoi.txt
  readme.txt
  makekile.txt

AppDep-htmldoc.7z - archive including documentation of source code.
Contient :
  htmldoc/
  COPYING.txt
  lisezmoi.txt
  readme.txt
  makefile.txt

src/resource.h - resources header.

src/version.h - application versions header.

src/xpmanifest.xml - manifest file, sets file AppDep.exe display in Windows
file explorer.

src/AppDep.rc - resources file. Permits to incorporate manifest file and
sets porgram information in Version tab of file AppDep.exe Properties.

src/AppDep.c - main module.

src/gettext.h - libintl.dll dynamic loading module header.

src/gettext.c - libintl.dll dynamic loading module.

src/wgetopt.h - wide strings getopt module header.

src/wgetopt.c - wide strings getopt module.

res/AppDep.ico - application icons file.

po/AppDep.pot - translations portable object template.

po/fr.po - French translation portable object.

po/fr.gmo - French translation machine object.

htmldoc/ - this folder includes documentation generated by Doxygen.

bin_x86/AppDep.exe - the tool itself, built as a 32-bits binary with
MinGW-w64. <http://mingw-w64.org/doku.php>. Copy it within same folder as the
makefile which requires it, or within any folder in the list the environment
variable PATH contains.

bin_x64/AppDep.exe - the tool itself, built as a 64-bits binary with
MinGW-w64. <http://mingw-w64.org/doku.php>. Copy it within same folder as the
makefile which requires it, or within any folder in the list the environment
variable PATH contains.

share/ - this folder includes all translations machine objects, ready to be
deployed. It must be moved in the parent folder of the one where AppDep.exe is
installed.

update-ver.bat - this batch file updates version numbers in manifest, version
header and Doxygen settings files. It requires GNU sed
<http://gnuwin32.sourceforge.net/packages/sed.htm> and GNU grep
<http://gnuwin32.sourceforge.net/packages/grep.htm>. Update environment
variable PATH if necessary so that batch can find sed and grep.

makefile - use this makefile to build AppDep with MinGW-w64. Set up paths in
this makefile before using it.

Doxyfile - Doxygen settings file <http://www.doxygen.org/>.

COPYING.txt - GPL licence version 3. It is the project's licence. Some source
files can individually use MIT licence, please check beginning of each file.

lisezmoi.txt - French translation of this file.

readme.txt - this file.

README.md - instructions to build or translate the tool.

makefile.txt - an example: this tool's version 2.0 makefile using itself.

Versions history
May 30, 2010 - version 1.0
  Creation.
November 20, 2010 - version 1.1
  Help message fix.
  New option to retrieve version number.
May 15, 2012 - version 1.2
  Using outputDOS version 1.3: adding an cammnd line option as an alternative
to environment variable OUTPUT_DOS.
May 19, 2012 - version 1.3
  outputDOS library is now dynamically loaded, so that AppDep can now execute
even if outputDOS was not found.
July 21, 2012 - version 1.4
  Building both 32-bits and 64-bits version.
  majVer.bat do not update makefile.txt anymore. This file will be from now on
version 1.2 makefile.
  Update icons file to add a 256x256 resolution.
August 25, 2013 - version 1.5
  Adding -n option to add nothing instead of reading standard input.
April 15, 2016 - version 2.0
  Remove outputDOS library usage.
  Rearranging files in folders.
  Translating application in English: batch files names, documentation
generated by Doxygen, application messages, new file readme.txt.
  Internationalization with GNU gettext.
  Dynamically loading libintl.dll for using GNU gettext.
  Using wgetopt.
  Truncage of a file now done with a system call rather than with an overwrite.
This is an important robustness enhancement.
  32-bits and 64-bits building can now be done with a single GNU make usage.
  makefile.txt example is now version 2.0 makefile.
  Adding a main page in the documentation generated by Doxygen.
  New GitHub repository.
April 24, 2016 - version 2.1
  Running makefile now creates all required folders.
  Adding -i option to choose explicitly given libiconv.dll.
  Adding -r option to choose not to try to load any dll.
  Adding -q option, quiet mode.
April 30, 2016 - version 2.2
  Adding .gitignore file.
  Supplementary makefile improvements.
  Adding ability to choose where to get locales.
May 1, 2016 - version 2.2.1
  Minor makefile fix.
