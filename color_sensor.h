/*
 * color_sensor.h
 *
 *  Created on: Nov 17, 2025
 *      Author: Lou
 */

#ifndef COLOR_SENSOR_H_
#define COLOR_SENSOR_H_

typedef enum {
    UNKNOWN,
    RED,
    BLUE,
    GREEN,
    YELLOW,
} color_type;
extern uint8_t raw_color[8];
extern color_type actual_color;
//TODO: configuration
void setup_color_sensor();
void setup_SCL_SDA();

//TODO: function
void color_sensor_init();
//set Wait Register
//set all enable register
//I2C_WriteByte
/* Start the sensor
1.SET PON,WEN,AEN are fields in the Enable register (0x00) - start the sensor, go to wait
2.Set PON, AEN -> go to RGBC
3.Set for 2.4ms Atime, Wtime
3.
 */

void color_sensor_read();
//read into buffer
//assign read color accordingly
//readblock
color_type get_color();
//return read color
void color_sensor_read();
void print_raw_color();
#endif /* COLOR_SENSOR_H_ */
