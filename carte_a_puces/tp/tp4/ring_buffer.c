#include <avr/io.h>

#include "ring_buffer.h"


void ring_buffer__init(struct ring_buffer* rb, uint8_t* buffer, uint8_t buffer_size){
    rb->buffer = buffer;
    rb->head = 0;
    rb->tail = 0;
    rb->maxlen = buffer_size;
}

void ring_buffer__push(struct ring_buffer* rb, uint8_t data){
    /*data is written in every case*/
    rb->buffer[rb->tail] = data;
    if(rb->head == 0){
    /*head is at the beginning, can't call rb->head - 1*/
        if(rb->tail == rb->maxlen - 1){
            /*buffer is full*/
            /*erase a character*/
            rb->head = 1;
            rb->tail = 0;
            rb->buffer[rb->tail] = 0;
        } else {
            rb->tail++;
        }
    } else if(rb->head == rb->maxlen - 1) {
        /*head is at the end, can't call rb->head++*/
        if(rb->tail == rb->head - 1){
            /*The buffer is full*/
            /*erase a character*/
            rb->head = 0;
            rb->tail = rb->maxlen - 1;
            rb->buffer[rb->tail] = 0;
        }else if(rb->tail == rb->maxlen - 1){
            rb->tail = 0;
            rb->buffer[rb->tail] = 0;
        } else {
            rb->tail++;
            rb->buffer[rb->tail] = 0;
        }
    } else {
        if(rb->tail == rb->head - 1){
            /*The buffer is full, erasing data*/
            rb->head++;
            rb->tail++;
            rb->buffer[rb->tail] = 0;
        } else if(rb->tail == rb->maxlen -1){
            rb->tail = 0;
            rb->buffer[rb->tail] = 0;
        } else {
            rb->tail++;
            rb->buffer[rb->tail] = 0;
        }
    }
}

uint8_t ring_buffer__pop(struct ring_buffer* rb, uint8_t *data){
    if(rb->head == rb->tail){
        /*empty buffer*/
        *data = 0;
        return 1;
    } else if(rb->head == rb->maxlen -1) {
        /*can't use rb->head++*/
        *data = rb->buffer[rb->head];
        rb->head = 0;
        return 0;
    } else {
        *data = rb->buffer[rb->head];
        rb->head++;
        return 0;
    }
}