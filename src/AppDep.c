/*****************************************************************************
 * AppDep.c                               main module for AppDep application *
 * Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr *
 * Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home *
 * Copyright 2010-2016 Julien Darthenay                                      *
 *****************************************************************************/

/*****************************************************************************
 * This file is part of AppDep                                               *
 *                                                                           *
 * AppDep is free software: you can redistribute it and/or modify it         *
 * under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or (at     *
 * your option) any later version.                                           *
 *                                                                           *
 * AppDep is distributed in the hope that it will be useful, but WITHOUT     *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public              *
 * License for more details.                                                 *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with AppDep. If not, see <http://www.gnu.org/licenses/>.            *
 *****************************************************************************/

/**
 * \file AppDep.c
 * \brief Main module for AppDep application
 * \author <a href="mailto:julien.darthenay@free.fr">Julien Darthenay</a>
 * \since 1.0
 */

/**
 * \mainpage AppDep application
 * \section intro Introduction
 * The main goal of AppDep is to append automatically generated dependencies.
 * \section install Installation
 * \subsection step1 Step 1: Install
 * Copy the AppDep.exe in any directory in your current path.
 * \subsection step2 Step 2: Install translations
 * Install libintl.dll. It should require iconv.dll.
 * Copy the share directory in the parent directory of the one where
 * you installed AppDep.exe.
 * \section usage Basic usage
 * Provided your command to generate dependencies is GenDep, use:
 * \verbatim
GenDep | AppDep makefile\endverbatim
 * If you need several commands to generate dependencies, you can do it this
 * way:
 * \verbatim
AppDep -n makefile
GenDep-1>> makefile
GenDep-2>> makefile
GenDep-3>> makefile\endverbatim
 * You can use gcc to generate dependencies list, for instance for AppDep 2.0
 * source files it will be:
 * \verbatim
AppDep -n makefile
gcc -MM src/AppDep.c -MT obj_x86/AppDep.o>> makefile
gcc -MM src/AppDep.c -MT obj_x64/AppDep.o>> makefile
gcc -MM src/gettext.c -MT obj_x86/gettext.o>> makefile
gcc -MM src/gettext.c -MT obj_x64/gettext.o>> makefile
gcc -MM src/wgetopt.c -MT obj_x86/wgetopt.o>> makefile
gcc -MM src/wgetopt.c -MT obj_x64/wgetopt.o>> makefile\endverbatim
 */

// Version include

#include "version.h"

// Standard includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>

// Windows includes

#include <windows.h>

// AppDep modules includes

#include "gettext.h"
#include "wgetopt.h"

// Return codes

/**
 * \brief Program terminates with no error.
 * \since 1.0
 */
#define RETURN_OK 0

/**
 * \brief Incorrects arguments.
 * \since 1.0
 */
#define RETURN_ARG_ERR 1

/**
 * \brief Opening file error.
 * \since 1.0
 */
#define RETURN_OPEN_ERR 2

/**
 * \brief Read file error.
 * \since 1.0
 */
#define RETURN_READ_ERR 3

/**
 * \brief Write file error.
 * \since 1.0
 */
#define RETURN_WRITE_ERR 4

/**
 * \brief Read stdin error.
 * \since 2.0
 */
#define RETURN_STDIN_ERR 5

// Other constants

/**
 * \brief Default line before lines to be appended.
 * \since 1.0
 */
#define DEFAULT_DEM_LINE L"# Dependencies start here."

/**
 * \brief Standard input buffer size.
 * \since 1.0
 */
#define STDIN_BUFFER_SIZE 1024

// Main functions

/**
 * \brief Updates target file.
 * \since 1.0
 * \param[in] target File to be updated
 * \param[in] dem_line Demacation line before appended text
 * \param[in] nflag Empty text flag
 * \param[in] qflag Quiet mode flag
 * \return Defined return code.
 */
