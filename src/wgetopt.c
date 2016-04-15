/*****************************************************************************
 * wgetopt.c                 processing command line wide strings parameters *
 * Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr *
 * Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home *
 * Copyright 2016 Julien Darthenay                                           *
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
 * \file wgetopt.c
 * \brief Processing command line wide strings parameters.
 * \author <a href="mailto:keithmarshall@users.sourceforge.net">Keith Marshall</a>
 * \author <a href="mailto:julien.darthenay@free.fr">Julien Darthenay</a>
 * \version 1.0
 */

/*
 * Please note this file is not original work.
 * Original file is getopt.c from MinGW pakage set, was written by
 * Keith Marshall <keithmarshall@users.sourceforge.net>
 * it can be found here:
 * https://sourceforge.net/p/mingw/mingw-org-wsl/ci/legacy/tree/mingwrt/mingwex/getopt.c
 * bluebaroncanada <https://github.com/bluebaroncanada> used it in
 * getoptW <https://github.com/bluebaroncanada/getoptW>
 * and wrote file getoptW.c which can be found here:
 * https://github.com/bluebaroncanada/getoptW/blob/master/getoptW.c
 *
 * Below is the copyright information from getoptW.c which includes
 * copyright information from getopt.c
 *
 *
 * getopt.c
 *
 * Implementation of the `getopt', `getopt_long' and `getopt_long_only'
 * APIs, for inclusion in the MinGW runtime library.
 *
 * This file is part of the MinGW32 package set.
 *
 * Written by Keith Marshall <keithmarshall@users.sourceforge.net>
 * Copyright (C) 2008, 2009, 2011, 2012, MinGW.org Project.
 *
 * ---------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, this permission notice, and the following
 * disclaimer shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This licence is governed by the laws of Ontario, Canada and any dispute
 * shall be finally resolved by the courts in London, Ontario, Canada.
 *
 * ---------------------------------------------------------------------------
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "wgetopt.h"
#include <crtdefs.h>

/* Identify how to get the calling program name, for use in messages...
 */
#ifdef __CYGWIN__
/*
 * CYGWIN uses this DLL reference...
 */
# define PROGNAME  __progname
extern char __declspec(dllimport) *__progname;
#else
/*
 * ...while elsewhere, we simply use the first argument passed.
 */
# define PROGNAME  *argv
#endif

/* Initialise the public variables. */

int woptind = 1;                /* index for first non-option arg     */
int wopterr = 1;                /* enable built-in error messages     */


#define WCHAR  wchar_t          /* argument type selector */

WCHAR *woptarg = NULL;          /* pointer to current option argument */

#define wgetopt_switchar         L'-'   /* option prefix character in argv    */
#define wgetopt_pluschar         L'+'   /* prefix for POSIX mode in optstring */
#define wgetopt_takes_argument   L':'   /* marker for optarg in optstring     */
#define wgetopt_arg_assign       L'='   /* longopt argument field separator   */
#define wgetopt_unknown          L'?'   /* return code for unmatched option   */
#define wgetopt_ordered           1     /* return code for ordered non-option */

#define wgetopt_all_done         -1     /* return code to indicate completion */

enum
{ /* All `wgetopt' API functions are implemented via calls to the
   * common static function `wgetopt_parse()'; these `mode' selectors
   * determine the behaviour of `wgetopt_parse()', to deliver the
   * appropriate result in each case.
   */
            wgetopt_mode_standard = 0,  /* wgetopt()           */
            wgetopt_mode_long,          /* wgetopt_long()      */
            wgetopt_mode_long_only      /* wgetopt_long_only() */
};

enum
{ /* When attempting to match a command line argument to a long form option,
   * these indicate the status of the match.
   */
            wgetopt_no_match = 0,       /* no successful match                       */
            wgetopt_abbreviated_match,  /* argument is an abbreviation for an option */
            wgetopt_exact_match         /* argument matches the full option name     */
};

int woptopt = wgetopt_unknown;   /* return value for option being evaluated   */

/* Some BSD applications expect to be able to reinitialise `wgetopt' parsing
 * by setting a global variable called `optreset'.  We provide an obfuscated
 * API, which allows applications to emulate this brain damage; however, any
 * use of this is non-portable, and is strongly discouraged.
 */
#define optreset  __mingw_optreset
int optreset = 0;

