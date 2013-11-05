
#ifndef RELAY_H_
#define RELAY_H_

#include <types.h>

enum relay_num
{
    relay_o1 = 0,
    relay_o2,
    relay_o3,
    relay_o4,
    relay_o5,
    relay_o6,
    relay_o7,
    relay_o8,
    relay_o9,
    relay_o10,

    relay_c1,
    relay_c2,
    relay_c3,
    relay_c4,
    relay_c5,
    relay_c6,
    relay_c7,
    relay_c8,
    relay_c9,
    relay_c10,
};

enum relay_state
{
    relay_open = 0,
    relay_close = 1
};

typedef struct
{
    uint8_t num;
    uint8_t onoff;
} relay_param_t;

extern status_t relay_device_init(void);

#endif /* RELAY_H_ */