static int update_file(const wchar_t *target,
                       const wchar_t *dem_line,
                       int nflag, int qflag)
{
  FILE *f = NULL;
  int b_line_feed = 1;
  int b_dem_reached = 0;
  int b_eof_reached = 0;
  size_t lg_dem_line = wcslen(dem_line);
  wchar_t buffer_target[2 + lg_dem_line];
  size_t lg_buffer_target = 0;
  char buffer_stdin[STDIN_BUFFER_SIZE];
  size_t lg_buffer_stdin;
  size_t lg_total = 0;
  off_t pos_end_of_demarcation, pos_end_of_append, pos_end_of_file;

  // Opening target file
  if ((f = _wfopen(target, L"r+")) == NULL)
  {
    if (!qflag)
    {
      perror("_wfopen");
    }
    return RETURN_OPEN_ERR;
  }

  // Searching demacation line
  while (!b_dem_reached)
  {
    if (fgetws(buffer_target, 2 + lg_dem_line, f) == NULL)
    {
      // Nothing more to be read: adding demarcation line
      if (!feof(f))
      {
        if (!qflag)
        {
          perror("fgetws");
        }
        fclose(f);
        return RETURN_READ_ERR;
      }
      b_eof_reached = 1;

      if (fseek(f, 0, SEEK_CUR) != 0)
      {
        if (!qflag)
        {
          perror("fseek");
        }
        fclose(f);
        return RETURN_WRITE_ERR;
      }

      if (!b_line_feed && fputwc(L'\n', f) == WEOF)
      {
        if (!qflag)
        {
          perror("fputwc");
        }
        fclose(f);
        return RETURN_WRITE_ERR;
      }

      if (fputws(dem_line, f) == WEOF)
      {
        if (!qflag)
        {
          perror("fputws");
        }
        fclose(f);
        return RETURN_WRITE_ERR;
      }

      if (fputwc(L'\n', f) == WEOF)
      {
        if (!qflag)
        {
          perror("fputwc");
        }
        fclose(f);
        return RETURN_WRITE_ERR;
      }

      b_dem_reached =  1;
    }
    else
    {
      // More lines to be read in target file
      lg_buffer_target = wcslen(buffer_target);
      lg_total += lg_buffer_target;

      if (b_line_feed && wcscmp(buffer_target, dem_line) == 0)
      {
        // Found demarcation line but not end of line
        if (fseek(f, 0, SEEK_CUR) != 0)
        {
          if (!qflag)
          {
            perror("fseek");
          }
          fclose(f);
          return RETURN_WRITE_ERR;
        }

        if (fputwc('\n', f) == WEOF)
        {
          if (!qflag)
          {
            perror("fputwc");
          }
          fclose(f);
          return RETURN_WRITE_ERR;
        }

        b_dem_reached = 1;
      }
      else if (   b_line_feed && lg_dem_line + 1 == lg_buffer_target
               && wcsncmp(buffer_target, dem_line, lg_dem_line) == 0
               && buffer_target[lg_buffer_target - 1] == L'\n')
      {
        // Found demarcation line with end of line
        if (fseek(f, 0, SEEK_CUR) != 0)
        {
          if (!qflag)
          {
            perror("fseek");
          }
          fclose(f);
          return RETURN_WRITE_ERR;
        }

        b_dem_reached = 1;
      }
      else
      {
        b_line_feed = buffer_target[lg_buffer_target - 1] == '\n';
      }
    }
  }

  // Appending stdin at the end of target file
  if (!nflag)
  {
    // Opening target file again to get rid of stream orientation
    if ((pos_end_of_demarcation = ftello(f)) < 0)
    {
      if (!qflag)
      {
        perror("ftello");
      }
      fclose(f);
      return RETURN_WRITE_ERR;
    }

    fclose(f);
    if ((f = _wfopen(target, L"r+")) == NULL)
    {
      if (!qflag)
      {
        perror("_wfopen");
      }
      return RETURN_OPEN_ERR;
    }

    if (fseeko(f, pos_end_of_demarcation, SEEK_SET) < 0)
    {
      if (!qflag)
      {
        perror("fseek");
      }
      fclose(f);
      return RETURN_WRITE_ERR;
    }

    while (!feof(stdin))
    {
      if (  (lg_buffer_stdin
             = fread(buffer_stdin, sizeof(char), STDIN_BUFFER_SIZE, stdin))
          < STDIN_BUFFER_SIZE
       && !feof(stdin))
      {
        if (!qflag)
        {
          perror("fread");
        }
        fclose(f);
        return RETURN_STDIN_ERR;
      }

      if (lg_buffer_stdin > 0)
      {
        lg_total += lg_buffer_stdin;

        if (  fwrite(buffer_stdin, sizeof(char), lg_buffer_stdin, f)
            < lg_buffer_stdin)
        {
          if (!qflag)
          {
            perror("fwrite");
          }
          fclose(f);
          return RETURN_WRITE_ERR;
        }
      }
    }
  }

  if (!b_eof_reached)
  {
    // Check file is not to be truncated
    if ((pos_end_of_append = ftello(f)) < 0)
    {
      if (!qflag)
      {
        perror("ftello");
      }
      fclose(f);
      return RETURN_READ_ERR;
    }

    if (fseeko(f, 0, SEEK_END) < 0)
    {
      if (!qflag)
      {
        perror("fseekp");
      }
      fclose(f);
      return RETURN_READ_ERR;
    }

    if ((pos_end_of_file = ftello(f)) < 0)
    {
      if (!qflag)
      {
        perror("ftellp");
      }
      fclose(f);
      return RETURN_READ_ERR;
    }

    if (pos_end_of_append < pos_end_of_file)
    {
      // File is to be truncated
      ftruncate(_fileno(f), pos_end_of_append);
    }
  }

  fclose(f);

  return RETURN_OK;
}

