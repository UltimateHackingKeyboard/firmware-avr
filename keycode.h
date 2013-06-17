#ifndef KEYCODE_H
#define KEYCODE_H

#define CONSTRUCT_KEYCODE(row, col, col_num) (row*col_num + col)

#define EXTRACT_KEYCODE_ROW(keycode, col_num) (keycode / col_num)
#define EXTRACT_KEYCODE_COL(keycode, row_num) (keycode % row_num)

#endif
