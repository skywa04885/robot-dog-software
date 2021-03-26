#include <stdint.h>

#pragma once

/*

>>
    JSON-Like Example of Protocol in action
>>

[Write]:
\ Request:
    {
        op: SSFP_SERVO_WRITE,
        cnt: 2,
        d: [
            {
                n: 1,
                v: 25.0
            }, {
                n: 2,
                v: 4.6
            }
        ]
    }

[Read]:
\ Request:
    {
        op: SSFP_SERVO_WRITE,
        cnt: 2,
        d: [
            {
                n: 1,
                v: NULL
            }, {
                n: 2,
                v: NULL
            }
        ]
    }
\ Response:
    {
        op: SSFP_SERVO_READ_RESP,
        cnt: 2,
        d: [
            {
                n: 1,
                v: 25.0
            }, {
                n: 2,
                v: 4.6
            }
        ]
    }

*/

#define __SSFP_SERVO_ENUM_ID_T(N, A, B, C, SEP) \
    SERVO_LEG##N##SEP##A, \
    SERVO_LEG##N##SEP##B, \
    SERVO_LEG##N##SEP##C
#define SSFP_SERVO_ENUM_ID_T(N) __SSFP_SERVO_ENUM_ID_T(N, WAIST, HIP, KNEE, _)

typedef enum {
    SSFP_SERVO_ENUM_ID_T(0),
    SSFP_SERVO_ENUM_ID_T(1),
    SSFP_SERVO_ENUM_ID_T(2)
} ssfp_servo_id_t;

typedef enum {
    SSFP_SERVO_WRITE = 0,           /* Write Degrees Of Specified Servo */
    SSFP_SERVO_READ = 1,            /* Read Degrees Of Specified Servos */
    SSFP_SERVO_READ_RESP = 3        /* Read Response */
} ssfp_servo_pkt_op_t;

typedef struct __attribute__ (( packed )) {
    ssfp_servo_pkt_op_t     op;     /* Opcode */
    uint8_t                 cnt;    /* Read/Write, Servo Data Count */
    ssfp_servo_pos_t        d[0];   /* The Data */
} ssfp_servo_pkt_t;

typedef struct __attribute__ (( packed )) {
    uint8_t                 n;      /* The Servo ID */
    float                   v;      /* The Value For The Servo */
    ssfp_servo_pos_t        n[0];   /* The Next Servo */
} ssfp_servo_pos_t;
