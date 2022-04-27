/* src/include/config.h.  Generated from config.h.in by configure.  */
/* src/include/config.h.in.  Generated from configure.ac by autoheader.  */

/*
 * Copyright (C) 2003, Northwestern University and Argonne National Laboratory
 * See COPYRIGHT notice in top-level directory.
 */


#ifndef _CONFIG_H
#define _CONFIG_H

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define if to disable MPI_File_sync */
/* #undef DISABLE_FILE_SYNC */

/* Define if to enable ADIOS BP read feature */
/* #undef ENABLE_ADIOS */

/* Define if to enable burst buffer feature */
/* #undef ENABLE_BURST_BUFFER */

/* Define if to enable C++ feature */
#define ENABLE_CXX 1

/* Define if to enable Fortran feature */
#define ENABLE_FORTRAN 1

/* Define if to enable large single MPI-IO request */
/* #undef ENABLE_LARGE_SINGLE_REQ */

/* Define if to enable NetCDF-4 support */
/* #undef ENABLE_NETCDF4 */

/* Define if to enable strict null-byte padding in file header */
/* #undef ENABLE_NULL_BYTE_HEADER_PADDING */

/* Define if able to support request aggregation in nonblocking routines */
#define ENABLE_REQ_AGGREGATION 1

/* Define if to enable subfiling feature */
/* #undef ENABLE_SUBFILING */

/* Define if to enable thread-safe capability */
/* #undef ENABLE_THREAD_SAFE */

/* Define if Fortran names are lower case */
/* #undef F77_NAME_LOWER */

/* Define if Fortran names are lower case with two trailing underscore2 */
/* #undef F77_NAME_LOWER_2USCORE */

/* Define if Fortran names are lower case with one trailing underscore */
#define F77_NAME_LOWER_USCORE /**/

/* Define if Fortran names are uppercase */
/* #undef F77_NAME_UPPER */

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
/* #undef FC_DUMMY_MAIN */

/* Define if F77 and FC dummy `main' functions are identical. */
/* #undef FC_DUMMY_MAIN_EQ_F77 */

/* Define to 1 if you have the `access' function. */
#define HAVE_ACCESS 1

/* Define to 1 if you have the <adios2_c.h> header file. */
/* #undef HAVE_ADIOS2_C_H */

/* Define to 1 if you have the <adios_read.h> header file. */
/* #undef HAVE_ADIOS_READ_H */

/* available */
/* #undef HAVE_DECL_MPI_OFFSET */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if the system has the type `int64'. */
/* #undef HAVE_INT64 */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
/* #undef HAVE_LIBPTHREAD */

/* Define to 1 if the system has the type `longlong'. */
/* #undef HAVE_LONGLONG */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if MPI_COMBINER_HINDEXED_INTEGER is defined and not deprecated */
#define HAVE_MPI_COMBINER_HINDEXED_INTEGER 1

/* Define if MPI_COMBINER_HVECTOR_INTEGER is defined and not deprecated */
#define HAVE_MPI_COMBINER_HVECTOR_INTEGER 1

/* Define if MPI_COMBINER_STRUCT_INTEGER is defined and not deprecated */
#define HAVE_MPI_COMBINER_STRUCT_INTEGER 1

/* Define to 1 if you have the <netcdf_meta.h> header file. */
/* #undef HAVE_NETCDF_META_H */

/* Define to 1 if you have the <netcdf_par.h> header file. */
/* #undef HAVE_NETCDF_PAR_H */

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if the system has the type `schar'. */
/* #undef HAVE_SCHAR */

/* Define to 1 if you have the <search.h> header file. */
#define HAVE_SEARCH_H 1

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `tdelete' function. */
#define HAVE_TDELETE 1

/* Define to 1 if you have the `truncate' function. */
#define HAVE_TRUNCATE 1

/* Define to 1 if you have the `tsearch' function. */
#define HAVE_TSEARCH 1

/* Define to 1 if the system has the type `uchar'. */
/* #undef HAVE_UCHAR */

/* Define to 1 if the system has the type `uint'. */
#define HAVE_UINT 1

/* Define to 1 if the system has the type `uint64'. */
/* #undef HAVE_UINT64 */

