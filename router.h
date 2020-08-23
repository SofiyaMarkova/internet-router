/* DO NOT MODIFY THIS FILE*/

#ifndef ROUTER_H_
#define ROUTER_H_

#include <stdint.h>

typedef struct PACKET {
	int source;
	int destination;
	int priority;
	struct PACKET* next;
	struct PACKET* prev;
} Packet;

typedef struct {
	Packet* head;
	Packet* tail;
} Ends ;

void enqueue_packet(Ends* ends, Packet* packet);
void send_packet(Packet* packet);
void starting_from_head_send_oldest_packet(Ends* ends);
void starting_from_tail_send_oldest_packet(Ends* ends);
void in_priority_starting_from_tail_send_oldest_packet(Ends* ends);
void fill_queue(Ends* ends);
char* last_three(void* p, char* result);
void show_packet(Packet* current);
void show_queue(Ends* ends);

#endif /* ROUTER_H_ */
