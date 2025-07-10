#ifndef __RASPIKE_ADDITIONAL_API_H_
#define __RASPIKE_ADDITIONAL_API_H_

/* additional API */

#ifdef __cplusplus
extern "C" {
#endif  
#include <pbio/error.h>

  extern pbio_error_t hub_imu_reset_angular(void);
  extern void hub_imu_get_angular(float ang[3]);
  
#ifdef __cplusplus
}
#endif
  
#endif /* !__RASPIKE_ADDITIONAL_API_H_ */
