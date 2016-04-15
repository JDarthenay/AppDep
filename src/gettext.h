/*****************************************************************************
 * gettext.h     dynamic link with GNU gettext header for AppDep application *
 * Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr *
 * Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home *
 * Copyright 2010-2016 Julien Darthenay                                      *
 *****************************************************************************/

/*****************************************************************************
 * The MIT License (MIT)                                                     *
 *                                                                           *
 * Copyright (c) 2016 Julien Darthenay <julien.darthenay@free.fr>            *
 *                                                                           *
 * Permission is hereby granted, free of charge, to any person obtaining a   *
 * copy of this software and associated documentation files (the             *
 * "Software"), to  deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,       *
 * distribute, sublicense, and/or sell copies of the Software, and to permit *
 * persons to whom the Software is furnished to do                           *
 * so, subject to the following conditions:                                  *
 *                                                                           *
 * The above copyright notice and this permission notice shall be included   *
 * in all copies or substantial portions of the Software.                    *
 *                                                                           *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN *
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR     *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                    *
 *****************************************************************************/

/**
 * \file gettext.h
 * \brief Dynamic link with GNU gettext header.
 * \author <a href="mailto:julien.darthenay@free.fr">Julien Darthenay</a>
 * \version 1.0
 */

#ifndef GETTEXT_H
#define GETTEXT_H

#include <stdio.h>
#include <wchar.h>

/**
 * \brief Wrapper to GNU gettext().
 * \param[in] msgid Message id of the message to be translated.
 * \return Message translation.
 * \since 1.0
 */
extern const char *gettext(const char *msgid);

/**
 * \brief Dynamic linking with GNU gettext intializations.
 * Searches libint with version number used during developement first.
 * If not found launches the Windows command where to find any DLL
 * with name matching libintl*.dll.
 * If still not found, uses default English messages.
 * \param[in] dllname Default dll name, may be only a file name or a full
 * pathname. May be NULL if no default dll name.
 * \since 1.0
 * \code
 * #include "gettext.h"
 * int main(int agrc, char *argv[])
 * {
 *   init_gettext(NULL); // Initialization
 *
 *   printf(_("Hello World!\n"));
 *
 *   close_gettext();
 *
 *   return 0;
 * }
 * \endcode
 */
extern void init_gettext(const wchar_t *dllname);

/**
 * \brief Freeing memory used by dynamic linking with GNU gettext.
 * \since 1.0
 */
extern void close_gettext();

/**
 * \brief Environment variable used to specify libint.dll to be used
 * \since 1.0
 */
#define VAR_LIBINT L"LIBINT_DLL"

/**
 * \brief Pattern to be used to seach libint.dll.
 * \since 1.0
 */
#define PATTERN_LIBINT L"libintl*.dll"

#ifndef GETTEXT_C

/**
 * \brief Convenient notation for using gettext().
 * \since 1.0
 */
#define _(text) (gettext(text))

#endif

#endif /* GETTEXT_H */
