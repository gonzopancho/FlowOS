#ifndef __CHANNEL__
#define __CHANNEL__

#include <stdint.h>
#include <rte_atomic.h>
#include <rte_spinlock.h>

#include "packet.h"

typedef struct task* task_t;

typedef struct channel* channel_t; 

struct channel {
  uint16_t capacity;
  uint16_t size;
  rte_atomic16_t readIndex;
  rte_atomic16_t writeIndex;
  rte_atomic16_t closed;
  task_t producer;
  uint8_t producerIndex;
  task_t consumer;
  uint8_t consumerIndex;
  TAILQ_HEAD(, packet) head;
  rte_spinlock_t lock;
};

void channel_init(channel_t channel, size_t capacity);

int channel_is_full(channel_t channel);

int channel_is_empty(channel_t channel);

int channel_size(channel_t channel);

int channel_is_closed(channel_t channel);

void channel_insert(channel_t channel, packet_t packet);

packet_t channel_remove(channel_t channel);

packet_t channel_peek(channel_t channel);

void channel_close(channel_t channel);

void channel_register_producer(channel_t channel, uint8_t index, task_t producer);

void channel_register_consumer(channel_t channel, uint8_t index, task_t consumer);

void channel_notify_consumer(channel_t channel);

int channel_pool_create(uint16_t capacity);

void channel_pool_destroy();

channel_t channel_get();

void channel_put(channel_t channel);

#endif // __CHANNEL__
