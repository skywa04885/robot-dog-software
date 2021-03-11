#pragma once

#include <stdint.h>

#include "math.h"

typedef struct {

} ik_joint_t;

/**
 * Calculates the angles to reach the specified coordinate.
 */
void ik_calc_angles(ik_joint_t *joints, uint8_t n_joints, vec3_t target_coord, float *ret);