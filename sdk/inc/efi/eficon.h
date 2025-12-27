/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    eficon.h

Abstract:

    EFI console protocol definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef _EFICON_H
#define _EFICON_H

//
// Simple text input protocol.
//

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID { 0x387477c1, 0x69c7, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }
#define SIMPLE_TEXT_INPUT_PROTOCOL_ EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL, SIMPLE_TEXT_INPUT_INTERFACE;

typedef struct {
    UINT16 ScanCode;
    CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET) (
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
    IN BOOLEAN                        ExtendedVerification
    );

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY) (
    IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
    OUT EFI_INPUT_KEY                  *Key
    );

struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET    Reset;
    EFI_INPUT_READ_KEY ReadKeyStroke;
    EFI_EVENT          WaitForKey;
};

//
// Extended simple text input protocol.
//

#define EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID { 0xdd9e7534, 0x7762, 0x4698, { 0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa } }

typedef struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

#define EFI_SCROLL_LOCK_ACTIVE 0x01
#define EFI_NUM_LOCK_ACTIVE    0x02
#define EFI_CAPS_LOCK_ACTIVE   0x04
#define EFI_KEY_STATE_EXPOSED  0x40
#define EFI_TOGGLE_STATE_VALID 0x80

typedef UINT8 EFI_KEY_TOGGLE_STATE;

typedef struct {
    UINT32               KeyShiftState;
    EFI_KEY_TOGGLE_STATE KeyToggleState;
} EFI_KEY_STATE;

typedef struct {
    EFI_INPUT_KEY Key;
    EFI_KEY_STATE KeyState;
} EFI_KEY_DATA;

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET_EX) (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN BOOLEAN                           ExtendedVerification
    );

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY_EX) (
    IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    OUT EFI_KEY_DATA                      *KeyData
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_STATE) (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN EFI_KEY_TOGGLE_STATE              *KeyToggleState
    );

typedef
EFI_STATUS
(EFIAPI *EFI_KEY_NOTIFY_FUNCTION) (
    IN EFI_KEY_DATA *KeyData
    );

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_KEYSTROKE_NOTIFY) (
    IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN  EFI_KEY_DATA                      *KeyData,
    IN  EFI_KEY_NOTIFY_FUNCTION           KeyNotificationFunction,
    OUT VOID                              **NotifyHandle
    );

typedef
EFI_STATUS
(EFIAPI *EFI_UNREGISTER_KEYSTROKE_NOTIFY) (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN VOID                              *NotificationHandle
    );

struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL {
    EFI_INPUT_RESET_EX              Reset;
    EFI_INPUT_READ_KEY_EX           ReadKeyStrokeEx;
    EFI_EVENT                       WaitForKeyEx;
    EFI_SET_STATE                   SetState;
    EFI_REGISTER_KEYSTROKE_NOTIFY   RegisterKeyNotify;
    EFI_UNREGISTER_KEYSTROKE_NOTIFY UnregisterKeyNotify;
};

//
// Simple text output protocol.
//

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID { 0x387477c2, 0x69c7, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }
#define SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID

//
// Text attributes.
//

#define EFI_TEXT_ATTR(Foreground, Background) ((Foreground) | ((Background) << 4))

#define EFI_BLACK        0x00
#define EFI_BLUE         0x01
#define EFI_GREEN        0x02
#define EFI_CYAN         0x03
#define EFI_RED          0x04
#define EFI_MAGENTA      0x05
#define EFI_BROWN        0x06
#define EFI_LIGHTGRAY    0x07
#define EFI_BRIGHT       0x08
#define EFI_DARKGRAY     0x08
#define EFI_LIGHTBLUE    0x09
#define EFI_LIGHTGREEN   0x0a
#define EFI_LIGHTCYAN    0x0b
#define EFI_LIGHTRED     0x0c
#define EFI_LIGHTMAGENTA 0x0d
#define EFI_YELLOW       0x0e
#define EFI_WHITE        0x0f

