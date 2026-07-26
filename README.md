# DEV-SAMPLES-C-PM-SETOPT

An OS/2 Presentation Manager sample that demonstrates how to set the **color** and **font** of a Multi-Line Entry (MLE) control at runtime.

## Description

This sample application shows how to programmatically change the appearance of an MLE control using OS/2 PM message APIs. Two dialogs are available under the **Options** menu:

- **Set font…** — lists all installed public fonts, allows selecting a typeface and point size, and toggling bold, italic, underscore, and strikeout attributes via `MLM_SETFONT`.
- **Set color…** — presents foreground and background color list boxes populated from the 16 standard OS/2 palette entries, and applies the selection via `MLM_SETTEXTCOLOR` / `MLM_SETBACKCOLOR`.

A live MLE preview control in each dialog updates immediately when a selection changes.

### Key PM APIs demonstrated

| API | Purpose |
|-----|---------|
| `GpiQueryFonts` | Enumerate installed public and private fonts |
| `MLM_SETFONT` / `MLM_QUERYFONT` | Apply or read the MLE font (FATTRS structure) |
| `MLM_SETTEXTCOLOR` / `MLM_QUERYTEXTCOLOR` | Change MLE text (foreground) color |
| `MLM_SETBACKCOLOR` / `MLM_QUERYBACKCOLOR` | Change MLE background color |
| `WinDlgBox` / `WinDismissDlg` | Open a modal dialog and dismiss it |
| `WinSendDlgItemMsg` | Send messages to controls inside a dialog |

## Requirements

- OS/2 Warp 4.5x or later (including ArcaOS)
- GCC 9.2 (bitwiseworks / kLIBC) **or** OpenWatcom 2.0 C compiler

Install build tools on ArcaOS:
```
yum install git gcc make libc-devel binutils watcom-wrc watcom-wlink-hll
```

## Directory Structure

```
├── src/                    Source files
│   ├── SETOPT.C            Main source — main(), WndProc, font/color dialogs
│   ├── SETOPT.H            Global declarations and function prototypes
│   ├── SETOPT.RCH          Resource ID constants
│   ├── SETFONT.H           Control IDs for the Set Font dialog
│   ├── SETCOLOR.H          Control IDs for the Set Color dialog
│   ├── SETOPT.DEF          Module definition file (BLDLEVEL, exports)
│   ├── SETOPT.RC           Menu, dialogs, and string table resources
│   ├── SETOPT.ico          Application icon
│   └── setopt-ow.lnk       OpenWatcom wlink response file
├── bin-gcc/                GCC build output (created by build)
├── bin-ow/                 OpenWatcom build output (created by build)
├── compile-gcc.cmd         GCC build script
├── compile-ow.cmd          OpenWatcom build script
├── makefile-gcc            GCC makefile
├── makefile-ow             OpenWatcom makefile
├── .gitignore              Git ignore rules
├── LICENSE                 BSD 3-Clause License
└── README.md               This file
```

## Building

### Using GCC

```
compile-gcc.cmd
```

Or invoke make directly:
```
make -f makefile-gcc
```

Output: `bin-gcc\SETOPT.exe`

### Using OpenWatcom

```
compile-ow.cmd
```

Or invoke wmake directly:
```
wmake -f makefile-ow
```

Output: `bin-ow\SETOPT.exe`

### Cleaning build artifacts

```
make -f makefile-gcc clean
wmake -f makefile-ow clean
```

## BLDLEVEL Information

The executable embeds BLDLEVEL metadata queryable with the OS/2 `bldlevel` command:

```
bldlevel SETOPT.exe
```

## Compile Notes

This sample was updated to compile with GCC 9.2 (bitwiseworks) on ArcaOS.

1. **Header files** — use the kLIBC headers from `libc-devel`, not the OS/2 Toolkit headers. Verify `config.sys` contains `SET INCLUDE=C:\usr\include`.

2. **Resource and linker tools** — the Watcom Resource Compiler (`wrc`) and Watcom Linker (`wl.exe`) are used instead of the classic `rc.exe` / `ilink.exe`. Add to `config.sys`:
   ```
   SET EMXOMFLD_LINKER=wl.exe
   SET EMXOMFLD_TYPE=WLINK
   SET EMXOMFLD_RC_TYPE=WRC
   SET EMXOMFLD_RC=wrc.exe
   ```
   `compile-gcc.cmd` sets these variables automatically for the build session.

3. **DosAllocSeg** — the font dialog uses the 16-bit `DosAllocSeg` API to allocate the font metrics buffer. This is the original 1991 code and works on 32-bit OS/2 via thunking. The returned `SEL` is cast through `MAKEP` to obtain a flat pointer on the 32-bit side.

## Version History

- **1.02** — 2026-07-25
  - Reorganised directory structure (source files in `src/`, build outputs in `bin-gcc/` and `bin-ow/`)
  - Added OpenWatcom build support (`makefile-ow`, `compile-ow.cmd`, `setopt-ow.lnk`)
  - Modernised K&R function signatures to ANSI C prototypes
  - Improved source code documentation
  - Improved README
  - Added `.gitignore`

- **1.01** — 2026-05-25
  - Adapted to compile with GCC 9.2 on ArcaOS 5.0.7
  - Fixed resource ID conflicts; added `#ifndef` guards to `SETFONT.H` and `SETCOLOR.H`
  - Replaced `DosFreeSeg` calls with `DosFreeMem` for 32-bit compatibility

- **1.0** — 1991
  - Original version by Larry B. Finkelstein / Creative Systems Programming Corporation

## License

BSD 3-Clause License — see [LICENSE](LICENSE) file for details.

## Authors

- Larry B. Finkelstein / Creative Systems Programming Corporation (original, 1991)
- Martin Iturbide / OS2World (GCC port and reorganisation, 2026)

## Links

- https://github.com/OS2World/DEV-SAMPLES-C-PM-SETOPT
