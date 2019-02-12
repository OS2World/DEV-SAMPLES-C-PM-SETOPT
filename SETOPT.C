/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Author: Larry B. Finkelstein                              *
 *   (C) Copyright 1991.                                       *
 *   Creative Systems Programming Corporation.                 *
 *   All Rights Reserved.                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                             *
 * This program provides an example of how to set the color    *
 * and font for a Multi-Line Entry (MLE) control.              *
 *                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#define EXTERN
#include "SETOPT.h"

#define DOSASSERT(x) x;

SHORT cdecl main(argc, argv)
int argc;
char *argv[];
{
 QMSG qmsg;  /* MSG structure to store your messages                       */
 PID  pid;   /* Process identifier for adding name to switch list          */
 TID  tid;   /* Thread identifier                                          */

 /* The WinInitialize routine initializes the Presentation Manager         */
 /* facilities for use by this application and returns a handle to the     */
 /* anchor block assigned to the application by PM.                        */

 if((hAB = WinInitialize(0)) == 0)
    return(FALSE);

 /* The WinCreateMsgQueue call creates a message queue for this application*/

 if((hMQ = WinCreateMsgQueue(hAB, 0)) == 0)
    return(FALSE);

 /* The following function registers the classes of all application windows*/
 if(!cwRegisterClass())
    return(FALSE);

 /* The CreateWindow function creates a frame window for this application's*/
 /* top window, and set the window's size and location as appropriate.     */

 hWndFrame = cwCreateWindow((HWND)HWND_DESKTOP,
          FCF_TITLEBAR     |
          FCF_SYSMENU      |
          FCF_MINBUTTON    |
          FCF_MAXBUTTON    |
          FCF_SIZEBORDER   |
          FCF_MENU         |
          FCF_SHELLPOSITION,
          szAppName,
          "",
          ID_SETOPT,
          0, 0,
          0, 0,
          &hWndClient,
          0L, SWP_SHOW);

 if(hWndFrame == 0)
    return(FALSE);

 /* The following inline routine fills out the application's switch control*/
 /* structure with the appropriate information to add the application's    */
 /* name to the OS/2 Task Manager List, a list of the jobs currently       */
 /* running on the computer.                                               */

 WinQueryWindowProcess(hWndFrame, &pid, &tid);
 Swctl.hwnd = hWndFrame;                         /* Frame window handle    */
 Swctl.idProcess = pid;                          /* Process identifier     */
 Swctl.uchVisibility = SWL_VISIBLE;              /* visibility             */
 Swctl.fbJump = SWL_JUMPABLE;                    /* Jump indicator         */
 strcpy(Swctl.szSwtitle, "SETOPT.EXE");          /* Program name           */
 hSwitch = WinAddSwitchEntry(&Swctl);

 /* The following is the message loop for the application.                 */

 while(WinGetMsg(hAB, (PQMSG)&qmsg, 0, 0, 0))
       WinDispatchMsg(hAB,(PQMSG)&qmsg);

 /* Perform clean up before exiting application.                           */
 /* The following routine destroys the application's frame window (which   */
 /* also destroys its child windows), destroys its message queue, and      */
 /* disassociates the application from the Presentation Manager system.    */

 WinDestroyWindow(hWndFrame); /* Destroy the frame window                  */
 WinDestroyMsgQueue(hMQ);     /* Destroy this application's message queue  */
 WinTerminate(hAB);           /* Terminate this application's use of the   */
                              /* Presentation Manager resources            */
} /* end of main */

/***************************************************************************/
/*                                                                         */
/* Main Window Procedure                                                   */
/*                                                                         */
/* This procedure provides service routines for the general PM events      */
/* (messages) that PM sends to the window, as well as the user             */
/* initiated events (messages) that are generated when the user selects    */
/* the action bar and pulldown menu controls or the corresponding          */
/* keyboard accelerators.                                                  */
/*                                                                         */
/***************************************************************************/

MRESULT EXPENTRY WndProc(hWnd, message, mp1, mp2)
HWND hWnd;
USHORT message;
MPARAM mp1;
MPARAM mp2;
{                        /* Beginning of message processor                 */
   HPS hPS;              /* Handle for the Presentation Space              */
   RECTL rClient;        /* Handle to rectangle formed by client area      */
   USHORT rc;            /* common return code value                       */

   switch(message)
     {
      case WM_COMMAND:
        /* The PM messages for action bar and pulldown menu items are      */
        /* processed in this routine.                                      */

           switch(SHORT1FROMMP(mp1))
            {
             case IDM_O_SETFONT:
                  /* This makes a call to the dialog box named             */
                  /* "SETFONT" rc will receive the return code             */
                  /* sent when the dialog box is closed                    */
                  rc = WinDlgBox(HWND_DESKTOP, hWnd, (PFNWP)SETFONTMsgProc,
                                 0, IDLG_SETFONT, (PBYTE)hWnd);
                  break;

             case IDM_O_SETCOLOR:
                  /* This makes a call to the dialog box named             */
                  /* "SETCOLOR" rc will receive the return code            */
                  /* sent when the dialog box is closed                    */
                  rc = WinDlgBox(HWND_DESKTOP, hWnd, (PFNWP)SETCOLORMsgProc,
                                 0, IDLG_SETCOLOR, (PBYTE)hWnd);
                  break;



             default:
                  break; /* End of default case for switch(mp1) */
            }
           break; /* End of WM_COMMAND */


      case WM_CREATE:
           break; /* End of WM_CREATE */

      case WM_MOUSEMOVE:
           return(WinDefWindowProc(hWnd, message, mp1, mp2));
           break;

      case WM_SIZE:     /* code for sizing client area                     */
           break;       /* End of WM_SIZE                                  */

      case WM_PAINT:    /* code for the window's client area               */
           /* Obtain a handle to a cache presentation space                */
           hPS = WinBeginPaint(hWnd, 0, 0);

           /* Determine the size of the client area                        */
           WinQueryWindowRect(hWnd, &rClient);

           /* Fill the background with the default background color        */
           WinFillRect(hPS, &rClient, CLR_BACKGROUND);

           /* return presentation space to state before WinBeginPaint      */
           WinEndPaint(hPS);
           break; /* End of WM_PAINT */


      case WM_CLOSE:  /* close the window                                  */
//           if(hWnd != hWndClient)
//             break;
           return(WinDefWindowProc(hWnd, message, mp1, mp2));
           break; /* End of WM_CLOSE */

      default:
           /* For any message for which you don't specifically provide a   */
           /* service routine, you should return the message to PM using   */
           /* the WinDefWindowProc function.                               */
           return(WinDefWindowProc(hWnd, message, mp1, mp2));
           break;  /* End of default */
     }
   return(0L);
} /* End of WndProc */

