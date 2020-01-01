#ifndef PERIPH_SEMAPHORE_H_
#define PERIPH_SEMAPHORE_H_
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t periph_semaphore_t;

void periph_semaphore_init(periph_semaphore_t *periph_sem,uint8_t count);

bool periph_semaphore_p(periph_semaphore_t *periph_sem);

void periph_semaphore_v(periph_semaphore_t *periph_sem);

uint8_t periph_semaphore_get_count(periph_semaphore_t *periph_sem);

#endif
