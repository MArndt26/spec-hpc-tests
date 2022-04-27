/*
 *  Copyright (C) 2015, Northwestern University and Argonne National Laboratory
 *  See COPYRIGHT notice in top-level directory.
 */
/* $Id$ */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <string.h>
#include <ctype.h>  /* isspace() */
#include <assert.h>
#include <mpi.h>

#include <pnetcdf.h>
#include <pnc_debug.h>
#include <common.h>

/* There are 3 levels of UTF8 checking: 1=> (exact)validating 2=>relaxed
   and 3=>very relaxed
*/
/* Use semi-relaxed check */
#define UTF8_CHECK 2

static int
nextUTF8(const char* cp)
{
    /*  The goal here is to recognize the length of each
	multibyte utf8 character sequence and skip it.
        Again, we assume that every non-ascii character is legal.
        We can define three possible tests of decreasing correctness
        (in the sense that the least correct will allow some sequences that
        are technically illegal UTF8).
        As Regular expressions they are as follows:
        1. most correct:
            UTF8   ([\xC2-\xDF][\x80-\xBF])                       \
                 | (\xE0[\xA0-\xBF][\x80-\xBF])                   \
                 | ([\xE1-\xEC][\x80-\xBF][\x80-\xBF])            \
                 | (\xED[\x80-\x9F][\x80-\xBF])                   \
                 | ([\xEE-\xEF][\x80-\xBF][\x80-\xBF])            \
                 | (\xF0[\x90-\xBF][\x80-\xBF][\x80-\xBF])        \
                 | ([\xF1-\xF3][\x80-\xBF][\x80-\xBF][\x80-\xBF]) \
                 | (\xF4[\x80-\x8F][\x80-\xBF][\x80-\xBF])        \

        2. partially relaxed:
            UTF8 ([\xC0-\xDF][\x80-\xBF])
                 |([\xE0-\xEF][\x80-\xBF][\x80-\xBF])
                 |([\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF])

        3. The most relaxed version of UTF8:
            UTF8 ([\xC0-\xD6].)|([\xE0-\xEF]..)|([\xF0-\xF7]...)

        We use #2 here.

	The tests are derived from the table at
	    http://www.w3.org/2005/03/23-lex-U
    */

/* Define a test macro to test against a range */
#define RANGE(c,lo,hi) (((uchar)c) >= lo && ((uchar)c) <= hi)
/* Define a common RANGE */
#define RANGE0(c) RANGE(c,0x80,0xBF)

    int ch0;

    int skip = -1; /* assume failed */

    ch0 = (uchar)*cp;
    if(ch0 <= 0x7f) skip = 1; /* remove ascii case */
    else

#if UTF8_CHECK == 2
    /* Do relaxed validation check */
    if(RANGE(ch0,0xC0,0XDF)) {/* 2-bytes, but check */
        if(cp[1] != 0 && RANGE0(cp[1]))
		skip = 2; /* two bytes */
    } else if(RANGE(ch0,0xE0,0XEF)) {/* 3-bytes, but check */
        if(cp[1] != 0 && RANGE0(cp[1]) && cp[2] != 0 && RANGE0(cp[1]))
		skip = 3; /* three bytes */
    } else if(RANGE(ch0,0xF0,0XF7)) {/* 3-bytes, but check */
        if(cp[1] != 0 && RANGE0(cp[1]) && cp[2] != 0
           && RANGE0(cp[1]) && cp[3] != 0 && RANGE0(cp[1]))
		skip = 4; /* four bytes*/
    }
#elif UTF8_CHECK == 1
    /* Do exact validation check */
    if(RANGE(ch0,0xC2,0xDF)) {/* non-overlong 2-bytes */
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE0(ch1)) skip = 2;
    } else if((ch0 == 0xE0)) {/* 3-bytes, not overlong */
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE(ch1,0xA0,0xBF)) {
	    int ch2 = (uchar)cp[2];
	    if(ch2 != 0 && RANGE0(ch2)) skip = 3;
	}
    } else if((ch0 == 0xED)) {/* 3-bytes minus surrogates */
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE(ch1,0x80,0x9f)) {
	    int ch2 = (uchar)cp[2];
	    if(ch2 != 0 && RANGE0(ch2)) skip = 3;
	}
    } else if(RANGE(ch0,0xE1,0xEC) || ch0 == 0xEE || ch0 == 0xEF) {
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE0(ch1)) {
	    int ch2 = (uchar)cp[2];
	    if(ch2 != 0 && RANGE0(ch2)) skip = 3;
	}
    } else if((ch0 == 0xF0)) {/* planes 1-3 */
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE(ch1,0x90,0xBF)) {
	    int ch2 = (uchar)cp[2];
	    if(ch2 != 0 && RANGE0(ch2)) {
	        int ch3 = (uchar)cp[3];
	        if(ch3 != 0 && RANGE0(ch3)) skip = 4;
	    }
	}
    } else if((ch0 == 0xF4)) {/* plane 16 */
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE0(ch1)) {
	    int ch2 = (uchar)cp[2];
	    if(ch2 != 0 && RANGE0(ch2)) {
	        int ch3 = (uchar)cp[3];
	        if(ch3 != 0 && RANGE0(ch3)) skip = 4;
	    }
	}
    } else if(RANGE(ch0,0xF1,0xF3)) { /* planes 4-15 */
	int ch1 = (uchar)cp[1];
	if(ch1 != 0 && RANGE0(ch1)) {
	    int ch2 = (uchar)cp[2];
	    if(ch2 != 0 && RANGE0(ch2)) {
	        int ch3 = (uchar)cp[3];
	        if(ch3 != 0 && RANGE0(ch3)) skip = 4;
	    }
	}
    }
