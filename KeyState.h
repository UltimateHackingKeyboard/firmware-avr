#ifndef KEY_STATE_H
#define KEY_STATE_H

    /* Macros: */
        #define KEY_STATE_MASK_CURRENT    (1 << 0)
        #define KEY_STATE_MASK_PREVIOUS   (1 << 1)
        #define KEY_STATE_MASK_SUPPRESSED (1 << 2)

        #define KEY_STATE_IS_PRESSED(KeyState)    (KeyState & KEY_STATE_MASK_CURRENT)
        #define KEY_STATE_WAS_PRESSED(KeyState)   (KeyState & KEY_STATE_MASK_PREVIOUS)
        #define KEY_STATE_IS_SUPPRESSED(KeyState) (KeyState & KEY_STATE_MASK_SUPPRESSED)

        #define KEY_STATE_BECAME_PRESSED(KeyState) \
            (!KEY_STATE_WAS_PRESSED(KeyState) && KEY_STATE_IS_PRESSED(KeyState))
        #define KEY_STATE_BECAME_RELEASED(KeyState) \
            (KEY_STATE_WAS_PRESSED(KeyState) && !KEY_STATE_IS_PRESSED(KeyState))
        #define KEY_STATE_CHANGED(KeyState) \
            (KEY_STATE_BECAME_PRESSED(KeyState) || KEY_STATE_BECAME_RELEASED(KeyState))

        #define CONSTRUCT_KEY_STATE(PrevKeyState, CurrentKeyState, SuppressedKeyState) \
            ((CurrentKeyState    ? KEY_STATE_MASK_CURRENT    : 0) | \
             (PrevKeyState       ? KEY_STATE_MASK_PREVIOUS   : 0) | \
             (SuppressedKeyState ? KEY_STATE_MASK_SUPPRESSED : 0))

#endif
