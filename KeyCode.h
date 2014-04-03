#ifndef __KEYCODE_H__
#define __KEYCODE_H__

    /* Macros: */
        #define CONSTRUCT_KEYCODE(Row, Col, ColNum) (Row*ColNum + Col)

        #define EXTRACT_KEYCODE_ROW(KeyCode, ColNum) (KeyCode / ColNum)
        #define EXTRACT_KEYCODE_COL(KeyCode, ColNum) (KeyCode % ColNum)

#endif
