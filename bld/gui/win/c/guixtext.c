/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2015-2016 The Open Watcom Contributors. All Rights Reserved.
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


#include "guiwind.h"
#include <string.h>
#include "guixutil.h"
#include "guicutil.h"
#include "guiscale.h"
#include "guicontr.h"

/*
 * GUIControlSetRedraw -- set the redraw flag for a given window control
 */
bool GUIControlSetRedraw( gui_window *wnd, gui_ctl_id id, bool redraw )
{
    HWND        hwnd;

    hwnd = _wpi_getdlgitem( wnd->hwnd, id );
    _wpi_setredraw( hwnd, ( redraw ) ? TRUE : FALSE );

    return( true );
}

/*
 * GUIAddText -- add the text to the given list/combo box
 */

bool GUIAddText( gui_window *wnd, gui_ctl_id id, const char *text )
{
    int                 pos;
#ifdef __OS2_PM__
    control_item        *item;
#endif

    if( text != NULL ) {
        pos = LIT_END;
#ifdef __OS2_PM__
        item = GUIGetControlByID( wnd, id );
        if( item != NULL ) {
            if( item->style & GUI_CONTROL_SORTED ) {
                pos = LIT_SORTASCENDING;
            }
        }
#endif
        GUIToComboList( wnd, id, LB_ADDSTRING, CB_ADDSTRING, (WPI_PARAM1)pos, (WPI_PARAM2)(LPSTR)text, (WPI_MRESULT)0 );
        return( true );
    }
    return( false );
}

bool GUIAddTextList( gui_window *wnd, gui_ctl_id id, int items, const void *data_handle, GUIPICKGETTEXT *getstring )
{
    int     item;

    GUIControlSetRedraw( wnd, id, false );
    for( item = 0; item < items; item++ ) {
        GUIAddText( wnd, id, (*getstring)( data_handle, item ) );
    }
    GUIControlSetRedraw( wnd, id, true );
    GUIControlDirty( wnd, id );
    return( true );
}

/*
 * GUIInsertText -- insert the text to the given list/combo box at position
 *                  indicated
 */

bool GUIInsertText( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice, const char *text )
{
    if( text != NULL ) {
        GUIToComboList( wnd, id, LB_INSERTSTRING, CB_INSERTSTRING, (WPI_PARAM1)choice, (WPI_PARAM2)(LPSTR)text, (WPI_MRESULT)0 );
        return( true );
    }
    return( false );
}

/*
 * GUISetTopIndex -- change which item is at the top of the list/combo box
 */

bool GUISetTopIndex( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice )
{
    GUIToComboList( wnd, id, LB_SETTOPINDEX, LB_SETTOPINDEX, (WPI_PARAM1)choice, (WPI_PARAM2)0, (WPI_MRESULT)0 );
    return( true );
}

/*
 * GUIGetTopIndex -- get index of item which is at the top of the list/combo
 *                   box
 */

gui_ctl_idx GUIGetTopIndex( gui_window *wnd, gui_ctl_id id )
{
    return( (gui_ctl_idx)GUIToComboList( wnd, id, LB_GETTOPINDEX, LB_GETTOPINDEX, (WPI_PARAM1)0, (WPI_PARAM2)0, (WPI_MRESULT)-1 ) );
}

/*
 * GUISetHorizontalExtent -- set the width of items in the list box
 */

bool GUISetHorizontalExtent( gui_window *wnd, gui_ctl_id id, int extent )
{
#ifndef __OS2_PM__
    gui_coord   coord;

    coord.x = extent;
    GUIScaleToScreenR( &coord );
    GUIToComboList( wnd, id, LB_SETHORIZONTALEXTENT, LB_SETHORIZONTALEXTENT, coord.x, (WPI_PARAM2)0, (WPI_MRESULT)0 );
    return( true );
#else
    wnd = wnd;
    id = id;
    extent = extent;
    return( false );
#endif
}

/*
 *  GUIGetListSize
 */

gui_ctl_idx GUIGetListSize( gui_window *wnd, gui_ctl_id id )
{
    return( (gui_ctl_idx)GUIToComboList( wnd, id, LB_GETCOUNT, CB_GETCOUNT, (WPI_PARAM1)0, (WPI_PARAM2)0, (WPI_MRESULT)0 ) );
}

bool GUIDeleteItem( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice )
{
    if( ( choice >= 0 ) && ( choice < GUIGetListSize( wnd, id ) ) ) {
        GUIToComboList( wnd, id, LB_DELETESTRING, CB_DELETESTRING, (WPI_PARAM1)choice, (WPI_PARAM2)0, (WPI_MRESULT)0 );
        return( true );
    }
    return( false );
}

/*
 * GUIClearList -- clear the contents of the list/combo box
 */

bool GUIClearList( gui_window *wnd, gui_ctl_id id )
{
    GUIToComboList( wnd, id, LB_RESETCONTENT, CB_RESETCONTENT, (WPI_PARAM1)0, (WPI_PARAM2)0, (WPI_MRESULT)0 );
    return( true );
}

/*
 * GUISetText - Set the text of the given control to the given text
 */

