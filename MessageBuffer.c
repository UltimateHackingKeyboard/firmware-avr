/** \file
 *
 *  Special buffer to store messages.  It's a specialization of the ring buffer of LUFA.
 */

#include "MessageBuffer.h"

void MessageBuffer_InitBuffer(MessageBuffer_t *MessageBuffer, uint8_t* const DataPtr, const uint16_t Size)
{
    MessageBuffer->PreviousInsertByte = 1;  // Anything but 0
    MessageBuffer->PreviousRemoveByte = 1;  // Anything but 0
    MessageBuffer->IsPreviousInsertByteEOM = false;
    MessageBuffer->IsPreviousRemoveByteEOM = false;
    MessageBuffer->MessageCount = 0;
    RingBuffer_InitBuffer(&MessageBuffer->RingBuffer, DataPtr, Size);
}

uint16_t MessageBuffer_GetCount(MessageBuffer_t *MessageBuffer)
{
    return RingBuffer_GetCount(&MessageBuffer->RingBuffer);
}

uint16_t MessageBuffer_GetFreeCount(MessageBuffer_t *MessageBuffer)
{
    return RingBuffer_GetFreeCount(&MessageBuffer->RingBuffer);
}

bool MessageBuffer_IsEmpty(MessageBuffer_t *MessageBuffer)
{
    return RingBuffer_IsEmpty(&MessageBuffer->RingBuffer);
}

bool MessageBuffer_IsFull(MessageBuffer_t *MessageBuffer)
{
    return RingBuffer_IsFull(&MessageBuffer->RingBuffer);
}

void MessageBuffer_Insert(MessageBuffer_t *MessageBuffer, const uint8_t Data)
{
    RingBuffer_Insert(&MessageBuffer->RingBuffer, Data);
    if (MessageBuffer->PreviousInsertByte == 0 && Data == 0 && !MessageBuffer->IsPreviousInsertByteEOM) {
        MessageBuffer->MessageCount++;
        MessageBuffer->IsPreviousInsertByteEOM = true;
    } else {
        MessageBuffer->IsPreviousInsertByteEOM = false;
    }
    MessageBuffer->PreviousInsertByte = Data;
}

uint8_t MessageBuffer_Remove(MessageBuffer_t *MessageBuffer)
{
    uint8_t Data = RingBuffer_Remove(&MessageBuffer->RingBuffer);
    if (MessageBuffer->PreviousRemoveByte == 0 && Data == 0 && !MessageBuffer->IsPreviousRemoveByteEOM) {
        MessageBuffer->MessageCount++;
        MessageBuffer->IsPreviousRemoveByteEOM = true;
    } else {
        MessageBuffer->IsPreviousRemoveByteEOM = false;
    }
    MessageBuffer->PreviousRemoveByte = Data;
    return Data;
}

uint8_t MessageBuffer_Peek(MessageBuffer_t *MessageBuffer)
{
    return RingBuffer_Peek(&MessageBuffer->RingBuffer);
}