/***************************************************************************/
/*                                                                         */
/* Dialog Window Procedure                                                 */
/*                                                                         */
/* This procedure is associated with the dialog box that is included in    */
/* the function name of the procedure. It provides the service routines    */
/* for the events (messages) that occur because the end user operates      */
/* one of the dialog box's buttons, entry fields, or controls.             */
/*                                                                         */
/***************************************************************************/

MRESULT EXPENTRY SETFONTMsgProc(hWndDlg, message, mp1, mp2)
HWND hWndDlg;
USHORT message;
MPARAM mp1;
MPARAM mp2;
{
 static SHORT sfValidate = TRUE;
 SHORT  i, j ;
 static HWND  hWndParent;

   typedef struct _SETFONTS {
      LONG         lNumberFonts ;
      SHORT        sDfltSize ;
      SHORT        sDfltPoints ;
      SHORT        sDfltHeight ;
      FATTRS       fatDefault;
      PFONTMETRICS pFontMetrics ;
   } SETFONTS;
   typedef SETFONTS FAR *PSETFONTS ;

   HPS          hPS ;
   LONG         lNumberFonts, lNoFontsReq = 0L ;
   USHORT       usAllocSize, usMaxLB ;
   SHORT        sLBPos ;
   SEL          selFonts ;
   CHAR         szFont[257];
   HWND         hWnd ;
   CHAR         szDfltFont[FACESIZE];
   PFONTMETRICS pFontMetrics ;
   PSETFONTS    pSetFonts ;
   MRESULT      rc ;
   static SHORT sOutlineFonts[10] = { 6,8,10,12,14,18,24,30,36,48 } ;
   SHORT        sPointSize ;
   BOOL         fScanning ;
   BOOL         fSeries ;

   hWnd = hWndDlg ;
   pSetFonts = (PSETFONTS)WinQueryWindowULong( hWndDlg, QWL_USER ) ;
 switch(message)
   {
    case WM_INITDLG:
         hWndParent = (HWND)mp2;
         hPS = WinGetPS( hWndDlg ) ;
         lNumberFonts = GpiQueryFonts( hPS, QF_PUBLIC | QF_PRIVATE, NULL,
                                       &lNoFontsReq, 0L, (PFONTMETRICS)NULL ) ;
         usAllocSize = ((USHORT)sizeof(FONTMETRICS) * (USHORT)lNumberFonts)+sizeof(SETFONTS) ;
         DOSASSERT( DosAllocSeg( usAllocSize, &selFonts, 0 ) ) ;
         pSetFonts = MAKEP( selFonts, 0 ) ;
         WinSetWindowULong( hWndDlg, QWL_USER, (ULONG)pSetFonts ) ;

         pSetFonts->fatDefault.usRecordLength=sizeof(FATTRS) ;
         WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_QUERYFONT, &(pSetFonts->fatDefault), 0L ) ;
//         WinSendMsg( (HWND)0x1f677c20 , MLM_QUERYFONT, &(pSetFonts->fatDefault), 0L ) ;
         pSetFonts->pFontMetrics = MAKEP( selFonts, sizeof(SETFONTS) ) ;
         GpiQueryFonts( hPS, QF_PUBLIC | QF_PRIVATE, NULL, &lNumberFonts,
                        (LONG)sizeof(FONTMETRICS), pSetFonts->pFontMetrics ) ;
         pSetFonts->lNumberFonts = lNumberFonts ;
         for ( i=0 ; i<(USHORT)lNumberFonts ; i++ )
         {
            pFontMetrics = MAKEP( selFonts, (i*sizeof(FONTMETRICS))+sizeof(SETFONTS) ) ;
            strcpy( szFont, pFontMetrics->szFacename ) ;
            if ( !(pFontMetrics->fsDefn & FM_DEFN_OUTLINE) )
            {
               if ( strcmp( szFont, "Courier" ) == 0 ||
                    strcmp( szFont, "Helv"    ) == 0 ||
                    strcmp( szFont, "Tms Rmn" ) == 0 )
               {
                  strcat( szFont, " Bitmap" ) ;
//                  strcpy( pFontMetrics->szFacename, szFont ) ;
               } /* endif */
            } /* endif */
            if ( i==0 )
            {
               strcpy( szDfltFont, szFont ) ;
               pSetFonts->sDfltSize = pFontMetrics->sNominalPointSize ;
               pSetFonts->sDfltPoints = pFontMetrics->sNominalPointSize/10 ;
               pSetFonts->sDfltHeight = pFontMetrics->lMaxBaselineExt ;
            }
            else
            {
            } /* endif */
            printf ( "Font: %s,%d Type=%x,Defn=%x,Sel=%x,Height=%ld,Width=%ld,ptr=%p\n",
                      szFont,
                      pFontMetrics->sNominalPointSize/10,
                      pFontMetrics->fsType,
                      pFontMetrics->fsDefn,
                      pFontMetrics->fsSelection,
                      pFontMetrics->lMaxBaselineExt,
                      pFontMetrics->lAveCharWidth,
                      pFontMetrics ) ;
            sLBPos = SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                      LB_SETFONT_FONT,
                                                      LM_SEARCHSTRING,
                                                      MPFROM2SHORT( LSS_CASESENSITIVE, LIT_FIRST ),
                                                      (PSZ)szFont ) ) ;
            if ( sLBPos==LIT_NONE )
            {
               sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                       LB_SETFONT_FONT,
                                                       LM_INSERTITEM,
                                                       MPFROMSHORT( LIT_SORTASCENDING ),
                                                       szFont ) ) ;
               WinSendDlgItemMsg( hWndDlg,
                                  LB_SETFONT_FONT,
                                  LM_SETITEMHANDLE,
                                  MPFROMSHORT( sLBPos ),
                                  MPFROMP( pFontMetrics ) ) ;
               if ( stricmp( szFont, szDfltFont )==0 )
               {
                  WinSendDlgItemMsg( hWndDlg, LB_SETFONT_SIZE, LM_DELETEALL, 0L, 0L ) ;
                  WinSendDlgItemMsg( hWndDlg,
                                     LB_SETFONT_FONT,
                                     LM_SELECTITEM,
                                     MPFROMSHORT( sLBPos ),
                                     MPFROMSHORT( TRUE ) ) ;
               } /* endif */
            } /* endif */
         } /* endfor */
         sLBPos = SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                   LB_SETFONT_FONT,
                                                   LM_SEARCHSTRING,
                                                   MPFROM2SHORT( 0, LIT_FIRST ),
                                                   MPFROMP( szDfltFont ) ) ) ;
         if ( sLBPos != LIT_NONE )
         {
            WinSendDlgItemMsg( hWndDlg,
                               LB_SETFONT_FONT,
                               LM_SETTOPINDEX,
                               MPFROMSHORT( sLBPos ), 0L ) ;
         } /* endif */
         WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), FALSE ) ;
         WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), FALSE ) ;
         break; /* End of WM_INITDLG */

    case WM_CONTROL:
         switch(SHORT1FROMMP(mp1))
           {
            case CK_SETFONT_BOLD: /* Checkbox text: "~Bold"                */
                 {
                  USHORT checked;

                  /* Query check box control for current state             */
                  checked = !WinSendDlgItemMsg(hWndDlg, CK_SETFONT_BOLD,
                                               BM_QUERYCHECK, 0L, 0L );
                  if ( !checked )
                     pSetFonts->fatDefault.fsSelection |= FATTR_SEL_BOLD ;
                  else
                     pSetFonts->fatDefault.fsSelection &= 0xffff-FATTR_SEL_BOLD ;
                  WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETFONT, &(pSetFonts->fatDefault), 0L ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), TRUE ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), TRUE ) ;
                 }
                 break;

            case CK_SETFONT_ITALIC: /* Checkbox text: "~Italic"            */
                 {
                  USHORT checked;

                  /* Query check box control for current state             */
                  checked = !WinSendDlgItemMsg(hWndDlg, CK_SETFONT_ITALIC,
                                               BM_QUERYCHECK, 0L, 0L );
                  if ( !checked )
                     pSetFonts->fatDefault.fsSelection |= FATTR_SEL_ITALIC ;
                  else
                     pSetFonts->fatDefault.fsSelection &= 0xffff-FATTR_SEL_ITALIC ;
                  WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETFONT, &(pSetFonts->fatDefault), 0L ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), TRUE ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), TRUE ) ;
                 }
                 break;

            case CK_SETFONT_UNDERSCORE: /* Checkbox text: "~Underscore"    */
                 {
                  USHORT checked;

                  /* Query check box control for current state             */
                  checked = !WinSendDlgItemMsg(hWndDlg, CK_SETFONT_UNDERSCORE,
                                               BM_QUERYCHECK, 0L, 0L );
                  if ( !checked )
                     pSetFonts->fatDefault.fsSelection |= FATTR_SEL_UNDERSCORE ;
                  else
                     pSetFonts->fatDefault.fsSelection &= 0xffff-FATTR_SEL_UNDERSCORE ;
                  WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETFONT, &(pSetFonts->fatDefault), 0L ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), TRUE ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), TRUE ) ;
                 }
                 break;

            case CK_SETFONT_STRIKEOUT: /* Checkbox text: "Strike~out"      */
                 {
                  USHORT checked;

                  /* Query check box control for current state             */
                  checked = !WinSendDlgItemMsg(hWndDlg, CK_SETFONT_STRIKEOUT,
                                               BM_QUERYCHECK, 0L, 0L );
                  if ( !checked )
                     pSetFonts->fatDefault.fsSelection |= FATTR_SEL_STRIKEOUT ;
                  else
                     pSetFonts->fatDefault.fsSelection &= 0xffff-FATTR_SEL_STRIKEOUT ;
                  WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETFONT, &(pSetFonts->fatDefault), 0L ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), TRUE ) ;
                  WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), TRUE ) ;
                 }
                 break;

            case MLE_SETFONT: /* Entry field                               */
                 switch(SHORT2FROMMP(mp1)) /* switch on Notification Code  */
                   {
                    case EN_SETFOCUS:/* Entry field is receiving the focus */
                         break;

                    case EN_KILLFOCUS: /* Entry field is losing the focus  */
                         break;

                    default: /* Default other messages                     */
                         return(WinDefDlgProc(hWndDlg, message, mp1, mp2));
                         break;
                   }
                 break;

            case LB_SETFONT_FONT: /* List box                              */
                 {
                    if ( SHORT2FROMMP(mp1)==LN_SELECT )
                    {
                       if ( pSetFonts != NULL )
                       {
                          sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                                  LB_SETFONT_FONT,
                                                                  LM_QUERYSELECTION,
                                                                  MPFROMSHORT( LIT_FIRST ), 0L ) ) ;
                          if ( sLBPos != LIT_NONE )
                          {
                             WinSendDlgItemMsg( hWndDlg,
                                                LB_SETFONT_FONT,
                                                LM_QUERYITEMTEXT,
                                                MPFROM2SHORT( sLBPos, sizeof(szDfltFont) ),
                                                szDfltFont ) ;
//                             pFontMetrics = (PFONTMETRICS)
//                                WinSendDlgItemMsg( hWndDlg,
//                                                   LB_SETFONT_FONT,
//                                                   LM_QUERYITEMHANDLE,
//                                                   MPFROMSHORT( sLBPos ), 0L ) ;
                             WinSendDlgItemMsg( hWndDlg,
                                                LB_SETFONT_SIZE,
                                                LM_DELETEALL,
                                                0L, 0L ) ;
                             fScanning = fSeries = TRUE ;
                             for ( i=0 ; i<(USHORT)pSetFonts->lNumberFonts ; i++ )
                             {
                                pFontMetrics = MAKEP( SELECTOROF(pSetFonts), (i*sizeof(FONTMETRICS))+sizeof(SETFONTS) ) ;
                                strcpy( szFont, pFontMetrics->szFacename ) ;
                                if ( !(pFontMetrics->fsDefn & FM_DEFN_OUTLINE) )
                                {
                                   if ( strcmp( szFont, "Courier" ) == 0 ||
                                        strcmp( szFont, "Helv"    ) == 0 ||
                                        strcmp( szFont, "Tms Rmn" ) == 0 )
                                   {
                                      strcat( szFont, " Bitmap" ) ;
                                   } /* endif */
                                } /* endif */
                                if ( stricmp( szFont, szDfltFont )==0 )
                                {
                                   for ( j= 0 ; j<10 ; j++ )
                                   {
                                      if ( pFontMetrics->fsDefn & FM_DEFN_OUTLINE )
                                      {
                                         sprintf ( szFont, "%d", sOutlineFonts[j] ) ;
                                         sPointSize = sOutlineFonts[j]*10 ;
                                      }
                                      else
                                      {
                                         sprintf ( szFont, "%d", pFontMetrics->sNominalPointSize/10 ) ;
                                         sPointSize = pFontMetrics->sNominalPointSize ;
                                         j = 10 ;
                                         if ( pSetFonts->sDfltPoints==(sPointSize/10) &&
                                              pSetFonts->sDfltHeight==pFontMetrics->lMaxBaselineExt )
                                         {
                                            fScanning = FALSE ;
                                         } /* endif */
                                      } /* endif */
                                      sLBPos = SHORT1FROMMR(
                                         WinSendDlgItemMsg( hWndDlg,
                                                             LB_SETFONT_SIZE,
                                                             LM_SEARCHSTRING,
                                                             MPFROM2SHORT( LSS_CASESENSITIVE, LIT_FIRST ),
                                                             (PSZ)szFont ) ) ;
                                      if ( sLBPos==LIT_NONE )
                                      {
                                         sLBPos=SHORT1FROMMR(
                                                 WinSendDlgItemMsg( hWndDlg,
                                                                    LB_SETFONT_SIZE,
                                                                    LM_INSERTITEM,
                                                                    MPFROMSHORT( LIT_END ),
                                                                    szFont ) ) ;
                                      } /* endif */
                                      if ( !fScanning )
                                      {
                                         if ( (pSetFonts->sDfltPoints<(sPointSize/10)) && fSeries )
                                         {
                                            fSeries = FALSE ;
                                         } /* endif */
                                      } /* endif */
                                      if ( fScanning || fSeries )
                                      {
                                         WinSendDlgItemMsg( hWndDlg,
                                                            LB_SETFONT_SIZE,
                                                            LM_SETITEMHANDLE,
                                                            MPFROMSHORT( sLBPos ),
                                                            MPFROMP( pFontMetrics ) ) ;
                                         printf ( "Size: %s,%d Type=%x,Defn=%x,Sel=%x,Height=%ld,Width=%ld,ptr=%p,scan=%d\n",
                                                   szDfltFont,
                                                   pFontMetrics->sNominalPointSize/10,
                                                   pFontMetrics->fsType,
                                                   pFontMetrics->fsDefn,
                                                   pFontMetrics->fsSelection,
                                                   pFontMetrics->lMaxBaselineExt,
                                                   pFontMetrics->lAveCharWidth,
                                                   pFontMetrics, fScanning ) ;
                                      }
                                      else
                                      {
                                      } /* endif */
                                   } /* endfor */
                                } /* endif */
                             } /* endfor */
                             sprintf ( szFont, "%d", pSetFonts->sDfltSize/10 ) ;
                             sLBPos = SHORT1FROMMR(
                                WinSendDlgItemMsg( hWndDlg,
                                                    LB_SETFONT_SIZE,
                                                    LM_SEARCHSTRING,
                                                    MPFROM2SHORT( LSS_CASESENSITIVE, LIT_FIRST ),
                                                    (PSZ)szFont ) ) ;
                             if ( sLBPos == LIT_NONE )
                             {
                                WinSendDlgItemMsg( hWndDlg,
                                                   LB_SETFONT_SIZE,
                                                   LM_QUERYITEMTEXT,
                                                   MPFROM2SHORT( 0 , sizeof(szFont) ),
                                                   szFont ) ;
                                if (pSetFonts->sDfltSize/10 < atoi(szFont) )
                                {
                                   sLBPos = 0 ;
                                }
                                else
                                {
                                   usMaxLB = SHORT1FROMMR(
                                      WinSendDlgItemMsg( hWndDlg,
                                                          LB_SETFONT_SIZE,
                                                          LM_QUERYITEMCOUNT,
                                                          0L, 0L ) ) ;
                                   WinSendDlgItemMsg( hWndDlg,
                                                      LB_SETFONT_SIZE,
                                                      LM_QUERYITEMTEXT,
                                                      MPFROM2SHORT( usMaxLB, sizeof(szFont) ),
                                                      szFont ) ;
                                   if (pSetFonts->sDfltSize/10 < atoi(szFont) )
                                   {
                                      sLBPos = usMaxLB ;
                                   } /* endif */
                                } /* endif */
                             } /* endif */
                             if ( sLBPos != LIT_NONE )
                             {
                                WinSendDlgItemMsg( hWndDlg,
                                                   LB_SETFONT_SIZE,
                                                   LM_SELECTITEM,
                                                   MPFROMSHORT( sLBPos ),
                                                   MPFROMSHORT( TRUE ) ) ;
                                WinSendDlgItemMsg( hWndDlg,
                                                   LB_SETFONT_SIZE,
                                                   LM_SETTOPINDEX,
                                                   MPFROMSHORT( sLBPos ), 0L ) ;
                             } /* endif */
                          } /* endif */
                       } /* endif */
                       WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETFONT, &(pSetFonts->fatDefault), 0L ) ;
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), TRUE ) ;
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), TRUE ) ;
                    } /* endif */
                 }
                 break;

            case LB_SETFONT_SIZE: /* List box                              */
                 {
                    if ( SHORT2FROMMP(mp1)==LN_SELECT )
                    {
                       sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                                LB_SETFONT_SIZE,
                                                                LM_QUERYSELECTION,
                                                                MPFROMSHORT( LIT_FIRST ), 0L ) ) ;
                       if ( sLBPos != LIT_NONE )
                       {
                          WinSendDlgItemMsg( hWndDlg,
                                             LB_SETFONT_SIZE,
                                             LM_QUERYITEMTEXT,
                                             MPFROM2SHORT( sLBPos, sizeof(szFont) ),
                                             MPFROMP( szFont ) ) ;
                          pFontMetrics=(PFONTMETRICS)
                                       WinSendDlgItemMsg( hWndDlg,
                                                          LB_SETFONT_SIZE,
                                                          LM_QUERYITEMHANDLE,
                                                          MPFROMSHORT( sLBPos ), 0L ) ;
                          strcpy( pSetFonts->fatDefault.szFacename, pFontMetrics->szFacename ) ;
//                          pSetFonts->fatDefault.fsSelection = pFontMetrics->fsSelection ;
//                          pSetFonts->fatDefault.lMatch = pFontMetrics->lMatch ;
                          pSetFonts->fatDefault.lMatch = 0L ;
                          pSetFonts->fatDefault.idRegistry = pFontMetrics->idRegistry ;
                          pSetFonts->fatDefault.usCodePage = pFontMetrics->usCodePage ;
                          if ( pFontMetrics->fsDefn & FM_DEFN_OUTLINE )
                          {
                             pSetFonts->fatDefault.lMaxBaselineExt = (LONG)sOutlineFonts[sLBPos] *
                                                                     pFontMetrics->lMaxBaselineExt /
                                                                     ( pFontMetrics->sNominalPointSize/10 ) ;
                             pSetFonts->fatDefault.lAveCharWidth = pFontMetrics->lAveCharWidth ;
                             pSetFonts->sDfltSize = sOutlineFonts[sLBPos]*10 ;
                             pSetFonts->fatDefault.fsFontUse = FATTR_FONTUSE_OUTLINE |
                                                               FATTR_FONTUSE_TRANSFORMABLE ;
                          }
                          else
                          {
                             pSetFonts->fatDefault.lMaxBaselineExt = pFontMetrics->lMaxBaselineExt ;
                             pSetFonts->fatDefault.lAveCharWidth = pFontMetrics->lAveCharWidth ;
                             pSetFonts->sDfltSize = pFontMetrics->sNominalPointSize ;
                             pSetFonts->fatDefault.fsFontUse = 0 ;
                          } /* endif */
                          pSetFonts->fatDefault.fsType = 0 /* pFontMetrics->fsType */ ;
//                                                            FATTR_FONTUSE_NOMIX ;
                       } /* endif */
                       rc=WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETFONT, &(pSetFonts->fatDefault), 0L ) ;
                       printf ( "SETF: %s,%s pFM=%p Baseline=%ld,%ld AveWidth=%ld,%ld,ptr=%p   RC=%lX\n",
                                pFontMetrics->szFacename,
                                szFont,
                                pFontMetrics,
                                pFontMetrics->lMaxBaselineExt,
                                pSetFonts->fatDefault.lMaxBaselineExt,
                                pFontMetrics->lAveCharWidth,
                                pSetFonts->fatDefault.lAveCharWidth, pFontMetrics, rc ) ;
                       if ( pFontMetrics->fsDefn & FM_DEFN_OUTLINE )
                       {
                          rc=WinSendDlgItemMsg( hWndDlg, MLE_SETFONT, MLM_SETTABSTOP, MPFROMSHORT(8*sOutlineFonts[sLBPos]), 0L ) ;
//                          printf( "Sett ... rc=%lX\n", rc ) ;
                       } /* endif */
                       if ( !rc )
                       {
                          WinAlarm( HWND_DESKTOP, WA_ERROR ) ;
                          WinMessageBox( HWND_DESKTOP, hWndDlg, "Unable to set new font",
                                         "ReadBBS - Set Font", 0, MB_OK | MB_ICONHAND ) ;
                       } /* endif */
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_OK ), TRUE ) ;
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETFONT_APPLY ), TRUE ) ;
                    } /* endif */
                 }
                 break;

           }
         break; /* End of WM_CONTROL */

    case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
           {
            case PB_SETFONT_OK:
            case DID_OK: /* Button text: "OK"                              */
                 DosFreeSeg( SELECTOROF(pSetFonts) ) ;
                 WinDismissDlg(hWndDlg, TRUE);
                 break;

            case PB_SETFONT_APPLY: /* Button text: "Apply"                 */
                 break;

            case PB_SETFONT_CANCEL:
            case DID_CANCEL: /* Button text: "Cancel"                      */
                 /* Ignore data values entered into the dialog controls    */
                 /* and dismiss the dialog window                          */
                 DosFreeSeg( SELECTOROF(pSetFonts) ) ;
                 WinDismissDlg(hWndDlg, FALSE);
                 break;

            case PB_SETFONT_HELP: /* Button text: "Help"                   */
                 break;

           }
         break; /* End of WM_COMMAND */

    case WM_CLOSE:
         WinPostMsg(hWndDlg, WM_COMMAND, MPFROMSHORT(DID_CANCEL), 0L);
         break; /* End of WM_CLOSE */

    case WM_FAILEDVALIDATE:
         WinAlarm(HWND_DESKTOP, WA_ERROR);
         sfValidate = FALSE;
         WinSetFocus(HWND_DESKTOP, WinWindowFromID(hWndDlg, SHORT1FROMMP(mp1)));
         sfValidate = TRUE;
         return((MRESULT)TRUE);
         break; /* End of WM_FAILEDVALIDATE */

    default:
         return(WinDefDlgProc(hWndDlg, message, mp1, mp2));
         break;
   }
 return FALSE;
} /* End of SETFONTMsgProc */