#else
#error "Must Define UTF8_CHECK as 1 or 2"
#endif
    return skip;
}


#ifdef _CONFORM_NETCDF_3_5_1
/*
 * For CDF-1, Verify that a name string is valid
 * CDL syntax, eg, all the characters are
 * alphanumeric, '-', '_', or '.'.
 * Also permit ':', '@', '(', or ')' in names for chemists currently making
 * use of these characters, but don't document until ncgen and ncdump can
 * also handle these characters in names.
 */
static int
check_name_CDF1(const char *name)
{
    const char *cp = name;
    assert(name != NULL);

    if (*name == 0)
        DEBUG_RETURN_ERROR(NC_EBADNAME) /* empty names disallowed */

    for (; *cp != 0; cp++) {
        int ch = *cp;
        if (!isalnum(ch)) {
            if (ch != '_' && ch != '-' && ch != '+' && ch != '.' &&
                ch != ':' && ch != '@' && ch != '(' && ch != ')')
                DEBUG_RETURN_ERROR(NC_EBADNAME)
        }
    }
    if (cp - name > NC_MAX_NAME)
        DEBUG_RETURN_ERROR(NC_EMAXNAME)

    return NC_NOERR;
}
#endif

/*
 * Verify that a name string is valid syntax.  The allowed name
 * syntax (in RE form) is:
 *
 * ([a-zA-Z_]|{UTF8})([^\x00-\x1F\x7F/]|{UTF8})*
 *
 * where UTF8 represents a multibyte UTF-8 encoding.  Also, no
 * trailing spaces are permitted in names.  This definition
 * must be consistent with the one in ncgen.l.  We do not allow '/'
 * because HDF5 does not permit slashes in names as slash is used as a
 * group separator.  If UTF-8 is supported, then a multi-byte UTF-8
 * character can occur anywhere within an identifier.  We later
 * normalize UTF-8 strings to NFC to facilitate matching and queries.
 */
static int
check_name_CDF2(const char *name)
{
	int skip, ch, err;
	const char *cp = name;

	assert(name != NULL);

	if(*name == 0		/* empty names disallowed */
	   || strchr(cp, '/'))	/* '/' can't be in a name */
		DEBUG_RETURN_ERROR(NC_EBADNAME)

	/* check validity of any UTF-8 */
        err = ncmpii_utf8_validate(name);
	if (err != NC_NOERR) return err;

	/* First char must be [a-z][A-Z][0-9]_ | UTF8 */
	ch = (uchar)*cp;
	if(ch <= 0x7f) {
	    if(!('A' <= ch && ch <= 'Z')
	       && !('a' <= ch && ch <= 'z')
               && !('0' <= ch && ch <= '9')
	       && ch != '_' )
		DEBUG_RETURN_ERROR(NC_EBADNAME)
	    cp++;
	} else {
	    if((skip = nextUTF8(cp)) < 0)
		DEBUG_RETURN_ERROR(NC_EBADNAME)
	    cp += skip;
	}

	while(*cp != 0) {
	    ch = (uchar)*cp;
	    /* handle simple 0x00-0x7f characters here */
	    if(ch <= 0x7f) {
                if( ch < ' ' || ch > 0x7E) /* control char or DEL */
		  DEBUG_RETURN_ERROR(NC_EBADNAME)
		cp++;
	    } else {
		if((skip = nextUTF8(cp)) < 0) DEBUG_RETURN_ERROR(NC_EBADNAME)
		cp += skip;
	    }
	    if(cp - name > NC_MAX_NAME)
		DEBUG_RETURN_ERROR(NC_EMAXNAME)
	}
	if(ch <= 0x7f && isspace(ch)) /* trailing spaces disallowed */
	    DEBUG_RETURN_ERROR(NC_EBADNAME)
	return NC_NOERR;
}

/*----< ncmpii_check_name() >------------------------------------------------*/
int
ncmpii_check_name(const char *name,
                  int         file_ver) /* CDF version: NC_FORMAT_CLASSIC,
                                           NC_FORMAT_CDF2, or NC_FORMAT_CDF5 */
{
    /* NetCDF4 has made CDF-1 no different from CDF-2 except the size of
     * OFFSET (i.e. 32-bit vs. 64-bit integer. Both formats support extended
     * names now.
     */
#ifdef _CONFORM_NETCDF_3_5_1
    if (file_ver == NC_FORMAT_CLASSIC)
        return check_name_CDF1(name);
#endif

    return check_name_CDF2(name);
}

