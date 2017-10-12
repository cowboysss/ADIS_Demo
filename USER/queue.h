#ifndef MY__QUEUE__H
#define MY__QUEUE__H

#define QUEUE_SIZE 150
#define MSG_LENGTH 200

#include "sys.h"
typedef struct
{
	u8 head;
	u8 tail;
	u8 count;
}Queue_Info;

void queue_init(Queue_Info *f_fifoInfo);
void queue_in(Queue_Info *f_fifoInfo);
void queue_out(Queue_Info *f_fifoInfo);
int queue_is_full(Queue_Info *f_fifoInfo);
int queue_is_empty(Queue_Info *f_fifoInfo);

#endif
