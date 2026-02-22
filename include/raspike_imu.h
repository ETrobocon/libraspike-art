#ifndef __RASPIKE_IMU_H_
#define __RASPIKE_IMU_H_

/* additional API */

#ifdef __cplusplus
extern "C" {
#endif  
#include <pbio/error.h>

  extern void raspike_imu_get_acceleration(float accel[3]);
  extern void raspike_imu_get_angular_velocity(float angv[3]);
  extern void raspike_imu_get_orientation(float rotation_matrix[3*3]);
  extern float raspike_imu_get_heading(void);
  extern pbio_error_t raspike_imu_initialize_by_default(void);
  extern pbio_error_t raspike_imu_initialize_by_flash(void);
  extern pbio_error_t raspike_imu_initialize(float gyro_stationary_threshold, float accel_stationary_threshold,
    float angular_velocity_bias[3], float angular_velocity_scale[3], float acceleration_correction[6]);
  
#ifdef __cplusplus
}
#endif
  
#endif /* !__RASPIKE_IMU_H_ */
