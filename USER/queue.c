/*
A queue used for SD card witer.
the queue is about 20 size
*/

#include "queue.h"
#include "interface.h"

char fifo_buffer[QUEUE_SIZE][MSG_LENGTH];
Queue_Info fifo_info;

void queue_init(Queue_Info *f_fifoInfo)
{
	f_fifoInfo->count = 0;
	f_fifoInfo->head = 0;
	f_fifoInfo->tail = 0;
}

void queue_in(Queue_Info *f_fifoInfo)
{
	if (f_fifoInfo->count >= QUEUE_SIZE)
	{
		// queue is full
		return;
	}
	f_fifoInfo->count++;
	f_fifoInfo->tail = (f_fifoInfo->tail+1)%QUEUE_SIZE;
}

void queue_out(Queue_Info *f_fifoInfo)
{
	if (f_fifoInfo->count == 0)
	{
		// queue is empty
		return;
	}
	f_fifoInfo->count--;
	f_fifoInfo->head = (f_fifoInfo->head+1)%QUEUE_SIZE;
}

int queue_is_full(Queue_Info *f_fifoInfo)
{
	if (f_fifoInfo->count == QUEUE_SIZE)
	{
		// queue is full
		return 1;
	}
	else
	{
		// queue is not full
		return 0;
	}
}

int queue_is_empty(Queue_Info *f_fifoInfo)
{
	if (f_fifoInfo->count == QUEUE_SIZE)
	{
		// queue is empty
		return 1;
	}
	else
	{
		// queue is not empty
		return 0;
	}
}

