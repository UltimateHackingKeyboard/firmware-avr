#ifndef EVENT_H
#define EVENT_H

    /* Macros: */
        #define EVENT_MASK_TYPE    0b10000000
        #define EVENT_MASK_STATE   0b01000000
        #define EVENT_MASK_PAYLOAD 0b00111111

        #define GET_EVENT_TYPE(event)    (event & EVENT_MASK_TYPE)
        #define GET_EVENT_STATE(event)   (event & EVENT_MASK_STATE)
        #define GET_EVENT_PAYLOAD(event) (event & EVENT_MASK_PAYLOAD)

        #define CONSTRUCT_EVENT_STATE(state) (state ? EVENT_MASK_STATE : 0)

        #define EVENT_TYPE_KEY  0
        #define EVENT_TYPE_LED  EVENT_MASK_TYPE
        #define EVENT_STATE_OFF 0
        #define EVENT_STATE_ON  EVENT_MASK_STATE

#endif
