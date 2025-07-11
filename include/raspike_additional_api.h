#ifndef __RASPIKE_ADDITIONAL_API_H_
#define __RASPIKE_ADDITIONAL_API_H_

/* additional API */

#ifdef __cplusplus
extern "C" {
#endif  
#include <pbio/error.h>

  extern void hub_imu_get_orientation(float rotation_matrix[3*3]);
  extern float hub_imu_get_heading(void);
  extern pbio_error_t hub_imu_initialize_by_default(void);
  extern pbio_error_t hub_imu_initialize(float gyro_stationary_threshold, float accel_stationary_threshold,
    float angular_velocity_bias[3], float angular_velocity_scale[3], float acceleration_correction[6]);
  
#ifdef __cplusplus
}
#endif
  
#endif /* !__RASPIKE_ADDITIONAL_API_H_ */
