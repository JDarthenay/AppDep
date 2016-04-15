/*****************************************************************************
 * wgetopt.h          processing command line wide strings parameters header *
 * Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr *
 * Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home *
 * Copyright 2010 Julien Darthenay                                           *
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
 * \file wgetopt.h
 * \brief Processing command line wide strings parameters header.
 * \author <a href="mailto:keithmarshall@users.sourceforge.net">Keith Marshall</a>
 * \author <a href="mailto:julien.darthenay@free.fr">Julien Darthenay</a>
 * \version 1.0
 */

/*
 * Please note this file is not original work.
 * Original file is getopt.h from MinGW pakage set, was written by
 * Keith Marshall <keithmarshall@users.sourceforge.net>
 * it can be found here:
 * https://sourceforge.net/p/mingw/mingw-org-wsl/ci/legacy/tree/mingwrt/include/getopt.h
 * bluebaroncanada <https://github.com/bluebaroncanada> used it in
 * getoptW <https://github.com/bluebaroncanada/getoptW>
 * and wrote file getoptW.h which can be found here:
 * https://github.com/bluebaroncanada/getoptW/blob/master/getoptW.h
 *
 * Below is the copyright information from getoptW.h which is the same as
 * copyright information from getopt.h
 *
 * DISCLAIMER
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 *
 * The mingw-w64 runtime package and its code is distributed in the hope that it
 * will be useful but WITHOUT ANY WARRANTY.  ALL WARRANTIES, EXPRESSED OR
 * IMPLIED ARE HEREBY DISCLAIMED.  This includes but is not limited to
 * warranties of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This licence is governed by the laws of Ontario, Canada and any dispute
 * shall be finally resolved by the courts in London, Ontario, Canada.
 */

#ifndef WGETOPT_H
#define WGETOPT_H

/* All the headers include this file. */
#include <crtdefs.h>

/**
 * \brief Index of first non-option in argv.
 * \since 1.0
 */
extern int woptind;     /* index of first non-option in argv      */

/**
 * \brief Single option character, as parsed.
 * \since 1.0
 */
extern int woptopt;     /* single option character, as parsed     */

/**
 * \brief flag to enable built-in diagnostics.
 * User may set to zero, to suppress.
 * \since 1.0
 */
extern int wopterr;     /* flag to enable built-in diagnostics... */
/* (user may set to zero, to suppress)    */

/**
 * \brief Pointer to argument of current option.
 * \since 1.0
 */
extern wchar_t *woptarg;        /* pointer to argument of current option  */

/**
 * \brief Parses the command-line arguments the same way as getopt(),
 * except that argv is a wide strings array.
 * \param[in] nargc Number of arguments.
 * \param[in,out] nargv Wide strings arguments array.
 * \param[in] options Options wide string.
 * \return -1 if no more options,
 * L':' if an option miss its parameter and first character in options is L':'.
 * L'?' if parse error,
 * parsed option else.
 * \since 1.0
 */
extern int wgetopt(int nargc, wchar_t * const *nargv, const wchar_t *options);

/**
 * \brief Option specifier for wgetopt_long() and wgetopt_long_only().
 * \since 1.0
 */
struct woption      /* specification for a long form option... */
{
    /**
     * \brief Does it take an argument?
     * \since 1.0
     */
    const wchar_t *name;        /* option name, without leading hyphens */

    /**
     * \brief Option name, without leading hyphens.
     * \since 1.0
     */
    int         has_arg;        /* does it take an argument?            */

    /**
     * \brief Where to save its status, or NULL.
     * \since 1.0
     */
    int        *flag;           /* where to save its status, or NULL    */

    /**
     * \brief Its associated status value.
     * \since 1.0
     */
    int         val;            /* its associated status value          */
};

/**
 * \brief Permitted values for its `has_arg' field.
 * \since 1.0
 */
enum            /* permitted values for its `has_arg' field... */
{
    /**
     * \brief Option never takes an argument.
     * \since 1.0
     */
    wno_argument = 0,               /* option never takes an argument     */

            /**
             * \brief Option always requires an argument.
             * \since 1.0
             */
            wrequired_argument,     /* option always requires an argument */

            /**
             * \brief Option may take an argument.
             * \since 1.0
             */
            woptional_argument      /* option may take an argument        */
};

/**
 * \brief Parses the command-line arguments the same way as long_getopt(),
 * except that argv is a wide strings array.
 * \param[in] nargc Number of arguments.
 * \param[in,out] nargv Wide strings arguments array.
 * \param[in] long_options Long options array.
 * \param[in] options Options wide string.
 * \param[in,out] idx If not NULL, it points to a variable which is set to the
 * index of the long option relative to longopts.
 * \return -1 if no more options,
 * L':' if an option miss its parameter and first character in options is L':'.
 * L'?' if parse error,
 * val if long option with flag == NULL,
 * 0 if long option with flag != NULL
 * parsed short option else.
 * \since 1.0
 */
extern int wgetopt_long(int nargc, wchar_t * const *nargv, const wchar_t *options,
                       const struct woption *long_options, int *idx);

/**
 * \brief Parses the command-line arguments the same way as long_getopt_only(),
 * except that argv is a wide strings array.
 * \param[in] nargc Number of arguments.
 * \param[in,out] nargv Wide strings arguments array.
 * \param[in] long_options Long options array.
 * \param[in] options Options wide string.
 * \param[in,out] idx If not NULL, it points to a variable which is set to the
 * index of the long option relative to longopts.
 * \return -1 if no more options,
 * L':' if an option miss its parameter and first character in options is L':'.
 * L'?' if parse error,
 * val if long option with flag == NULL,
 * 0 if long option with flag != NULL
 * parsed short option else.
 * \since 1.0
 */
extern int wgetopt_long_only(int nargc, wchar_t * const *nargv, const wchar_t *options,
                            const struct woption *long_options, int *idx);
#endif /* !defined(WGETOPT_H) */
