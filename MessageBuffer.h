#ifndef __MESSAGE_BUFFER_H__
#define __MESSAGE_BUFFER_H__

    /* Includes: */
        #include <LUFA/Drivers/Misc/RingBuffer.h>

    /* Type Defines: */
        typedef struct {
            RingBuffer_t RingBuffer;
            uint8_t PreviousInsertByte;
            uint8_t PreviousRemoveByte;
            bool IsPreviousInsertByteEOM;
            bool IsPreviousRemoveByteEOM;
            uint8_t MessageCount;
        } MessageBuffer_t;

    /* Function Prototypes: */
        void MessageBuffer_InitBuffer(MessageBuffer_t *MessageBuffer, uint8_t* const DataPtr, const uint16_t Size);
        uint16_t MessageBuffer_GetCount(MessageBuffer_t *MessageBuffer);
        uint16_t MessageBuffer_GetFreeCount(MessageBuffer_t *MessageBuffer);
        bool MessageBuffer_IsEmpty(MessageBuffer_t *MessageBuffer);
        bool MessageBuffer_IsFull(MessageBuffer_t *MessageBuffer);
        void MessageBuffer_Insert(MessageBuffer_t *MessageBuffer, const uint8_t Data);
        uint8_t MessageBuffer_Remove(MessageBuffer_t *MessageBuffer);
        uint8_t MessageBuffer_Peek(MessageBuffer_t *MessageBuffer);

#endif
