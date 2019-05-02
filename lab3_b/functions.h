
#ifndef functionsTas
#define functionsTas
#include "atomic_ops.h"


void tas_lock(volatile unsigned long* ptr){

	while(tas(ptr)){}
}

void unlock(volatile unsigned long* ptr){
	*ptr=0;
}

void tatas_lock(volatile unsigned long* ptr){
	tatas_acquire_without(ptr);
}

void tatas_lock_backoff(volatile unsigned long* ptr){
	tatas_acquire(ptr);
}

void ticket_lock(ticket_lock_t* ptr){
	ticket_acquire(ptr);
}
void ticket_unlock(ticket_lock_t* ptr){
	ticket_release(ptr);
}

#endif