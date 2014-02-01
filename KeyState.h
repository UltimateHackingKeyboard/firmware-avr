#ifndef KEY_STATE_H
#define KEY_STATE_H

    /* Macros: */
        #define KEY_STATE_MASK_CURRENT    0b001
        #define KEY_STATE_MASK_PREV       0b010
        #define KEY_STATE_MASK_SUPPRESSED 0b100

        #define GET_KEY_STATE_CURRENT(KeyState)    (KeyState & KEY_STATE_MASK_CURRENT)
        #define GET_KEY_STATE_PREV(KeyState)       (KeyState & KEY_STATE_MASK_PREV)
        #define GET_KEY_STATE_SUPPRESSED(KeyState) (KeyState & KEY_STATE_MASK_SUPPRESSED)

        #define CONSTRUCT_KEY_STATE(PrevKeyState, CurrentKeyState, SuppressedKeyState) \
            ((CurrentKeyState ? KEY_STATE_MASK_CURRENT : 0) | (PrevKeyState ? KEY_STATE_MASK_PREV : 0) | (SuppressedKeyState ? KEY_STATE_MASK_SUPPRESSED : 0))

        #define KEY_STATE_PRESSED  0b11
        #define KEY_STATE_RELEASED 0b00

        #define IS_KEY_STATE_CHANGED(KeyState) \
            ( (KeyState & KEY_STATE_MASK_PREV) && !(KeyState & KEY_STATE_MASK_CURRENT)) || \
            (!(KeyState & KEY_STATE_MASK_PREV) &&  (KeyState & KEY_STATE_MASK_CURRENT))
        #define IS_KEY_PRESSED(KeyState) (KeyState & KEY_STATE_MASK_CURRENT)

#endif
