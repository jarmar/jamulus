/******************************************************************************\
 * Copyright (c) 2004-2013
 *
 * Author(s):
 *  Volker Fischer
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
\******************************************************************************/

#include <QCloseEvent>
#include <QLabel>
#include <QListView>
#include <QTimer>
#include <QPixmap>
#include <QThread>
#include <QSlider>
#include <QMenuBar>
#include <QLayout>
#include <QSystemTrayIcon>
#include <QSettings>
#include "global.h"
#include "server.h"
#include "settings.h"
#include "multicolorled.h"
#include "ui_llconserverdlgbase.h"


/* Definitions ****************************************************************/
// update time for GUI controls
#define GUI_CONTRL_UPDATE_TIME      1000 // ms


/* Classes ********************************************************************/
class CLlconServerDlg : public QDialog, private Ui_CLlconServerDlgBase
{
    Q_OBJECT

public:
    CLlconServerDlg ( CServer*        pNServP,
                      CSettings*      pNSetP,
                      const bool      bStartMinimized,
                      QWidget*        parent = 0,
                      Qt::WindowFlags f = 0 );

protected:
    virtual void customEvent ( QEvent* pEvent );
    virtual void changeEvent ( QEvent* pEvent );
    virtual void closeEvent  ( QCloseEvent* Event );

    void         UpdateGUIDependencies();
    void         UpdateSystemTrayIcon ( const bool bIsActive );
    void         ShowWindowInForeground() { showNormal(); raise(); }
    void         ModifyAutoStartEntry ( const bool bDoAutoStart );

    QTimer                        Timer;
    CServer*                      pServer;
    CSettings*                    pSettings;

    CVector<CServerListViewItem*> vecpListViewItems;
    QMutex                        ListViewMutex;

    QMenuBar*                     pMenu;

    bool                          bSystemTrayIconAvaialbe;
    QSystemTrayIcon               SystemTrayIcon;
    QPixmap                       BitmapSystemTrayInactive;
    QPixmap                       BitmapSystemTrayActive;
    QMenu*                        pSystemTrayIconMenu;

public slots:
    void OnAboutToQuit() { pSettings->Save(); }

    void OnRegisterServerStateChanged ( int value );
    void OnDefaultCentralServerStateChanged ( int value );
    void OnStartOnOSStartStateChanged ( int value );
    void OnCentralServerAddressEditingFinished();
    void OnServerNameTextChanged ( const QString& strNewName );
    void OnLocationCityTextChanged ( const QString& strNewCity );
    void OnLocationCountryActivated ( int iCntryListItem );
    void OnTimer();
    void OnServerStarted() { UpdateSystemTrayIcon ( true ); }
    void OnServerStopped() { UpdateSystemTrayIcon ( false ); }
    void OnSysTrayMenuOpen() { ShowWindowInForeground(); }
    void OnSysTrayMenuHide() { hide(); }
    void OnSysTrayMenuExit() { close(); }
    void OnSysTrayActivated ( QSystemTrayIcon::ActivationReason ActReason );
};