#define EFI_BACKGROUND_BLACK     0x00
#define EFI_BACKGROUND_BLUE      0x10
#define EFI_BACKGROUND_GREEN     0x20
#define EFI_BACKGROUND_CYAN      0x30
#define EFI_BACKGROUND_RED       0x40
#define EFI_BACKGROUND_MAGENTA   0x50
#define EFI_BACKGROUND_BROWN     0x60
#define EFI_BACKGROUND_LIGHTGRAY 0x70

//
// Required arrow shapes.
//
#define ARROW_LEFT                         0x2190
#define ARROW_UP                           0x2191
#define ARROW_RIGHT                        0x2192
#define ARROW_DOWN                         0x2193

//
// Unicode box draw characters.
//

#define BOXDRAW_HORIZONTAL                 0x2500
#define BOXDRAW_VERTICAL                   0x2502
#define BOXDRAW_DOWN_RIGHT                 0x250c
#define BOXDRAW_DOWN_LEFT                  0x2510
#define BOXDRAW_UP_RIGHT                   0x2514
#define BOXDRAW_UP_LEFT                    0x2518
#define BOXDRAW_VERTICAL_RIGHT             0x251c
#define BOXDRAW_VERTICAL_LEFT              0x2524
#define BOXDRAW_DOWN_HORIZONTAL            0x252c
#define BOXDRAW_UP_HORIZONTAL              0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL        0x253c

#define BOXDRAW_DOUBLE_HORIZONTAL          0x2550
#define BOXDRAW_DOUBLE_VERTICAL            0x2551

#define BOXDRAW_DOWN_RIGHT_DOUBLE          0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT          0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT          0x2554
#define BOXDRAW_DOWN_LEFT_DOUBLE           0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT           0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT           0x2557

#define BOXDRAW_UP_RIGHT_DOUBLE            0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT            0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT            0x255a
#define BOXDRAW_UP_LEFT_DOUBLE             0x255b
#define BOXDRAW_UP_DOUBLE_LEFT             0x255c
#define BOXDRAW_DOUBLE_UP_LEFT             0x255d

#define BOXDRAW_VERTICAL_RIGHT_DOUBLE      0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT      0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT      0x2560

#define BOXDRAW_VERTICAL_LEFT_DOUBLE       0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT       0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT       0x2563

#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE     0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL     0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL     0x2566

#define BOXDRAW_UP_HORIZONTAL_DOUBLE       0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL       0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL       0x2569

#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE 0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL 0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL 0x256c

//
// Required block elements.
//
#define BLOCKELEMENT_FULL_BLOCK            0x2588
#define BLOCKELEMENT_LIGHT_SHADE           0x2591

//
// Required geometric shapes.
//
#define GEOMETRICSHAPE_UP_TRIANGLE         0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE      0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE       0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE       0x25c4

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL, SIMPLE_TEXT_OUTPUT_INTERFACE;

//
// Display mode.
//
typedef struct {
    INT32   MaxMode;
    INT32   Mode;
    INT32   Attribute;
    INT32   CursorColumn;
    INT32   CursorRow;
    BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_RESET) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN BOOLEAN                         ExtendedVerification
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN CHAR16                          *String
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_TEST_STRING) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN CHAR16                          *String
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_QUERY_MODE) (
    IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN  UINTN                           ModeNumber,
    OUT UINTN                           *Columns,
    OUT UINTN                           *Rows
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_MODE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN                           ModeNumber
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN                           Attribute
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN                           Column,
    IN UINTN                           Row
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_ENABLE_CURSOR) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN BOOLEAN                         Visible
    );

//
// Protocol interface.
//
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET               Reset;

    EFI_TEXT_STRING              OutputString;
    EFI_TEXT_TEST_STRING         TestString;

    EFI_TEXT_QUERY_MODE          QueryMode;
    EFI_TEXT_SET_MODE            SetMode;
    EFI_TEXT_SET_ATTRIBUTE       SetAttribute;

    EFI_TEXT_CLEAR_SCREEN        ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR       EnableCursor;

    SIMPLE_TEXT_OUTPUT_MODE      *Mode;
};

#endif // _EFICON_H