static __inline__
int wgetopt_missing_arg( const WCHAR *optstring )
{
    /* Helper function to determine the appropriate return value,
     * for the case where a required option argument is missing.
     */
    if( (*optstring == wgetopt_pluschar) || (*optstring == wgetopt_switchar) )
        ++optstring;
    return (*optstring == wgetopt_takes_argument)
           ? wgetopt_takes_argument
           : wgetopt_unknown;
}

/* `complain' macro facilitates the generation of simple built-in
 * error messages, displayed on various fault conditions, provided
 * `wopterr' is non-zero.
 */
#define complain( MSG, ARG )  if( wopterr ) \
  fprintf( stderr, "%ls: " MSG "\n", PROGNAME, ARG )

static __inline__
int wgetopt_argerror( int mode, char *fmt, WCHAR *prog, struct woption *opt, int retval )
{
    /* Helper function, to generate more complex built-in error
     * messages, for invalid arguments to long form options ...
     */
    if( wopterr )
    {
        /* ... but, displayed only if `wopterr' is non-zero.
         */
        WCHAR flag[] = L"--";
        if( mode != wgetopt_mode_long )
            /*
             * only display one hyphen, for implicit long form options,
             * improperly resolved by `wgetopt_long_only()'.
             */
            flag[1] = L'\0';
        /*
         * always preface the program name ...
         */
        fprintf( stderr, "%ls: ", prog );
        /*
         * to the appropriate, option specific message.
         */
        fprintf( stderr, fmt, flag, opt->name );
    }
    /* Whether displaying the message, or not, always set `woptopt'
     * to identify the faulty option ...
     */
    woptopt = opt->val;
    /*
     * and return the `invalid option' indicator.
     */
    return retval;
}

/* `wgetopt_conventions' establish behavioural options, to control
 * the operation of `wgetopt_parse()', e.g. to select between POSIX
 * and GNU style argument parsing behaviour.
 */
#define wgetopt_set_conventions  0x1000
#define wgetopt_posixly_correct  0x0010

static __inline__
int wgetopt_conventions( int flags )
{
    static int conventions = 0;

    if( (conventions == 0) && ((flags & wgetopt_set_conventions) == 0) )
    {
        /* default conventions have not yet been established;
         * initialise them now!
         */
        conventions = wgetopt_set_conventions;
        if( (flags == wgetopt_pluschar) || (getenv( "POSIXLY_CORRECT" ) != NULL) )
            conventions |= wgetopt_posixly_correct;
    }

    else if( flags & wgetopt_set_conventions )
        /*
         * default conventions may have already been established,
         * but this is a specific request to augment them.
         */
        conventions |= flags;

    /* in any event, return the currently established conventions.
     */
    return conventions;
}

static __inline__
int is_switchar( WCHAR flag )
{
    /* A simple helper function, used to identify the switch character
     * introducing an optional command line argument.
     */
    return flag == wgetopt_switchar;
}

static __inline__
const WCHAR *wgetopt_match( WCHAR lookup, const WCHAR *opt_string )
{
    /* Helper function, used to identify short form options.
     */
    if( (*opt_string == wgetopt_pluschar) || (*opt_string == wgetopt_switchar) )
        ++opt_string;
    if( *opt_string == wgetopt_takes_argument )
        ++opt_string;
    do if( lookup == *opt_string ) return opt_string;
    while( *++opt_string );
    return NULL;
}

static __inline__
int wgetopt_match_long( const WCHAR *nextchar, const WCHAR *optname )
{
    /* Helper function, used to identify potential matches for
     * long form options.
     */
    WCHAR matchchar;
    while( (matchchar = *nextchar++) && (matchchar == *optname) )
        /*
         * skip over initial substring which DOES match.
         */
        ++optname;

    if( matchchar )
    {
        /* did NOT match the entire argument to an initial substring
         * of a defined option name ...
         */
        if( matchchar != wgetopt_arg_assign )
            /*
             * ... and didn't stop at an `=' internal field separator,
             * so this is NOT a possible match.
             */
            return wgetopt_no_match;

        /* DID stop at an `=' internal field separator,
         * so this IS a possible match, and what follows is an
         * argument to the possibly matched option.
         */
        woptarg = (WCHAR *)(nextchar);
    }
    return *optname
           /*
            * if we DIDN'T match the ENTIRE text of the option name,
            * then it's a possible abbreviated match ...
            */
           ? wgetopt_abbreviated_match
           /*
            * but if we DID match the entire option name,
            * then it's a DEFINITE EXACT match.
            */
           : wgetopt_exact_match;
}