static struct woption long_options[] =
{
  {L"help", wno_argument, NULL, L'h'},
  {L"version", wno_argument, NULL, L'V'},
  {L"dem-line", wrequired_argument, NULL, L'l'},
  {L"null", wno_argument, NULL, L'n'},
  {L"quiet", wno_argument, NULL, L'q'},
  {L"libintl-dll", wrequired_argument, NULL, L'd'},
  {L"libiconv-dll", wrequired_argument, NULL, L'i'},
  {L"raw-output", wrequired_argument, NULL, L'r'},
  {L"no-dll", wno_argument, NULL, L'r'},
  {NULL, 0, NULL, 0}
};

static wchar_t *short_options = L"hVl:nqd:i:r";

/**
 * \brief Main using wide string arguments.
 * \since 2.0
 * \param[in] argc Number of arguments
 * \param[in] argv Arguments array
 * \return Defined return code.
 */
static int wmain(int argc, wchar_t *argv[])
{
  int c;
  int errflag = 0;
  int help_flag = 0;
  int version_flag = 0;
  int null_flag = 0;
  int dem_line_flag = 0;
  int quiet_flag = 0;
  const wchar_t *dem_line = DEFAULT_DEM_LINE;
  wchar_t *libintl_dll = NULL;
  wchar_t *libiconv_dll = NULL;
  int raw_flag = 0;
  int result = 0;

  // Command line analysis
  wopterr = 0;
  while (!errflag
       && (c = wgetopt_long(argc, argv, short_options, long_options, NULL))
          != -1)
  {
    switch (c)
    {
    case L'?':
      errflag = 1;
      break;

    case L'h':
      if (   help_flag || version_flag 
          || dem_line_flag || null_flag || quiet_flag)
      {
        errflag = 1;
      }
      else
      {
        help_flag = 1;
      }
      break;

    case L'V':
      if (   help_flag || version_flag
          || dem_line_flag || null_flag || quiet_flag)
      {
        errflag = 1;
      }
      else
      {
        version_flag = 1;
      }
      break;

    case L'l':
      if (help_flag || version_flag || dem_line_flag)
      {
        errflag = 1;
      }
      else
      {
        dem_line_flag = 1;
        dem_line = woptarg;
      }
      break;

    case L'n':
      if (help_flag || version_flag || null_flag)
      {
        errflag = 1;
      }
      else
      {
        null_flag = 1;
      }
      break;

    case L'q':
      if (   help_flag || version_flag || quiet_flag
          || libintl_dll != NULL || libiconv_dll != NULL)
      {
        errflag = 1;
      }
      else
      {
        quiet_flag = 1;
      }
      break;

    case L'd':
      if (quiet_flag || raw_flag || libintl_dll != NULL)
      {
        errflag = 1;
      }
      else
      {
        libintl_dll = woptarg;
      }
      break;

    case L'i':
      if (quiet_flag || raw_flag || libiconv_dll != NULL)
      {
        errflag = 1;
      }
      else
      {
        libiconv_dll = woptarg;
      }
      break;

    case L'r':
      if (   libintl_dll != NULL || libiconv_dll != NULL
          || raw_flag)
      {
        errflag = 1;
      }
      else
      {
        raw_flag = 1;
      }
      break;
    }
  }

  if (!errflag
   && (((help_flag || version_flag) && woptind != argc)
    || (!(help_flag || version_flag) && woptind != argc - 1)))
        errflag = 1;

  if (!raw_flag && !quiet_flag)
  {
    init_gettext(libiconv_dll, libintl_dll);
  }

  if (errflag)
  {
    // Incorrect arguments
    fprintf(stderr,
            _("Incorrects arguments, see: %s --help\n"),
            APPDEP_APPLICATION);
    result = RETURN_ARG_ERR;
  }
  else if (help_flag)
  {
    // Help message
    printf(_("\
Append standard input at the end of a file.\n\
Designed to add dependencies at the end of a makefile.\n\
\n\
Usage:\n\
%s [appending options] [gettext options] TARGET\n\
%s -h [gettext options]\n\
%s -v [gettext options]\n\
\n\
  -V,  --version                 display the version of %s and exit.\n\
  -h,  --help                    print this help.\n\
\n\
Appending:\n\
  -n,  --null                append nothing instead of standard input.\n\
  -l,  --dem-line LINE_TEXT  LINE_TEXT will be used as a demarcation line.\n\
  -q,  --quiet               do not print any error message (implies -r).\n\
\n\
gettext:\n\
  -i,  --libiconv-dll LIBICONV_DLL  try to use LIBICONV_DLL\n\
                                    as a libiconv.dll dynamic library.\n\
  -d,  --libintl-dll LIBINT_DLL     try to use LIBINT_DLL\n\
                                    as a libintl.dll dynamic library.\n\
  -r,  --raw-output,  --no-dll      do not try to load any DLL.\n\
\n\
Default demarcation line is:\n\
%ls\n\
If no LIBICONV_DLL is given or loading LIBICONV_DLL failed,\n\
environment variable %%%ls%% will be used. If this also fails,\n\
libintl.dll will have to load libinconv.dll by itself.\n\
If no LIBINT_DLL is given or loading LIBINT_DLL failed, environment variable\n\
%%%ls%% will be used.\n\
If this also fails, %ls will be searched in Path.\n\
By default, locales are searched from the executable position to\n\
../share/locale. But depending on system, this could not work.\n\
If you have the problem you can provide locales directory either by setting\n\
environment variable %%%s%%.\n\
"),
           APPDEP_APPLICATION, APPDEP_APPLICATION, APPDEP_APPLICATION,
           APPDEP_APPLICATION, DEFAULT_DEM_LINE,
           VAR_LIBICONV, VAR_LIBINT, PATTERN_LIBINT, VAR_LOCALEDIR);
    result = RETURN_OK;
  }
  else if (version_flag)
  {
    // Show version
    printf(_("%s version %d.%d.%d.%d\n"),
           APPDEP_APPLICATION,
           APPDEP_VER_MAJ,
           APPDEP_VER_MIN,
           APPDEP_VER_REV,
           APPDEP_VER_BUILD);

    result = RETURN_OK;
  }
  else
  {
    // Update required file
    result = update_file(argv[woptind], dem_line, null_flag, quiet_flag);

    if (!quiet_flag)
    {
      switch (result)
      {
      case RETURN_OPEN_ERR:
        fprintf(stderr,
                _("File %ls was not found or was not reachable.\n"),
                argv[woptind]);
        break;

      case RETURN_READ_ERR:
        fprintf(stderr, _("Read error in file %ls.\n"), argv[woptind]);
        break;

      case RETURN_WRITE_ERR:
        fprintf(stderr, _("Write error in file %ls.\n"), argv[woptind]);
        break;

      case RETURN_STDIN_ERR:
        fprintf(stderr, _("Read error in standard input.\n"));
        break;
      }
    }
  }

  // Free memory
  close_gettext();

  return result;
}

/**
 * \brief Application entry point.
 * Computes wide string arguments and transmits them to wmain().
 * \since 2.0
 * \return Result from wmain().
 */
int main()
{
  LPWSTR *sz_arglist;
  int n_args;
  int result;

  sz_arglist = CommandLineToArgvW(GetCommandLineW(), &n_args);
  if (sz_arglist == NULL)
  {
     fprintf(stderr, _("CommandLineToArgvW() failed.\n"));
     return 1;
  }
  else
  {
    result = wmain(n_args, sz_arglist);
  }

  LocalFree(sz_arglist);

  return result;
}