/***************************************************************************/
/*                                                                         */
/* Dialog Window Procedure                                                 */
/*                                                                         */
/* This procedure is associated with the dialog box that is included in    */
/* the function name of the procedure. It provides the service routines    */
/* for the events (messages) that occur because the end user operates      */
/* one of the dialog box's buttons, entry fields, or controls.             */
/*                                                                         */
/***************************************************************************/

MRESULT EXPENTRY SETCOLORMsgProc(hWndDlg, message, mp1, mp2)
HWND hWndDlg;
USHORT message;
MPARAM mp1;
MPARAM mp2;
{
 static SHORT sfValidate = TRUE;
 INT    i;
 static HWND  hWndParent;
   SHORT sLBPos ;
   LONG lBackColor, lTextColor ;
   IPT  iptStart, iptStop ;
   ULONG ulImport ;
   CHAR szText[] = "\tThis is an example of text.\r\nThis is an example of selected text." ;
   CHAR szColor[16][16] = { "White","Blue ","Red ","Pink ","Green ","Cyan ",
                            "Yellow ", "Black",
                            "Dark Grey ","Dark Blue ","Dark Red ","Dark Pink ",
                            "Dark Green ",
                            "Dark Cyan ","Brown ","Pale Grey " } ;

 switch(message)
   {
    case WM_INITDLG:
         hWndParent = (HWND)mp2;
         lBackColor=WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_QUERYBACKCOLOR, 0L, 0L ) ;
         lTextColor=WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_QUERYTEXTCOLOR, 0L, 0L ) ;
         for ( i=CLR_BACKGROUND ; i<=CLR_PALEGRAY ; i++ )
         {
            sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg, LB_SETCOLOR_FOREGROUND,
                                                    LM_INSERTITEM,
                                                    MPFROMSHORT( LIT_SORTASCENDING ),
                                                    szColor[i] ) ) ;
            WinSendDlgItemMsg( hWndDlg, LB_SETCOLOR_FOREGROUND, LM_SETITEMHANDLE,
                               MPFROMSHORT( sLBPos ), MPFROMSHORT( i ) ) ;
            if (i==lTextColor)
            {
               WinSendDlgItemMsg( hWndDlg, LB_SETCOLOR_FOREGROUND,
                                  LM_SELECTITEM, MPFROMSHORT( sLBPos ), MPFROMSHORT( TRUE ) ) ;
            } /* endif */
            sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg, LB_SETCOLOR_BACKGROUND,
                                                    LM_INSERTITEM,
                                                    MPFROMSHORT( LIT_SORTASCENDING ),
                                                    szColor[i] ) ) ;
            WinSendDlgItemMsg( hWndDlg, LB_SETCOLOR_BACKGROUND, LM_SETITEMHANDLE,
                               MPFROMSHORT( sLBPos ), MPFROMSHORT( i ) ) ;
            if (i==lBackColor)
            {
               WinSendDlgItemMsg( hWndDlg, LB_SETCOLOR_BACKGROUND,
                                  LM_SELECTITEM, MPFROMSHORT( sLBPos ), MPFROMSHORT( TRUE ) ) ;
            } /* endif */
         } /* endfor */
         WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_FORMAT, MLFIE_CFTEXT, 0L ) ;
         ulImport = (ULONG)sizeof(szText) ;
         WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_SETIMPORTEXPORT, MPFROMP(szText), &ulImport ) ;
         iptStart = (IPT)-1L ;
         WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_IMPORT, &iptStart, MPFROMSHORT(sizeof(szText)) ) ;
         WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_SETSEL, (IPT)29L, (IPT)MPFROMSHORT(sizeof(szText)) ) ;
         WinSetFocus( HWND_DESKTOP, WinWindowFromID( hWndDlg, MLE_SETCOLOR ) ) ;
         WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETCOLOR_OK ), FALSE ) ;
         WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETCOLOR_APPLY ), FALSE ) ;
         break; /* End of WM_INITDLG */

    case WM_CONTROL:
         switch(SHORT1FROMMP(mp1))
           {
            case MLE_SETCOLOR: /* Entry field                              */
                 switch(SHORT2FROMMP(mp1)) /* switch on Notification Code  */
                   {
                    case EN_SETFOCUS:/* Entry field is receiving the focus */
                         break;

                    case EN_KILLFOCUS: /* Entry field is losing the focus  */
                         break;

                    default: /* Default other messages                     */
                         return(WinDefDlgProc(hWndDlg, message, mp1, mp2));
                         break;
                   }
                 break;

            case LB_SETCOLOR_FOREGROUND: /* List box                       */
                 {
                    if ( SHORT2FROMMR(mp1)==LN_SELECT )
                    {
                       sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                               LB_SETCOLOR_FOREGROUND,
                                                               LM_QUERYSELECTION,
                                                               MPFROMSHORT( LIT_FIRST ), 0L ) ) ;
                       if ( sLBPos != LIT_NONE )
                       {
                          lTextColor=LONGFROMMR( WinSendDlgItemMsg( hWndDlg,
                                                                    LB_SETCOLOR_FOREGROUND,
                                                                    LM_QUERYITEMHANDLE,
                                                                    MPFROMSHORT( sLBPos ), 0L ) ) ;
                          WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_SETTEXTCOLOR,
                                             MPFROMLONG( lTextColor ), 0L ) ;
                       } /* endif */
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETCOLOR_OK ), TRUE ) ;
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETCOLOR_APPLY ), TRUE ) ;
                       WinSetFocus( HWND_DESKTOP, WinWindowFromID( hWndDlg, MLE_SETCOLOR ) ) ;
                    } /* endif */
                 }
                 break;

            case LB_SETCOLOR_BACKGROUND: /* List box                      */
                 {
                    if ( SHORT2FROMMR(mp1)==LN_SELECT )
                    {
                       sLBPos=SHORT1FROMMR( WinSendDlgItemMsg( hWndDlg,
                                                               LB_SETCOLOR_BACKGROUND,
                                                               LM_QUERYSELECTION,
                                                               MPFROMSHORT( LIT_FIRST ), 0L ) ) ;
                       if ( sLBPos != LIT_NONE )
                       {
                          lBackColor=LONGFROMMR( WinSendDlgItemMsg( hWndDlg,
                                                                    LB_SETCOLOR_BACKGROUND,
                                                                    LM_QUERYITEMHANDLE,
                                                                    MPFROMSHORT( sLBPos ), 0L ) ) ;
                          WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_SETBACKCOLOR,
                                             MPFROMLONG( lBackColor ), 0L ) ;
                       } /* endif */
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETCOLOR_OK ), TRUE ) ;
                       WinEnableWindow( WinWindowFromID( hWndDlg, PB_SETCOLOR_APPLY ), TRUE ) ;
                       WinSetFocus( HWND_DESKTOP, WinWindowFromID( hWndDlg, MLE_SETCOLOR ) ) ;
                    } /* endif */
                 }
                 break;

           }
         break; /* End of WM_CONTROL */

    case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
           {
            case PB_SETCOLOR_OK:
            case DID_OK: /* Button text: "OK"                              */
                 lBackColor=WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_QUERYBACKCOLOR, 0L, 0L ) ;
                 lTextColor=WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_QUERYTEXTCOLOR, 0L, 0L ) ;
