#include <stdlib.h>
#include <stdio.h>
#include "router.h"
/*
 * Three times in succession: fills a queue with test data, prints
 * the queue, sends all packets in the queue (which should empty the queue),
 * then prints the queue again.
 *
 * The first time, sends using starting_from_head_send_oldest_packet( ). This
 * sends the oldest packet in the queue. "Oldest" means farthest from the
 * head, since that is where packets are enqueued.
 *
 * The second time, sends using starting_from_tail_send_oldest_packet( ). This
 * also sends the oldest packet in the queue.
 *
 * The third time, sends using
 * in_priority_starting_from_tail_send_oldest_packet( ).  This sends the oldest
 * priority 1 packet, if there is one; if not, sends the oldest priority 2
 * packet.
 */
int main(void) {
	Ends* ends = (Ends*)malloc(sizeof(Ends));
	ends->head = NULL;
	ends->tail = NULL;


	printf("Routing starting from head, no priority\n");
	printf("---------------------------------------\n\n");
	fill_queue(ends);
	printf("  Queue before routing:\n");
	show_queue(ends);
	while (ends->head != NULL) {
		starting_from_head_send_oldest_packet(ends);
	}
	printf("\n  Queue after routing:\n");
	show_queue(ends);


	printf("Routing starting from tail, no priority\n");
	printf("---------------------------------------\n\n");
	fill_queue(ends);
	printf("  Queue before routing:\n");
	show_queue(ends);
	while (ends->head != NULL) {
		starting_from_tail_send_oldest_packet(ends);
	}
	printf("\n  Queue after routing:\n");
	show_queue(ends);


	printf("Routing starting from tail, in order by priority\n");
	printf("------------------------------------------------\n\n");
	fill_queue(ends);
	printf("  Queue before routing:\n");
	show_queue(ends);
	printf("Routing from tail in priority:\n");
	while (ends->head != NULL) {
		in_priority_starting_from_tail_send_oldest_packet(ends);
	}
	printf("\n  Queue after routing:\n");
	show_queue(ends);

	printf("\n# END #\n");
	return EXIT_SUCCESS;
}

/*
 * Inserts new_packet at head of queue.
 */
void enqueue_packet(Ends* ends, Packet* new_packet) {
  if (!ends->head) {
    ends->head = new_packet;
  } else {
    new_packet->next = ends->head;
    ends->head->prev = new_packet;
    ends->head = new_packet;
  }

  if (!ends->tail) {
    ends->tail = new_packet;
  }
}

/*
 * Sends the oldest packet in the queue (farthest from the head) and removes it
 * from the queue. Starts at the tail and sends the packet it finds there.
 */
void starting_from_tail_send_oldest_packet(Ends* ends){
  if (ends->tail) {
    Packet *old_packet = ends->tail;

    send_packet(old_packet);
    
    ends->tail = old_packet->prev;

    free(old_packet);
  }

  if (!ends->tail) ends->head = NULL;
}

/*
 * If there is at least one priority 1 packet in the queue, sends the oldest
 * priority 1 packet. If there are no priority 1 packets in the queue, sends
 * the oldest priority 2 packet. Starts its search from the tail of the queue.
 */
void in_priority_starting_from_tail_send_oldest_packet(Ends* ends) {
  Packet *current = !ends->tail ? ends->head : ends-> tail;
  
  while(current->prev) {
    if (current->priority == 1) {
      break; // if we found a packet with priority equals 1, there's no need to continue search 
    }

    current = current->prev;
  }

  send_packet(current);

  if (current == ends->head && current == ends->tail) {
    ends->head = NULL;
    ends->tail = NULL;
  } else if (current == ends->tail) {
    ends->tail = current->prev;
    ends->tail->next = NULL;
  } else if (current == ends->head) {
    ends->head = current->next;
    ends->head->prev = NULL;
  } else {
    current->prev->next = current->next;
    current->next->prev = current->prev;
  }
    
  free(current);
}