static __inline__
int wgetopt_resolved( int mode, int argc, WCHAR *const *argv, int *argind,
                     struct woption *opt, int index, int *retindex, const WCHAR *optstring )
{
    /* Helper function to establish appropriate return conditions,
     * on resolution of a long form option.
     */
    if( retindex != NULL )
        *retindex = index;

    /* On return, `optind' should normally refer to the argument, if any,
     * which follows the current one; it is convenient to set this, before
     * checking for the presence of any `woptarg'.
     */
    woptind = *argind + 1;

    if( woptarg && (opt[index].has_arg == wno_argument) )
        /*
         * it is an error for the user to specify an option specific argument
         * with an option which doesn't expect one!
         */
        return wgetopt_argerror( mode, "option `%ls%ls' doesn't accept an argument\n",
                                PROGNAME, opt + index, wgetopt_unknown );

    else if( (woptarg == NULL) && (opt[index].has_arg == wrequired_argument) )
    {
        /* similarly, it is an error if no argument is specified
         * with an option which requires one ...
         */
        if( woptind < argc )
            /*
             * ... except that the requirement may be satisfied from
             * the following command line argument, if any ...
             */
            woptarg = argv[*argind = woptind++];

        else
            /* so fail this case, only if no such argument exists!
             */
            return wgetopt_argerror( mode, "option `%ls%ls' requires an argument\n",
                                    PROGNAME, opt + index, wgetopt_missing_arg( optstring ) );
    }

    /* when the caller has provided a return buffer ...
     */
    if( opt[index].flag != NULL )
    {
        /* ... then we place the proper return value there,
         * and return a status code of zero ...
         */
        *(opt[index].flag) = opt[index].val;
        return 0;
    }
    /* ... otherwise, the return value becomes the status code.
     */
    return opt[index].val;
}

static __inline__
int wgetopt_verify( const WCHAR *nextchar, const WCHAR *optstring )
{
    /* Helper function, called by wgetopt_parse() when invoked
     * by wgetopt_long_only(), to verify when an unmatched or an
     * ambiguously matched long form option string is valid as
     * a short form option specification.
     */
    if( ! (nextchar && *nextchar && optstring && *optstring) )
        /*
         * There are no characters to be matched, or there are no
         * valid short form option characters to which they can be
         * matched, so this can never be valid.
         */
        return 0;

    while( *nextchar )
    {
        /* For each command line character in turn ...
         */
        const WCHAR *test;
        if( (test = wgetopt_match( *nextchar++, optstring )) == NULL )
            /*
             * ... there is no short form option to match the current
             * candidate, so the entire argument fails.
             */
            return 0;

        if( test[1] == wgetopt_takes_argument )
            /*
             * The current candidate is valid, and it matches an option
             * which takes an argument, so this command line argument is
             * a valid short form option specification; accept it.
             */
            return 1;
    }
    /* If we get to here, then every character in the command line
     * argument was valid as a short form option; accept it.
     */
    return 1;
}