//                 WinSendMsg( pMleData->hWndReadMLE, MLM_SETTEXTCOLOR, MPFROMLONG( lTextColor ), 0L ) ;
//                 WinSendMsg( pMleData->hWndReadMLE, MLM_SETBACKCOLOR, MPFROMLONG( lBackColor ), 0L ) ;
                 WinDismissDlg(hWndDlg, TRUE);
                 break;

            case PB_SETCOLOR_APPLY: /* Button text: "Apply"                */
                 lBackColor=WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_QUERYBACKCOLOR, 0L, 0L ) ;
                 lTextColor=WinSendDlgItemMsg( hWndDlg, MLE_SETCOLOR, MLM_QUERYTEXTCOLOR, 0L, 0L ) ;
//                 WinSendMsg( pMleData->hWndReadMLE, MLM_SETTEXTCOLOR, MPFROMLONG( lTextColor ), 0L ) ;
//                 WinSendMsg( pMleData->hWndReadMLE, MLM_SETBACKCOLOR, MPFROMLONG( lBackColor ), 0L ) ;
                 WinSetFocus( HWND_DESKTOP, WinWindowFromID( hWndDlg, MLE_SETCOLOR ) ) ;
                 break;

            case PB_SETCOLOR_CANCEL:
            case DID_CANCEL: /* Button text: "Cancel"                      */
                 /* Ignore data values entered into the dialog controls    */
                 /* and dismiss the dialog window                          */
                 WinDismissDlg(hWndDlg, FALSE);
                 break;

            case PB_SETCOLOR_HELP: /* Button text: "Help"                  */
                 WinSetFocus( HWND_DESKTOP, WinWindowFromID( hWndDlg, MLE_SETCOLOR ) ) ;
                 break;

           }
         break; /* End of WM_COMMAND */

    case WM_CLOSE:
         WinPostMsg(hWndDlg, WM_COMMAND, MPFROMSHORT(DID_CANCEL), 0L);
         break; /* End of WM_CLOSE */

    case WM_FAILEDVALIDATE:
         WinAlarm(HWND_DESKTOP, WA_ERROR);
         sfValidate = FALSE;
         WinSetFocus(HWND_DESKTOP, WinWindowFromID(hWndDlg, SHORT1FROMMP(mp1)));
         sfValidate = TRUE;
         return((MRESULT)TRUE);
         break; /* End of WM_FAILEDVALIDATE */

    default:
         return(WinDefDlgProc(hWndDlg, message, mp1, mp2));
         break;
   }
 return FALSE;
} /* End of SETCOLORMsgProc */