bool GUISetText( gui_window *wnd, gui_ctl_id id, const char *text )
{
    char                *new_text;
    gui_control_class   control_class;

    if( !GUIGetControlClass( wnd, id, &control_class ) ) {
        return( false );
    }
    if( control_class != GUI_EDIT ) {
        new_text = _wpi_menutext2pm( text );
        _wpi_setdlgitemtext( wnd->hwnd, id, new_text );
        if( new_text ) {
            _wpi_freemenutext( new_text );
        }
    } else {
        _wpi_setdlgitemtext( wnd->hwnd, id, text );
    }
    return( true );
}

/*
 * GUIGetText -- get the text of the given control
 */

char *GUIGetText( gui_window *wnd, gui_ctl_id id )
{
    LONG                length;
    char                *text;
    gui_control_class   control_class;
    HWND                hwnd;
    gui_ctl_idx         choice;

    if( !GUIGetControlClass( wnd, id, &control_class ) ) {
        return( NULL );
    }
    switch( control_class ) {
    case GUI_LISTBOX :
        if( !GUIGetCurrSelect( wnd, id, &choice ) ) {
            return( NULL );
        }
        return( GUIGetListItem( wnd, id, choice ) );
    default :
        hwnd = _wpi_getdlgitem( wnd->hwnd, id );
        if( hwnd == NULLHANDLE ) {
            return( NULL );
        }
        length = _wpi_getwindowtextlength( hwnd );
        if( length > 0 ) {
            text = (char *)GUIMemAlloc( length + 1 );
            if( text != NULL ) {
                _wpi_getwindowtext( hwnd, (LPSTR)text, length + 1 );
                switch( control_class ) {
                case GUI_PUSH_BUTTON:
                case GUI_DEFPUSH_BUTTON:
                case GUI_RADIO_BUTTON:
                case GUI_CHECK_BOX:
                case GUI_STATIC:
                case GUI_GROUPBOX:
                    _wpi_menutext2win( text );
                    break;
                }
            }
            return( text );
        }
        return( NULL );
    }
}

/*
 *  GUIGetCurrSelect -- return the index of the currently selected item
 *                      in the given list/combo box
 */

bool GUIGetCurrSelect( gui_window *wnd, gui_ctl_id id, gui_ctl_idx *choice )
{
    gui_ctl_idx sel;

    sel = (gui_ctl_idx)GUIToComboList( wnd, id, LB_GETCURSEL, CB_GETCURSEL, (WPI_PARAM1)0, (WPI_PARAM2)0, (WPI_MRESULT)-1 );
    if( ( sel == LB_ERR ) || ( sel == CB_ERR ) ) {
        return( false );
    }
    *choice = sel;
    return( true );
}

/*
 *  GUISetCurrSelect -- if the combo/list box contains enough items, set the
 *                      current selection to the indicated choice
 */

bool GUISetCurrSelect( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice )
{
#ifndef __OS2_PM__
    GUIToComboList( wnd, id, LB_SETCURSEL, CB_SETCURSEL, (WPI_PARAM1)choice, (WPI_PARAM2)false, (WPI_MRESULT)-1 );
#else
    GUIToComboList( wnd, id, LB_SETCURSEL, CB_SETCURSEL, (WPI_PARAM1)choice, (WPI_PARAM2)true, (WPI_MRESULT)-1 );
#endif
    return( true );
}

char *GUIGetListItem( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice )
{
    int         length;
    char        *text;

    length = (int)GUIToComboList( wnd, id, LB_GETTEXTLEN, CB_GETLBTEXTLEN, (WPI_PARAM1)choice, (WPI_PARAM2)0, (WPI_MRESULT)0 );
    if( length > 0 ) {
        text = (char *)GUIMemAlloc( length + 1 );
        if( text != NULL ) {
#ifdef __OS2_PM__
            GUIToComboList( wnd, id, LB_GETTEXT , CB_GETLBTEXT, MPFROM2SHORT( choice, length + 1 ), (WPI_PARAM2)(LPSTR)text, (WPI_MRESULT)0 );
#else
            GUIToComboList( wnd, id, LB_GETTEXT , CB_GETLBTEXT, (WPI_PARAM1)choice, (WPI_PARAM2)(LPSTR)text, (WPI_MRESULT)0 );
#endif
        }
    } else {
        text = NULL;
    }
    return( text );
}

bool GUISetListItemData( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice, void *data )
{
    WPI_MRESULT ret;

    ret = GUIToComboList( wnd, id, LB_SETITEMDATA, CB_SETITEMDATA, (WPI_PARAM1)choice, (WPI_PARAM2)data, (WPI_MRESULT)0 );
#ifndef __OS2_PM__
    if( ( ret == LB_ERR ) || ( ret == CB_ERR ) ) {
        return( false );
    }
    return( true );
#else
    return( ret == 0 );
#endif
}

void *GUIGetListItemData( gui_window *wnd, gui_ctl_id id, gui_ctl_idx choice )
{
    return( (void *)GUIToComboList( wnd, id, LB_GETITEMDATA, CB_GETITEMDATA, (WPI_PARAM1)choice, (WPI_PARAM2)0, (WPI_MRESULT)0 ) );
}
