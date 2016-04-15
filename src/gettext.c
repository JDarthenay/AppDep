/*****************************************************************************
 * gettext.c            dynamic link with GNU gettext for AppDep application *
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
 * \file gettext.c
 * \brief Dynamic link with GNU gettext.
 * \author <a href="mailto:julien.darthenay@free.fr">Julien Darthenay</a>
 * \version 1.0
 */

#define GETTEXT_C

#include "gettext.h"
#include "version.h"

#include <stdlib.h>
#include <errno.h>
#include <locale.h>

#include <windows.h>

/**
 * \brief Size of buffer for reading Windows command where results.
 * \since 1.0
 */
#define WHERE_BUFFER_SIZE 1024

/**
 * \brief Default gettext() function.
 * \param[in] msgid message id.
 * \return Default English message, which is identical to message id.
 * \since 1.0
 */
static const char *default_gettext(const char *msgid)
{
  return msgid;
}

/**
 * \brief handle to gettext().
 * \since 1.0
 */
static const char *(*p_gettext)(const char *) = &default_gettext;

const char *gettext(const char *text)
{
  return (*p_gettext)(text);
}

/**
 * \brief Handle to loaded library.
 * \since 1.0
 */
static HMODULE h_libint = NULL;

// Dynamic linking with GNU gettext intializations.
void init_gettext(const wchar_t *dllname)
{
  const char *(*p_bindtextdomain)(const char *, const char *);
  const char *(*p_textdomain)(const char *);

  // Tries to load a module with given name.
  // Also checks handlers to all necessary functions.
  int load_libint(const wchar_t *module_name)
  {
    if ((h_libint = LoadLibraryW(module_name)) != NULL)
    {
      p_gettext = (const char *(*)(const char *))
                  GetProcAddress(h_libint, "gettext");

      p_bindtextdomain = (const char *(*)(const char *, const char *))
                         GetProcAddress(h_libint, "bindtextdomain");

      p_textdomain = (const char *(*)(const char *))
                     GetProcAddress(h_libint, "textdomain");

      if (   p_gettext != NULL && p_bindtextdomain != NULL
          && p_textdomain != NULL)
      {
        (*p_bindtextdomain)(APPDEP_APPLICATION, "/usr/share/locale");
        (*p_textdomain)(APPDEP_APPLICATION);
      }
      else
      {
        FreeLibrary(h_libint);
        h_libint = NULL;
        p_gettext = &default_gettext;
      }
    }

    return (h_libint != NULL);
  }

  // Tries to load a module as specified by VAR_LIBINT environement variable.
  int load_var_libint()
  {
    const wchar_t *env_var_libint = _wgetenv(VAR_LIBINT);

    return (env_var_libint == NULL) ? 0 : load_libint(env_var_libint);
  }

  // Even if no library is found locale is set.
  // It changes character set for output in DOS console.
  setlocale(LC_ALL, "");

  if (dllname == NULL || !load_libint(dllname))
  {
    // No module found with default names: trying to call where
    FILE *f_where = _wpopen(L"where " PATTERN_LIBINT, L"r");
    if (f_where != NULL)
    {
      wchar_t buffer[WHERE_BUFFER_SIZE];
      wint_t c;
      int cont = 1;
      int buffer_overflow;

      while (cont)
      {
        // Tries to load each line Windows command where returns.
        buffer_overflow = 0;
        if (fgetws(buffer, WHERE_BUFFER_SIZE, f_where) == NULL)
        {
          cont = 0;
        }
        else
        {
          size_t len = wcslen(buffer);
          if (buffer[len - 1] == L'\n')
          {
            // Removes trailing newline
            buffer[len - 1] = L'\0';
          }
          else
          {
            // Removes all characters until a newline or end of file.
            do {
              if ((c = fgetwc(f_where)) != WEOF && c != L'\n')
              {
                buffer_overflow = 1;
              }
            } while (c != WEOF && c != L'\n');

            if (c == WEOF)
            {
              cont = 0;
            }
          }

          if (!buffer_overflow)
          {
            // If buffer is a full line,
            // tries to load the library with this full path.
            if (load_libint(buffer))
            {
              cont = 0;
            }
          }
        }
      }

      _pclose(f_where);
    }
  }
}

// Freeing memory used by dynamic linking with GNU gettext.
void close_gettext()
{
  if (h_libint != NULL)
  {
    FreeLibrary(h_libint);
    h_libint = NULL;
    p_gettext = &default_gettext;
  }
}