/***************************************************************************/
/*                                                                         */
/* cwRegisterClass Function                                                */
/*                                                                         */
/* The following function registers all the classes of all the windows     */
/* associated with this application. The function returns TRUE if it is    */
/* successful, otherwise it returns FALSE.                                 */
/*                                                                         */
/***************************************************************************/

INT cwRegisterClass()
{
 INT rc;

 strcpy(szAppName, "SETOPT");
 rc = WinRegisterClass(hAB,             /* Anchor block handle             */
                      (PCH)szAppName,   /* Name of class being registered  */
                      (PFNWP)WndProc,   /* Window procedure for class      */
                      CS_SIZEREDRAW ,
                      0);            /* Extra bytes to reserve             */
 if (rc == FALSE)
   return(FALSE);

 return(TRUE);
} /* End of cwRegisterClass */



/***************************************************************************/
/*                                                                         */
/* cwCreateWindow Function                                                 */
/*                                                                         */
/* The following function is used to create a window (the main window,     */
/* a child window, an icon window, etc.) and set it's initial size and     */
/* position. It returns the handle to the frame window.                    */
/*                                                                         */
/***************************************************************************/

HWND cwCreateWindow(
   HWND   hWndParent,  /* Handle to the parent of the window to be created */
   ULONG  ctldata,     /* Frame control flags for the window               */
   PCH    appname,     /* Class name of the window                         */
   PCH    title,       /* Title of the window                              */
   USHORT ResID,       /* Resource id value                                */
   INT    x,           /* Initial horizontal and vertical location         */
   INT    y,
   INT    cx,          /* Initial width and height of the window           */
   INT    cy,
   PHWND  hWndClient,  /* Handle to the client area of the window          */
   ULONG  lfStyle,     /* Frame window style                               */
   USHORT uSizeStyle)  /* User defined size and location flags             */
{
   USHORT rc;            /* accepts return codes from function calls       */
   HWND   hWndFrame;     /* local handle to created window frame           */
   USHORT SizeStyle;     /* local window positioning options               */
   CHAR   MsgBuffer[80]; /* buffer for error messages                      */
   HPS    hPS;           /* handle to a presentation space                 */
   int    xmod, ymod;    /* modifiers for sizing                           */
   #define DLGXMOD  4    /* Dialog units X modulo */
   #define DLGYMOD  8    /* Dialog units Y modulo */
   FONTMETRICS fm;       /* structure for determing modifiers              */
   /* Create the frame window                                              */
   hWndFrame = WinCreateStdWindow(hWndParent,  /* parent of window         */
                                  lfStyle,     /* frame window style       */
                                  &ctldata,    /* frame flags              */
                                  appname,     /* class name               */
                                  title,       /* window title             */
                                  0L,          /* client window style      */
                                  0,           /* module for resources     */
                                  ResID,       /* resource id              */
                                  (HWND FAR *)hWndClient); /* client handle*/

   /* if hWndFrame is NULL, an error occured when opening the window,      */
   /*     notify the user and exit this function                           */
   if(hWndFrame == 0)
     {
      WinLoadString(hAB, 0, IDS_ERR_WINDOW_CREATE, 80, MsgBuffer);
      WinMessageBox(HWND_DESKTOP, hWndParent, MsgBuffer,
                    0, 0, MB_OK|MB_ICONEXCLAMATION);
      return((HWND)0);
     }

   /* set up size options                                                  */
   SizeStyle = SWP_ACTIVATE | SWP_ZORDER | uSizeStyle;

   /* if the height, width, intial x or initial y values are non-zero,     */
   /*     then we will need to set up the modifiers for size and location  */
   if((cx > 0) || (cy > 0) ||
      (x > 0)  || (y > 0))
     {
      hPS = WinGetPS(HWND_DESKTOP);
      GpiQueryFontMetrics(hPS, (LONG)sizeof(FONTMETRICS), &fm);
      xmod = (INT)fm.lAveCharWidth;
      ymod = (INT)fm.lMaxBaselineExt;
      WinReleasePS(hPS);
     }
   /* if either the width or the height are non-zero, then the size of the */
   /*     created window will be changed, set SizeStyle accordingly        */
   if((cx > 0) || (cy > 0))
     SizeStyle |= SWP_SIZE;
   /* set the size and position of the window and activate it              */
   rc = WinSetWindowPos(hWndFrame, HWND_TOP, 0, 0,
                        (USHORT)(cx * xmod)/DLGXMOD,
                        (USHORT)(cy * ymod)/DLGYMOD, SizeStyle);

   /* if the either the intial x or y position of the window is non-zero,  */
   /*    set the window to its new position                                */
   if((x > 0) || (y > 0))
     rc = WinSetWindowPos(hWndFrame, HWND_TOP,
                          (USHORT)(x * xmod)/DLGXMOD,
                          (USHORT)(y * ymod)/DLGYMOD, 0, 0, SWP_MOVE);

   /* if rc is not set to TRUE then WinSetWindowPos failed, notify the user*/
   /*     and exit this function                                           */
   if(!rc)
     {
      WinLoadString(hAB, 0, IDS_ERR_WINDOW_POS, 80, MsgBuffer);
      WinMessageBox(HWND_DESKTOP, hWndParent, MsgBuffer,
                    0, 0, MB_OK|MB_ICONEXCLAMATION);
      return((HWND)0);
     }

   /* return the handle to the frame window                                */
   return(hWndFrame);
}  /* End of cwCreateWindow */





