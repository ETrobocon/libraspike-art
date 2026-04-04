

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include "raspike_com.h"
#include "raspike_protocol_api.h"

#include "spike/pup/motor.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/forcesensor.h"
#include "spike/pup/ultrasonicsensor.h"
#include "spike/hub/system.h"
#include "spike/hub/light.h"
#include "spike/hub/imu.h"
#include "spike/hub/display.h"
#include "spike/hub/button.h"
#include "spike/hub/battery.h"


#define RASPIKE_COM_NAME "/dev/USB_SPIKE"

void *receiver_task(void *arg)
{
  while(1) {
    raspike_prot_receive();
  }
}

void ultrasonicsensor_test(void)
{
  // Ultrasonic sensor test
  pup_device_t *us = pup_ultrasonic_sensor_get_device(PBIO_PORT_ID_F);
  sleep(1);

#if 0
  // 超音波センサーのライトを点けるとハングするので、コメントアウトする
  pup_ultrasonic_sensor_light_set(us,100,0,0,0);
  sleep(1);
  pup_ultrasonic_sensor_light_set(us,0,50,0,0);
  sleep(1);
  pup_ultrasonic_sensor_light_set(us,0,0,50,0);  
  sleep(1);
  pup_ultrasonic_sensor_light_set(us,0,0,0,100);  
  sleep(1);
  pup_ultrasonic_sensor_light_set(us,100,100,100,100);  
  sleep(1);
  pup_ultrasonic_sensor_light_off(us);
#endif
  while ( 1 ) {
    printf("US: Distance = %d, presence =%d\n",
	   pup_ultrasonic_sensor_distance(us),
	   pup_ultrasonic_sensor_presence(us));
    sleep(1);
  }
}  

void motor_test(void)
{
  pup_motor_t *mot1 = pup_motor_get_device(PBIO_PORT_ID_A);  
  pup_motor_t *mot2 = pup_motor_get_device(PBIO_PORT_ID_C);
  pup_motor_t *mot3 = pup_motor_get_device(PBIO_PORT_ID_B);  
  pbio_error_t err= pup_motor_setup(mot1,PUP_DIRECTION_CLOCKWISE,true);
  err= pup_motor_setup(mot2,PUP_DIRECTION_CLOCKWISE,true);
  err= pup_motor_setup(mot3,PUP_DIRECTION_CLOCKWISE,true);
  int i = 50;

  while ( 1 ) {
    int power = i%50+30;
    pup_motor_set_power(mot1,power);
    pup_motor_set_power(mot2,power);    
    pup_motor_set_power(mot3,power);  
    usleep(10*1000);
  }
  pup_motor_set_speed(mot1,400);
  sleep(1);
  pup_motor_stop(mot1);
  sleep(1);
  pup_motor_set_speed(mot1,-400);
  sleep(1);
  pup_motor_brake(mot1);
  sleep(1);
  pup_motor_set_speed(mot1,400);  
  sleep(1);
  pup_motor_hold(mot1);
  sleep(1);
  
  while ( i < 100 ) {
    pup_motor_set_speed(mot1,i+400);
    printf("mot1:power=%d speed=%d count=%d isStall=%d\n",
	   pup_motor_get_power(mot1),
	   pup_motor_get_speed(mot1),
	   pup_motor_get_count(mot1),
	   pup_motor_is_stalled(mot1));
    sleep(1);
  }
}

void display_test(void)
{
  hub_display_number(12);
  sleep(3);

  hub_display_char('X');
  sleep(2);

  hub_display_text("Ras Pike!",1000,1000);

  hub_display_text_scroll("ETRobocon20th",500);
}

#include "color.h"
void light_test(void)
{
  pbio_color_hsv_t hsv = {30, 100, 100};
  hub_light_on_hsv(&hsv);
  sleep(2);
  hub_light_on_color(PBIO_COLOR_GREEN);
  sleep(2);
  hub_light_off();
  
}
  
#include "speaker.h"
void speaker_test(void)
{
  hub_speaker_set_volume(50);
  hub_speaker_play_tone((uint16_t)NOTE_C5,2000);
  hub_speaker_play_tone(NOTE_A5,SOUND_MANUAL_STOP);
  sleep(3);
  hub_speaker_stop();
  hub_light_on_color(PBIO_COLOR_GREEN);
  sleep(3);
  hub_system_shutdown();
}



void colorsensor_test(void)
{
  int i = 0;
  pup_device_t *col = pup_color_sensor_get_device(PBIO_PORT_ID_E);

  while ( i < 100 ) {

    pup_color_rgb_t rgb = pup_color_sensor_rgb(col);
    printf("[RGB]r=%d g=%d b=%d\n",rgb.r,rgb.g,rgb.b);

    i++;
    sleep(1);
  }

}

