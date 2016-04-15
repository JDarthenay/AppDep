# AppDep

Use this tool to append automatically generated dependencies at the end of your
makefile.

## Licence

This file is part of AppDep

AppDep is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

AppDep is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with AppDep. If not, see (http://www.gnu.org/licenses/).

## How to build

### Requirements

* [MinGW-64w](http://mingw-w64.org/doku.php) is required for building. You need
the C compiler GCC and GNU Make.
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) can be used to generate
source code documentation (not mandatory).
* [7-Zip](http://www.7-zip.org/) can be used to generate archives (not
mandatory).
* [GNU gettext](http://www.gnu.org/software/gettext/gettext.html) is required
if you need to update or add translations (not mandatory if you run AppDep
without i18n). A recent build for Windows can be found on github in
[mlocati's repository](https://github.com/mlocati/gettext-iconv-windows).

### Update the makefile

* set the PATH_xx variables so that GNU Make will find the commands it tries to
execute.
* PATH_BUILD_X86 and PATH_BUILD_X64 should be set to GCC bin directories path.
PATH_BUILD_X86 is for using a toolchain able to build 32-bits binaries.
PATH_BUILD_X64 is for 64-bits.
* PATH_ZT should be set to 7-Zip path.
* PATH_DOCGEN should be set to Doxygen path.
* PATH_GETTEXT should be set to GNU gettext path.
* set the INSTALLDIR_xx variables so that GNU make intalls files in the correct
directories.
* PATH_xx variables require using backslash as a directory separator.
* INSTALLDIR_xx variables require using slash as a directory separator and
require ending slashes.
* INSTALLDIR_SHARE should be the parent directory of INSTALLDIR_X86 and
INSTALLDIR_X64.

### Create required directories
* for 32-bits building you need to create obj_x86 and bin_x86.
* for 64-bits building you need to create obj_x64 and bin_x64.

### Build and install binaries

* Build and install 32-bits binary: `gmake install_x86`
* Build and install 64-bits binary: `gmake install_x64`
* Build and install both binaries: `gmake install`
* Cleanup object and binary files: `gmake mrproper`
* Build and install translation files: `gmake install_translation`
* Cleanup translation files: `gmake clean_translation`

### Update existing translations

If you updated any _(text) in the sources files, translations have to be
updated.
* Update .po files with new messages extract: `gmake merge_translation`
* Edit .po files with a text editor and update translations.
* Build and install translation files: `gmake install_translation`
* Cleanup translation files: `gmake clean_translation`

### Add a translation

* Generate new .po file, for instance if you wish to add French Canadian
translation: `gmake po/fr_CA.po`
* Edit newly created file with text editor, update header and add translations.
* Build and install translation files: `gmake install_translation`
* Cleanup translation files: `gmake clean_translation`

## How to use

Please refer to example makefile.txt.
