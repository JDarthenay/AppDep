@echo OFF

REM ---------------------------------------------------------------------------
REM update-ver.bat                               update version numbers batch |
REM                                             script for AppDep application |
REM Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr |
REM Developing tools                         Notepad++, MS Windows Seven Home |
REM Copyright 2010-2016 Julien Darthenay                                      |
REM ---------------------------------------------------------------------------

REM ---------------------------------------------------------------------------
REM This file is part of AppDep                                               |
REM                                                                           |
REM AppDep is free software: you can redistribute it and/or modify it         |
REM under the terms of the GNU General Public License as published by         |
REM the Free Software Foundation, either version 3 of the License, or (at     |
REM your option) any later version.                                           |
REM                                                                           |
REM AppDep is distributed in the hope that it will be useful, but WITHOUT     |
REM ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     |
REM FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public              |
REM License for more details.                                                 |
REM                                                                           |
REM You should have received a copy of the GNU General Public License         |
REM along with AppDep. If not, see <http://www.gnu.org/licenses/>.            |
REM ---------------------------------------------------------------------------

echo major version number
echo %1 | grep "^"[0-9][0-9]*" "*$
if errorlevel 2 goto GREPERR
if errorlevel 1 goto PARAMERR

echo minor version number
echo %2 | grep "^"[0-9][0-9]*" "*$
if errorlevel 2 goto GREPERR
if errorlevel 1 goto PARAMERR

echo revision number
echo %3 | grep "^"[0-9][0-9]*" "*$
if errorlevel 2 goto GREPERR
if errorlevel 1 goto PARAMERR

echo build number
echo %4 | grep "^"[0-9][0-9]*" "*$
if errorlevel 2 goto GREPERR
if errorlevel 1 goto PARAMERR

echo update file version.h
sed -e s/"APPDEP_VER_MAJ .*$"/"APPDEP_VER_MAJ "%1/ -e s/"APPDEP_VER_MIN .*$"/"APPDEP_VER_MIN "%2/ -e s/"APPDEP_VER_REV .*$"/"APPDEP_VER_REV "%3/ -e s/"APPDEP_VER_BUILD .*$"/"APPDEP_VER_BUILD "%4/ -e s/"APPDEP_VERSION_LONG .*$"/"APPDEP_VERSION_LONG "\"%1.%2.%3.%4\"/ -e s/"APPDEP_VERSION_NUMBER .*$"/"APPDEP_VERSION_NUMBER "%1","%2","%3","%4/ src\version.h> version.h
if errorlevel 1 goto SEDERR
del src\version.h
move version.h src\version.h

echo update file xpmanifest.xml
sed -e s/"^  "version=\"[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\"/"  "version=\"%1\.%2\.%3\.%4\"/ src\xpmanifest.xml > xpmanifest.xml
if errorlevel 1 goto SEDERR
del src\xpmanifest.xml
move xpmanifest.xml src\xpmanifest.xml

echo update file Doxyfile
sed -e s/"^ "*PROJECT_NUMBER" "*"=".*$/PROJECT_NUMBER"         = "%1\.%2\.%3\.%4/ Doxyfile > Doxyfile.tmp
if errorlevel 1 goto SEDERR
del Doxyfile
move Doxyfile.tmp Doxyfile

:GREPERR
echo erreur while executing grep
goto END

:PARAMERR
echo incorrect argument
goto END

:SEDERR
echo erreur while executing sed

:END