void imu_test(void) {
  float accel[3], angv[3];
  float rot[3*3];
  float heading;

  hub_imu_init();

  // HackSPi's hub is tilted at ~51 degrees.
  hub_imu_set_tilt(51.0f);

  // Wait for IMU to become ready
  while (!hub_imu_is_ready()) {
    printf(".");
    usleep(100*1000);
  }
  printf("IMU is ready.\n");

  for (int i = 0; i < 10; i++) { // 10 seconds
    hub_imu_get_acceleration(accel);
    hub_imu_get_angular_velocity(angv);
    printf("[accel(mm/s²)] x=%f y=%f z=%f [angv(deg/s)] x=%f y=%f z=%f\n",
          accel[0],accel[1],accel[2],angv[0],angv[1],angv[2]);
    heading = hub_imu_get_heading();
    printf("[heading] %f\n",heading);
    sleep(1);
  }

  // Test reset heading
  printf("Resetting heading to 0 in 3 seconds...\n");
  sleep(3);
  hub_imu_reset_heading();
  printf("Resetting heading...\n");
  sleep(1);

  for (int i = 0; i < 10; i++) { // 10 seconds
    hub_imu_get_acceleration(accel);
    hub_imu_get_angular_velocity(angv);
    printf("[accel(mm/s²)] x=%f y=%f z=%f [angv(deg/s)] x=%f y=%f z=%f\n",
          accel[0],accel[1],accel[2],angv[0],angv[1],angv[2]);
    heading = hub_imu_get_heading();
    printf("[heading] %f\n",heading);
    sleep(1);
  }
  printf("Done.\n");
}

#define LOOP_DELAY_US 10000  // 10 ms loop
#define SQUARE_SIDES 8
#define KP 1.2
#define KI 0.001
#define KD 0.02
#define MAX_POWER 100
#define BASE_POWER 55
#define MIN_POWER 30

float pid_control(float target, float current, float *integral, float *last_error) {
  float error = target - current;
  *integral += error * (LOOP_DELAY_US / 1000000.0f);
  float derivative = (error - *last_error) / (LOOP_DELAY_US / 1000000.0f);
  *last_error = error;
  return KP * error + KI * (*integral) + KD * derivative;
}

float clamp(float power) {
    if (power >= 0) {
        if (power > MAX_POWER) return MAX_POWER;
        if (power < MIN_POWER) return MIN_POWER;
    } else {
        if (power < -MAX_POWER) return -MAX_POWER;
        if (power > -MIN_POWER) return -MIN_POWER;
    }
    return power;
}

void drive_straight(pup_motor_t *right, pup_motor_t *left, float duration_sec, float target_heading) {
  float integral = 0.0;
  float last_error = 0.0;
  float time_start = (float)clock() / CLOCKS_PER_SEC;

  while (((float)clock() / CLOCKS_PER_SEC) - time_start < duration_sec) {
    float current_heading = hub_imu_get_heading();
    float correction = pid_control(target_heading, current_heading, &integral, &last_error);

    int left_power = clamp(BASE_POWER + correction);
    int right_power = clamp(BASE_POWER - correction);

    pup_motor_set_power(right,right_power);
    pup_motor_set_power(left,left_power);
    usleep(LOOP_DELAY_US);
  }
  pup_motor_stop(right);
  pup_motor_stop(left);
}

void turn_to_heading(pup_motor_t *right, pup_motor_t *left, float target_heading) {
  float integral = 0.0;
  float last_error = 0.0;

  while (true) {
    float current_heading = hub_imu_get_heading();
    float error = target_heading - current_heading;
    // normalize error to [-180, 180]
    if (error > 180) error -= 360;
    if (error < -180) error += 360;
    if (fabs(error) < 1.0) break;

    float power = clamp(pid_control(target_heading, current_heading, &integral, &last_error));

    pup_motor_set_power(right,-power);
    pup_motor_set_power(left,power);
    usleep(LOOP_DELAY_US);
  }
}

void imu_run_test(void) {
  hub_imu_init();

  // HackSPi's hub is tilted at ~51 degrees.
  hub_imu_set_tilt(51.0f);

  // Wait for IMU to become ready
  while (!hub_imu_is_ready()) {
    printf(".");
    usleep(100*1000);
  }
  printf("IMU is ready.\n");

  pup_motor_t *right = pup_motor_get_device(PBIO_PORT_ID_A);
  pup_motor_t *left  = pup_motor_get_device(PBIO_PORT_ID_B);
  pbio_error_t err= pup_motor_setup(right,PUP_DIRECTION_CLOCKWISE,true);
  err= pup_motor_setup(left,PUP_DIRECTION_COUNTERCLOCKWISE,true);
  float heading = hub_imu_get_heading();
  printf("[heading] initial    = %f\n",heading);

  for (int i = 0; i < SQUARE_SIDES; i++) {
    drive_straight(right, left, 3.0, heading); // 3 seconds straight
    heading = hub_imu_get_heading();
    printf("[heading] before turn= %f\n",heading);
    turn_to_heading(right, left, heading+90);
    heading = hub_imu_get_heading();
    printf("[heading] after turn = %f\n",heading);
  }
  pup_motor_stop(right);
  pup_motor_stop(left);
  printf("Done.\n");
}

int main(int argc,char const *argv[])
{
  const char *p = RASPIKE_COM_NAME;
  if ( argc > 1 ) {
    p = argv[1];
  }

  RPComDescriptor *desc = raspike_open_usb_communication(p);

  if ( !desc ) {
    printf("Cannot Open desc name=%s\n",p);
    exit(-1);
  }

  raspike_prot_init(desc);

  pthread_t receive_thread;
  pthread_create(&receive_thread,0,receiver_task,0);


  //ultrasonicsensor_test();
  //  motor_test();
  //display_test();
  // light_test();
  //  speaker_test();
  //    motor_test();
  //colorsensor_test();
  imu_test();
  //imu_run_test();

  return 0;
}
