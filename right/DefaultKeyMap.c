/** \file
 *
 *  The default keymap.
 */

#include "DefaultKeyMap.h"

// TODO: Should be put into the SRAM eventually.
const __flash uint8_t KeyMap[ROWS_NUM][TOTAL_COLS_NUM][LAYERS_NUM][ITEM_NUM_PER_KEY] =
{
    { /* key matrix row 1 */
        { /* A1 key */
            /* base layer  */ { HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE,            NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_ESCAPE,                            NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A2 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_1_AND_EXCLAMATION,                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F1,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A3 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_2_AND_AT,                          NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F2,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A4 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_3_AND_HASHMARK,                    NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F3,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A5 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_4_AND_DOLLAR,                      NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F4,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A6 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_5_AND_PERCENTAGE,                  NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F5,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A7 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_6_AND_CARET,                       NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F6,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A8 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_7_AND_AND_AMPERSAND,               NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F7,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A9 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_8_AND_ASTERISK,                    NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F8,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A10 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS,         NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F9,                                NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A11 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS,         NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F10,                               NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A12 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE,              NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F11,                               NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A13 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_EQUAL_AND_PLUS,                    NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_F12,                               NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* A14 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_BACKSPACE,                         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }
    }, /* key matrix row 2 */ {
        { /* B1 key */
            /* base layer  */ { HID_KEYBOARD_SC_TAB,                               NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B2 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_Q,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_ESCAPE,                            NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B3 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_W,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_TAB,                               HID_KEYBOARD_MODIFIER_LEFTCTRL | HID_KEYBOARD_MODIFIER_LEFTSHIFT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B4 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_E,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_T,                                 HID_KEYBOARD_MODIFIER_LEFTGUI },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B5 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_R,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_TAB,                               HID_KEYBOARD_MODIFIER_LEFTCTRL },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* no key */ {
            /* base layer  */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B7 */ {
            /* base layer  */ { HID_KEYBOARD_SC_T,                                 NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B8 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_U,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_HOME,                              NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B9 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_I,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_UP_ARROW,                          NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_MOVE_UP,                                     NO_ARGUMENT }
        }, /* B10 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_O,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_END,                               NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B11 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_P,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_DELETE,                            NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B12 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE, NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B13 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE, NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B14 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_BACKSLASH_AND_PIPE,                NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }
    }, /* key matrix row 3 */ {
        { /* C1 key */
            /* base layer  */ { LAYER_SWITCHER_KEY_MOUSE,                          NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C2 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_A,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_CAPS_LOCK,                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C3 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_S,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_LEFT_ARROW,                        HID_KEYBOARD_MODIFIER_LEFTCTRL },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_CLICK_RIGHT,                                 NO_ARGUMENT }
        }, /* C4 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_D,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_TAB,                               HID_KEYBOARD_MODIFIER_LEFTGUI },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_CLICK_MIDDLE,                                NO_ARGUMENT }
        }, /* C5 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_F,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_RIGHT_ARROW,                       HID_KEYBOARD_MODIFIER_LEFTCTRL },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_CLICK_LEFT,                                  NO_ARGUMENT }
        }, /* no key */ {
            /* base layer  */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C7 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_G,                                 NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* B7 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_Y,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_PAGE_UP,                           NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C8 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_J,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_LEFT_ARROW,                        NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_MOVE_LEFT,                                   NO_ARGUMENT }
        }, /* C9 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_K,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_DOWN_ARROW,                        NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_MOVE_DOWN,                                   NO_ARGUMENT }
        }, /* C10 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_L,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_RIGHT_ARROW,                       NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { MOUSE_MOVE_RIGHT,                                  NO_ARGUMENT }
        }, /* C11 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_SEMICOLON_AND_COLON,               NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C12 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE,              NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C13 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_ENTER,                             NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }
    }, /* key matrix row 4 */ {
        { /* D1 key */
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_LEFTSHIFT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D2 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_NON_US_BACKSLASH_AND_PIPE,         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D3 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_Z,                                 NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D4 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_X,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_PAGE_UP,                           HID_KEYBOARD_MODIFIER_LEFTSHIFT | HID_KEYBOARD_MODIFIER_LEFTCTRL },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D5 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_C,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_W,                                 HID_KEYBOARD_MODIFIER_LEFTGUI },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D6 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_V,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_PAGE_DOWN,                         HID_KEYBOARD_MODIFIER_LEFTSHIFT | HID_KEYBOARD_MODIFIER_LEFTCTRL },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D7 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_B,                                 NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* C7 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_H,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_PAGE_DOWN,                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D7 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_N,                                 NO_ARGUMENT },
            /* mod layer   */ { HID_KEYBOARD_SC_ESCAPE,                            NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D8 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_M,                                 NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D9 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,          NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D10 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D11 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,           NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* D12 key */ {
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_RIGHTSHIFT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }
    }, /* key matrix row 5 */ {
        { /* E1 key */
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_LEFTCTRL },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E2 key */ {
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_LEFTGUI },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E3 key */ {
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_LEFTALT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E4 key */ {
            /* base layer  */ { LAYER_SWITCHER_KEY_FN,                             NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* F1 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_SPACE,                             NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E5 key */ {
            /* base layer  */ { LAYER_SWITCHER_KEY_MOD,                            NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* no key */ {
            /* base layer  */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* no key */ {
            /* base layer  */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E6 key */ {
            /* base layer  */ { HID_KEYBOARD_SC_SPACE,                             NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* F2 key */ {
            /* base layer  */ { LAYER_SWITCHER_KEY_MOD,                            NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E7 key */ {
            /* base layer  */ { LAYER_SWITCHER_KEY_FN,                             NO_ARGUMENT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E8 key */ {
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_RIGHTALT },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E9 key */ {
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_RIGHTGUI },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }, /* E10 key */ {
            /* base layer  */ { NO_ACTION,                                         HID_KEYBOARD_MODIFIER_RIGHTCTRL },
            /* mod layer   */ { NO_ACTION,                                         NO_ARGUMENT },
            /* fn layer    */ { NO_ACTION,                                         NO_ARGUMENT },
            /* mouse layer */ { NO_ACTION,                                         NO_ARGUMENT }
        }
    }
};
