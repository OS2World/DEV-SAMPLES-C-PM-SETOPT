/* KNB Version 2.00 */
#define INCL_PM
#include <os2.h>
#include "SETOPT.rch"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define WM_FAILEDVALIDATE (WM_USER+0)

/* these structures are used to add the program name to the task list      */
EXTERN HSWITCH hSwitch;
EXTERN SWCNTRL Swctl;

EXTERN CHAR szAppName[20]; /* class name of application                    */


EXTERN HAB  hAB;         /* Handle to the Anchor Block                     */
EXTERN HMQ  hMQ;         /* Handle to the Message Queue                    */
EXTERN HWND hWndFrame;   /* Handle to the Window Frame                     */
EXTERN HWND hWndClient;  /* Handle to the Client Window                    */


/* Function prototypes                                                     */
INT cwRegisterClass(VOID);
HWND cwCreateWindow(HWND, ULONG, PCH, PCH, USHORT, INT, INT, INT, INT, PHWND, ULONG, USHORT);

MRESULT EXPENTRY WndProc(HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY SETFONTMsgProc(HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY SETCOLORMsgProc(HWND, USHORT, MPARAM, MPARAM);
