/* DO NOT MODIFY THIS FILE*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "router.h"

/*
 * Sends the oldest packet in the queue (farthest from the head) and removes it
 * from the queue. Starts at the head, walks until it finds the end, sends that
 * packet.
 */
void starting_from_head_send_oldest_packet(Ends* ends) {
	if (ends->head != NULL) {
		// queue not empty | file d'attente non vide
		if (ends->head->next == NULL) {
			// one packet in queue | un paquet dans la file d'attente
			send_packet(ends->head);
			free(ends->head);
			ends->head = NULL;
		} else {
			// at least two packets | au moins deux paquets
			Packet* current = ends->head->next;
			Packet* previous = ends->head;
			while (current->next != NULL) {
				previous = current;
				current = current->next;
			}
			send_packet(current);
			free(current);
			previous->next = NULL;
		}
	}
}

/*
 * Send a packet. Simulated by printing the packet's fields.
 */
void send_packet(Packet* packet) {
	printf("  sending %3d -> %3d (%1d)\n", packet->source, packet->destination,
			packet->priority);
}

/*
 * Empties the queue, starting from the head. Used by fill_queue( )
 */
void empty_queue(Ends* ends) {
	Packet* temp = NULL;
	while (ends->head != NULL) {
		temp = ends->head;
		ends->head = ends->head->next;
		free(temp);
	}
}

/*
 * Fills the queue with test data. Uses enqueue_packet( ) to add the
 * constructed packets to the queue.
 */
void fill_queue(Ends* ends) {
	const int length = 10;
	const int source[] =
			{ 132, 255, 24, 12, 174, 162, 203, 216, 188, 103 };
	const int destination[] =
			{ 73, 137, 1, 15, 152, 31, 138, 137, 97, 243 };
	const int priority[] = { 1, 2, 1, 2, 2, 2, 1, 1, 2, 1 };
	int index;
	empty_queue(ends);
	ends->head = NULL;
	ends->tail = NULL;
	Packet* packet;
	for (index = 0; index < length; index++) {
		packet = (Packet*) malloc(sizeof(Packet));
		packet->source = source[index];
		packet->destination = destination[index];
		packet->priority = priority[index];
		packet->next = NULL;
		packet->prev = NULL;
		enqueue_packet(ends, packet);
	}
}

/*
 * Given a pointer, returns a string containing the last three hexadecimal
 * digits in the pointer's value, or "null" if the pointer is to NULL. Used by
 * show_paquet( )
 */
char* last_three(void* p, char* result) {
	char s[40];
	if (p == NULL) {
		strcpy(result, "null");
	} else {
		sprintf(s, "%p", p);
		strcpy(result, s + strlen(s) - 3);
	}
	return result;
}

/*
 * Prints all the fields in a queued packet, including its pointers. Used by
 * show_queue( )
 */
void show_packet(Packet* current) {
	char current_buffer[5], next_buffer[5], prev_buffer[5];
	printf("  %3d   %3d   %1d   %4s  %4s  %4s\n", current->source,
			current->destination, current->priority,
			last_three(current, current_buffer),
			last_three(current->next, next_buffer),
			last_three(current->prev, prev_buffer));
}

/*
 * Prints all the packets in the queue, in newest to oldest order, starting
 * from the head of the queue.
 */
void show_queue(Ends* ends) {
	char buffer[5];
	Packet* current = ends->head;
	printf("\n  head: %4s", last_three(ends->head, buffer));
	printf("\n  tail: %4s\n", last_three(ends->tail, buffer));
	printf("\n  --------------------------------\n");
	printf(
			"  from  to   pri  this  next  prev\n  ----  ---  ---  ----  ----  ----\n");
	while (current != NULL) {
		show_packet(current);
		current = current->next;
	}
	printf("  --------------------------------\n\n");
}

