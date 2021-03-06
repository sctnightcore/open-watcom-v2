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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "uidef.h"
#include "uiforce.h"
#include <sys/types.h>
#include <sys/time.h>
#include "uivirts.h"
#include "uiintern.h"


MOUSETIME UIAPI uiclock( void )
/*****************************
 * this routine get time in platform dependant units,
 * used for mouse & timer delays
 */
{
    struct timeval  timev;

    gettimeofday( &timev, NULL );
    /* return time in miliseconds */
    return( timev.tv_usec / 1000 + timev.tv_sec * 1000 );
}

unsigned UIAPI uiclockdelay( unsigned milli )
/*******************************************
 * this routine converts milli-seconds into platform
 * dependant units - used to set mouse & timer delays
 */
{
    return( milli );
}

void UIAPI uiflush( void )
/*************************/
{
    uiflushevent();
    flushkey();
}

static ui_event doget( bool update )
/**********************************/
{
    static short    ReturnIdle = 1;
    ui_event        ui_ev;

    for( ;; ) {
        ui_ev = forcedevent();
        if( ui_ev > EV_NO_EVENT )
            break;
        ui_ev = _uievent();
        if( ui_ev > EV_NO_EVENT )
            break;
        if( ReturnIdle ) {
            --ReturnIdle;
            return( EV_IDLE );
        } else {
            if( update )
                uirefresh();
            if( UIData->busy_wait ) {
                return( EV_SINK );
            }
        }
        _uiwaitkeyb( 60, 0 );
    }
    ReturnIdle = 1;
    if( ui_ev == EV_REDRAW_SCREEN ) {
        SAREA   screen;

        screen.row = 0;
        screen.col = 0;
        screen.height = UIData->height;
        screen.width = UIData->width;
        uidirty( screen );
        UserForcedTermRefresh = true;
        physupdate( &screen );
    }
    return( ui_ev );
}

ui_event UIAPI uieventsource( bool update )
/*****************************************/
{
    ui_event    ui_ev;

    ui_ev = doget( update );
    _stopmouse();
    _stopkeyb();
    return( uieventsourcehook( ui_ev ) );
}


ui_event UIAPI uiget( void )
/**************************/
{
    return( uieventsource( true ) );
}