/* Define to 1 if the system has the type `ulonglong'. */
/* #undef HAVE_ULONGLONG */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `unlink' function. */
#define HAVE_UNLINK 1

/* Define to 1 if the system has the type `ushort'. */
#define HAVE_USHORT 1

/* Define to 1 if you have the `usleep' function. */
/* #undef HAVE_USLEEP */

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define if HDF5 version is at least 1.10.4 */
/* #undef HDF5_VER_GE_1_10_4 */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Type of NC_BYTE */
/* #undef NCBYTE_T */

/* Type of NC_SHORT */
/* #undef NCSHORT_T */

/* Define if NetCDF version is at least 4.5.0 */
/* #undef NETCDF_GE_4_5_0 */

/* C type for Fortran double */
/* #undef NF_DOUBLEPRECISION_IS_C_ */

/* C type for Fortran INT1 */
/* #undef NF_INT1_IS_C_ */

/* Type for Fortran INT1 */
#define NF_INT1_T integer*1

/* C type for Fortran INT2 */
/* #undef NF_INT2_IS_C_ */

/* Type for Fortran INT2 */
#define NF_INT2_T integer*2

/* C type for Fortran INT8 */
/* #undef NF_INT8_IS_C_ */

/* Type for Fortran INT8 */
#define NF_INT8_T integer*8

/* C type for Fortran INT */
/* #undef NF_INT_IS_C_ */

/* C type for Fortran REAL */
/* #undef NF_REAL_IS_C_ */

/* Does system have IEEE FLOAT */
/* #undef NO_IEEE_FLOAT */

/* Name of package */
#define PACKAGE "pnetcdf"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "parallel-netcdf@mcs.anl.gov"

/* Define to the full name of this package. */
#define PACKAGE_NAME "PnetCDF"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "PnetCDF 1.12.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "pnetcdf"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://parallel-netcdf.github.io"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.12.3"

/* Define if to enable malloc tracing */
/* #undef PNC_MALLOC_TRACE */

/* Define if to enable PnetCDF internal performance profiling */
/* #undef PNETCDF_PROFILING */

/* Define if relaxed coordinate check is enabled */
#define RELAX_COORD_BOUND 1

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `float', as computed by sizeof. */
#define SIZEOF_FLOAT 4

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `longlong', as computed by sizeof. */
/* #undef SIZEOF_LONGLONG */

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `MPI_Aint', as computed by sizeof. */
#define SIZEOF_MPI_AINT 8

/* The size of `MPI_Fint', as computed by sizeof. */
#define SIZEOF_MPI_FINT 4

/* The size of `MPI_Offset', as computed by sizeof. */
#define SIZEOF_MPI_OFFSET 8

/* The size of `off_t', as computed by sizeof. */
#define SIZEOF_OFF_T 8

/* The size of `ptrdiff_t', as computed by sizeof. */
#define SIZEOF_PTRDIFF_T 8

/* The size of `schar', as computed by sizeof. */
/* #undef SIZEOF_SCHAR */

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `signed char', as computed by sizeof. */
#define SIZEOF_SIGNED_CHAR 1

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 8

/* The size of `uchar', as computed by sizeof. */
/* #undef SIZEOF_UCHAR */

/* The size of `uint', as computed by sizeof. */
#define SIZEOF_UINT 4

/* The size of `ulonglong', as computed by sizeof. */
/* #undef SIZEOF_ULONGLONG */

/* The size of `unsigned char', as computed by sizeof. */
#define SIZEOF_UNSIGNED_CHAR 1

/* The size of `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT 4

/* The size of `unsigned long long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG_LONG 8

/* The size of `unsigned short', as computed by sizeof. */
#define SIZEOF_UNSIGNED_SHORT 2

/* The size of `unsigned short int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_SHORT_INT 2

/* The size of `ushort', as computed by sizeof. */
#define SIZEOF_USHORT 2

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if performing coverage tests */
/* #undef USE_COVERAGE */

/* Version number of package */
#define VERSION "1.12.3"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 if type `char' is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
/* # undef __CHAR_UNSIGNED__ */
#endif

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

#include <nctypes.h>
#endif
