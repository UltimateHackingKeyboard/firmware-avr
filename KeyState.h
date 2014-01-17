#ifndef KEY_STATE_H
#define KEY_STATE_H

    /* Macros: */
        #define KEY_STATE_MASK_CURRENT 0b01
        #define KEY_STATE_MASK_PREV    0b10

        #define GET_KEY_STATE_CURRENT(KeyState) (KeyState & KEY_STATE_MASK_CURRENT)
        #define GET_KEY_STATE_PREV(KeyState)    (KeyState & KEY_STATE_MASK_PREV)

        #define CONSTRUCT_KEY_STATE(CurrentKeyState, PrevKeyState) \
            ((CurrentKeyState ? 1 : 0) | (PrevKeyState ? 0b10 : 0))

        #define KEY_STATE_PRESSED  0b11
        #define KEY_STATE_RELEASED 0b00

        #define IS_KEY_STATE_CHANGED(KeyState) \
            ( (KeyState & KEY_STATE_MASK_PREV) && !(KeyState & KEY_STATE_MASK_CURRENT)) || \
            (!(KeyState & KEY_STATE_MASK_PREV) &&  (KeyState & KEY_STATE_MASK_CURRENT))
        #define IS_KEY_PRESSED(KeyState) (KeyState & KEY_STATE_MASK_CURRENT)

#endif
