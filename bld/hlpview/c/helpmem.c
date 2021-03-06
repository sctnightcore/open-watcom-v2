/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Memory allocation/deallocation routines.
*
****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include "wio.h"
#include "helpmem.h"
#ifdef TRMEM
    #include "trmemcvr.h"
#endif


#ifdef TRMEM

#define malloc      TRMemAlloc
#define free        TRMemFree
#define realloc     TRMemRealloc

static FILE         *memFP = NULL;

#endif

void HelpMemOpen( void )
{
#ifdef TRMEM
    TRMemOpen();
    memFP = fopen( "MEMERR", "w" );
    TRMemRedirect( memFP );
#endif
}

void HelpMemClose( void )
{
#ifdef TRMEM
//    TRMemPrtList();
    TRMemClose();
    if( ftell( memFP ) != 0 ) {
        printf( "***************************\n" );
        printf( "* A memory error occurred *\n" );
        printf( "***************************\n" );
    }
    fclose( memFP );
#endif
}

HELPMEM void *HelpMemAlloc( size_t size )
{
    return( malloc( size ) );
}

HELPMEM void *HelpMemRealloc( void *ptr, size_t size )
{
    return( realloc( ptr, size ) );
}

HELPMEM void HelpMemFree( void *ptr )
{
    free( ptr );
}