static
#define wgetopt_std_args int argc, WCHAR *const argv[], const WCHAR *optstring
int wgetopt_parse( int mode, wgetopt_std_args, ... )
{
    /* Common core implementation for ALL `wgetopt' functions.
     */
    static int argind = 0;
    static int optbase = 0;
    static const WCHAR *nextchar = NULL;
    static int optmark = 0;

    if( (optreset |= (woptind < 1)) || (woptind < optbase) )
    {
        /* POSIX does not prescribe any definitive mechanism for restarting
         * a `wgetopt' scan, but some applications may require such capability.
         * We will support it, by allowing the caller to adjust the value of
         * `optind' downwards, (nominally setting it to zero).  Since POSIX
         * wants `optind' to have an initial value of one, but we want all
         * of our internal place holders to be initialised to zero, when we
         * are called for the first time, we will handle such a reset by
         * adjusting all of the internal place holders to one less than
         * the adjusted `optind' value, (but never to less than zero).
         */
        if( optreset )
        {
            /* User has explicitly requested reinitialisation...
             * We need to reset `optind' to it's normal initial value of 1,
             * to avoid a potential infinitely recursive loop; by doing this
             * up front, we also ensure that the remaining place holders
             * will be correctly reinitialised to no less than zero.
             */
            woptind = 1;

            /* We also need to clear the `optreset' request...
             */
            optreset = 0;
        }

        /* Now, we may safely reinitialise the internal place holders, to
         * one less than `woptind', without fear of making them negative.
         */
        optmark = optbase = argind = woptind - 1;
        nextchar = NULL;
    }

        /* From a POSIX perspective, the following is `undefined behaviour';
         * we implement it thus, for compatibility with GNU and BSD wgetopt.
         */
    else if( woptind > (argind + 1) )
    {
        /* Some applications expect to be able to manipulate `woptind',
         * causing `wgetopt' to skip over one or more elements of `argv';
         * POSIX doesn't require us to support this brain-damaged concept;
         * (indeed, POSIX defines no particular behaviour, in the event of
         *  such usage, so it must be considered a bug for an application
         *  to rely on any particular outcome); nonetheless, Mac-OS-X and
         * BSD actually provide *documented* support for this capability,
         * so we ensure that our internal place holders keep track of
         * external `woptind' increments; (`argind' must lag by one).
         */
        argind = woptind - 1;

        /* When `woptind' is misused, in this fashion, we also abandon any
         * residual text in the argument we had been parsing; this is done
         * without any further processing of such abandoned text, assuming
         * that the caller is equipped to handle it appropriately.
         */
        nextchar = NULL;
    }

    if( nextchar && *nextchar )
    {
        /* we are parsing a standard, or short format, option argument ...
         */
        const WCHAR *optchar;
        if( (optchar = wgetopt_match( woptopt = *nextchar++, optstring )) != NULL )
        {
            /* we have identified it as valid ...
             */
            if( optchar[1] == wgetopt_takes_argument )
            {
                /* and determined that it requires an associated argument ...
                 */
                if( ! *(woptarg = (WCHAR *)(nextchar)) )
                {
                    /* the argument is NOT attached ...
                     */
                    if( optchar[2] == wgetopt_takes_argument )
                        /*
                         * but this GNU extension marks it as optional,
                         * so we don't provide one on this occasion.
                         */
                        woptarg = NULL;

                        /* otherwise this option takes a mandatory argument,
                         * so, provided there is one available ...
                         */
                    else if( (argc - argind) > 1 )
                        /*
                         * we take the following command line argument,
                         * as the appropriate option argument.
                         */
                        woptarg = argv[++argind];

                        /* but if no further argument is available,
                         * then there is nothing we can do, except for
                         * issuing the requisite diagnostic message.
                         */
                    else
                    {
                        complain( "option requires an argument -- %lc", woptopt );
                        return wgetopt_missing_arg( optstring );
                    }
                }
                woptind = argind + 1;
                nextchar = NULL;
            }
            else
                woptarg = NULL;
            woptind = (nextchar && *nextchar) ? argind : argind + 1;
            return woptopt;
        }
        /* if we didn't find a valid match for the specified option character,
         * then we fall through to here, so take appropriate diagnostic action.
         */
        if( mode == wgetopt_mode_long_only )
        {
            complain( "unrecognised option `-%ls'", --nextchar );
            nextchar = NULL;
            woptopt = 0;
        }
        else
        complain( "invalid option -- %lc", woptopt );
        woptind = (nextchar && *nextchar) ? argind : argind + 1;
        return wgetopt_unknown;
    }

    if( optmark > optbase )
    {
        /* This can happen, in GNU parsing mode ONLY, when we have
         * skipped over non-option arguments, and found a subsequent
         * option argument; in this case we permute the arguments.
         */
        int index;
        /*
         * `optspan' specifies the number of contiguous arguments
         * which are spanned by the current option, and so must be
         * moved together during permutation.
         */
        int optspan = argind - optmark + 1;
        /*
         * we use `this_arg' to store these temporarily.
         */
        WCHAR *this_arg[optspan];
        /*
         * we cannot manipulate `argv' directly, since the `wgetopt'
         * API prototypes it as `read-only'; this cast to `arglist'
         * allows us to work around that restriction.
         */
        WCHAR **arglist = (WCHAR **)(argv);

        /* save temporary copies of the arguments which are associated
         * with the current option ...
         */
        for( index = 0; index < optspan; ++index )
            this_arg[index] = arglist[optmark + index];

        /* move all preceding non-option arguments to the right,
         * overwriting these saved arguments, while making space
         * to replace them in their permuted location.
         */
        for( --optmark; optmark >= optbase; --optmark )
            arglist[optmark + optspan] = arglist[optmark];

        /* restore the temporarily saved option arguments to
         * their permuted location.
         */
        for( index = 0; index < optspan; ++index )
            arglist[optbase + index] = this_arg[index];

        /* adjust `optbase', to account for the relocated option.
         */
        optbase += optspan;
    }

    else
        /* no permutation occurred ...
         * simply adjust `optbase' for all options parsed so far.
         */
        optbase = argind + 1;

    /* enter main parsing loop ...
     */
    while( argc > ++argind )
    {
        /* inspect each argument in turn, identifying possible options ...
         */
        if( is_switchar( *(nextchar = argv[optmark = argind]) ) && *++nextchar )
        {
            /* we've found a candidate option argument ... */

            if( is_switchar( *nextchar ) )
            {
                /* it's a double hyphen argument ... */

                const WCHAR *refchar = nextchar;
                if( *++refchar )
                {
                    /* and it looks like a long format option ...
                     * `wgetopt_long' mode must be active to accept it as such,
                     * `wgetopt_long_only' also qualifies, but we must downgrade
                     * it to force explicit handling as a long format option.
                     */
                    if( mode >= wgetopt_mode_long )
                    {
                        nextchar = refchar;
                        mode = wgetopt_mode_long;
                    }
                }
                else
                {
                    /* this is an explicit `--' end of options marker, so wrap up now!
                     */
                    if( optmark > optbase )
                    {
                        /* permuting the argument list as necessary ...
                         * (note use of `this_arg' and `arglist', as above).
                         */
                        WCHAR *this_arg = argv[optmark];
                        WCHAR **arglist = (WCHAR **)(argv);

                        /* move all preceding non-option arguments to the right ...
                         */
                        do arglist[optmark] = arglist[optmark - 1];
                        while( optmark-- > optbase );

                        /* reinstate the `--' marker, in its permuted location.
                         */
                        arglist[optbase] = this_arg;
                    }
                    /* ... before finally bumping `optbase' past the `--' marker,
                     * and returning the `all done' completion indicator.
                     */
                    woptind = ++optbase;
                    return wgetopt_all_done;
                }
            }
            else if( mode < wgetopt_mode_long_only )
            {
                /* it's not an explicit long option, and `wgetopt_long_only' isn't active,
                 * so we must explicitly try to match it as a short option.
                 */
                mode = wgetopt_mode_standard;
            }

            if( mode >= wgetopt_mode_long )
            {
                /* the current argument is a long form option, (either explicitly,
                 * introduced by a double hyphen, or implicitly because we were called
                 * by `wgetopt_long_only'); this is where we parse it.
                 */
                int lookup;
                int matched = -1;

                /* we need to fetch the `extra' function arguments, which are
                 * specified for the `wgetopt_long' APIs.
                 */
                va_list refptr;
                va_start( refptr, optstring );
                struct woption *longopts = va_arg( refptr, struct woption * );
                int *optindex = va_arg( refptr, int * );
                va_end( refptr );

                /* ensuring that `woptarg' does not inherit any junk, from parsing
                 * preceding arguments ...
                 */
                woptarg = NULL;
                for( lookup = 0; longopts && longopts[lookup].name; ++lookup )
                {
                    /* scan the list of defined long form options ...
                     */
                    switch( wgetopt_match_long( nextchar, longopts[lookup].name ) )
                    {
                        /* looking for possible matches for the current argument.
                         */
                        case wgetopt_exact_match:
                            /*
                             * when an exact match is found,
                             * return it immediately, setting `nextchar' to NULL,
                             * to ensure we don't mistakenly try to match any
                             * subsequent characters as short form options.
                             */
                            nextchar = NULL;
                            return wgetopt_resolved( mode, argc, argv, &argind,
                                                    longopts, lookup, optindex, optstring );

                        case wgetopt_abbreviated_match:
                            /*
                             * but, for a partial (initial substring) match ...
                             */
                            if( matched >= 0 )
                            {
                                /* if this is not the first, then we have an ambiguity ...
                                 */
                                if( (mode == wgetopt_mode_long_only)
                                    /*
                                     * However, in the case of wgetopt_long_only(), if
                                     * the entire ambiguously matched string represents
                                     * a valid short option specification, then we may
                                     * proceed to interpret it as such.
                                     */
                                    &&  wgetopt_verify( nextchar, optstring )  )
                                    return wgetopt_parse( mode, argc, argv, optstring );

                                /* If we get to here, then the ambiguously matched
                                 * partial long option isn't valid for short option
                                 * evaluation; reset parser context to resume with
                                 * the following command line argument, diagnose
                                 * ambiguity, and bail out.
                                 */
                                woptopt = 0;
                                nextchar = NULL;
                                woptind = argind + 1;
                                complain( "option `%ls' is ambiguous", argv[argind] );
                                return wgetopt_unknown;
                            }
                            /* otherwise just note that we've found a possible match ...
                             */
                            matched = lookup;
                    }
                }
                if( matched >= 0 )
                {
                    /* if we get to here, then we found exactly one partial match,
                     * so return it, as for an exact match.
                     */
                    nextchar = NULL;
                    return wgetopt_resolved( mode, argc, argv, &argind,
                                            longopts, matched, optindex, optstring );
                }
                /* if here, then we had what SHOULD have been a long form option,
                 * but it is unmatched ...
                 */
                if( (mode < wgetopt_mode_long_only)
                    /*
                     * ... although paradoxically, `mode == wgetopt_mode_long_only'
                     * allows us to still try to match it as a short form option.
                     */
                    ||  (wgetopt_verify( nextchar, optstring ) == 0)  )
                {
                    /* When it cannot be matched, reset the parsing context to
                     * resume from the next argument, diagnose the failed match,
                     * and bail out.
                     */
                    woptopt = 0;
                    nextchar = NULL;
                    woptind = argind + 1;
                    complain( "unrecognised option `%ls'", argv[argind] );
                    return wgetopt_unknown;
                }
            }
            /* fall through to handle standard short form options...
             * when the option argument format is neither explictly identified
             * as long, nor implicitly matched as such, and the argument isn't
             * just a bare hyphen, (which isn't an option), then we make one
             * recursive call to explicitly interpret it as short format.
             */
            if( *nextchar )
                return wgetopt_parse( mode, argc, argv, optstring );
        }
        /* if we get to here, then we've parsed a non-option argument ...
         * in GNU compatibility mode, we step over it, so we can permute
         * any subsequent option arguments, but ...
         */
        if( *optstring == wgetopt_switchar )
        {
            /* if `optstring' begins with a `-' character, this special
             * GNU specific behaviour requires us to return the non-option
             * arguments in strict order, as pseudo-arguments to a special
             * option, with return value defined as `wgetopt_ordered'.
             */
            nextchar = NULL;
            woptind = argind + 1;
            woptarg = argv[argind];
            return wgetopt_ordered;
        }
        if( wgetopt_conventions( *optstring ) & wgetopt_posixly_correct )
            /*
             * otherwise ...
             * for POSIXLY_CORRECT behaviour, or if `optstring' begins with
             * a `+' character, then we break out of the parsing loop, so that
             * the scan ends at the current argument, with no permutation.
             */
            break;
    }
    /* fall through when all arguments have been evaluated,
     */
    woptind = optbase;
    return wgetopt_all_done;
}

/* All three public API entry points are trivially defined,
 * in terms of the internal `wgetopt_parse' function.
 */
int wgetopt( wgetopt_std_args )
{
    return wgetopt_parse( wgetopt_mode_standard, argc, argv, optstring );
}

int wgetopt_long( wgetopt_std_args, const struct woption *opts, int *index )
{
    return wgetopt_parse( wgetopt_mode_long, argc, argv, optstring, opts, index );
}

int wgetopt_long_only( wgetopt_std_args, const struct woption *opts, int *index )
{
    return wgetopt_parse( wgetopt_mode_long_only, argc, argv, optstring, opts, index );
}

#ifdef __weak_alias
/*
 * These Microsnot style uglified aliases are provided for compatibility
 * with the previous MinGW implementation of the wgetopt API.
 */
__weak_alias( wgetopt, _wgetopt )
__weak_alias( wgetopt_long, _wgetopt_long )
__weak_alias( wgetopt_long_only, _wgetopt_long_only )
#endif

/* $RCSfile$: end of file */
