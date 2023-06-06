/**
 * R2T2MD25 is a library that allows you to interact with the MD25 card to control
 * two EMG30 motors
 *
 *   Copyright (C) 2022  Lahcène Belhadi <lahcene.belhadi@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author: Lahcène Belhadi <lahcene.belhadi@gmail.com>
 * Created: 05/04/2022
 */

// lib
#include <stdlib.h>  // abs

#include "Arduino.h"
#include "HardwareSerial.h"  // HardwareSerial
#include "CapteurTOF.h"
#include "r2t2_md25.h"  // R2T2MD25


R2T2MD25::R2T2MD25(HardwareSerial* serial_port, const uint16 diameter) {

    set_serial(serial_port);
    set_diameter(diameter);

}

R2T2MD25::R2T2MD25(HardwareSerial* serial_port, const f32 coeffLeftDistance,
                    const f32 coeffRightDistance,
                    const f32 coeffLeftAngle,
                    const f32 coeffRightAngle) {

    set_serial(serial_port);

}

void R2T2MD25::set_serial(HardwareSerial* serial_port) {
    serial = serial_port;
    // setup the serial port
    serial->begin(BAUD_RATE, SERIAL_8N2); 
}

void R2T2MD25::set_coeff_left_distance(const f32 coeff) {
    this->coeffLeftDistance = coeff;
}

void R2T2MD25::set_coeff_right_distance(const f32 coeff) {
    this->coeffRightDistance = coeff;
}

void R2T2MD25::set_coeff_left_angle(const f32 coeff) {
    this->coeffLeftAngle = coeff;
}

void R2T2MD25::set_coeff_right_angle(const f32 coeff) {
    this->coeffRightAngle = coeff;
}

void R2T2MD25::set_diameter(const uint16 diameter) {
    this->diameter = diameter;
}

void R2T2MD25::set_wheel_to_patch(const Side side) {
    wheel_to_patch = side;
}

void R2T2MD25::set_weight_motor(const Side side, const f32 weight) {

    if (side == RIGHT) {
        right_weight = weight;
    } else {
        left_weight = weight;
    }

}

void R2T2MD25::command(const uint8 reg, const int32 value) {

    serial->write(COMMAND);
    serial->write(reg);
    serial->write(value);

}

void R2T2MD25::command(const uint8 reg) {

    serial->write(COMMAND);
    serial->write(reg);

}

void R2T2MD25::move(const Direction direction, const int8 speed, 
                    const uint32 distance) {

    reset_encoders();

    while (get_distance() < distance) {

        set_speed_motor(RIGHT, direction, speed);
        set_speed_motor(LEFT, direction, speed);

    }
    
    stop_motor();
}

void R2T2MD25::move(const Direction direction, const int8 speed,
                    const uint32 distance, const int16 treshold) {

    reset_encoders();

    while (get_distance() < distance) {
        if (direction == BACKWARD && seen_something(1, treshold)) {
            stop_motor();
            continue;
        } else if (direction == FORWARD && seen_something(0, treshold)) {
            stop_motor();
            continue;
        }
        set_speed_motor(RIGHT, direction, speed);
        set_speed_motor(LEFT, direction, speed);

    }

    stop_motor();

}

void R2T2MD25::move(const Direction direction, const int8 speed,
                    const uint32 distance, const Side side, const int treshold) {
    
    reset_encoders();

    while (get_distance() < distance) {
        if (direction == BACKWARD && seen_something(1, treshold)) {
            stop_motor();
            continue;
        } else if (direction == FORWARD && seen_something(0, treshold)) {
            stop_motor();
            continue;
        }
        set_speed_motor(side, direction, speed);
    }

    stop_motor(side);

}

void R2T2MD25::stop_motor(const Side side) {

    set_speed_motor(side, FORWARD, SPEED_STOP);

}

void R2T2MD25::stop_motor(void) {

    stop_motor(LEFT);
    stop_motor(RIGHT);

}

void R2T2MD25::set_speed_motor(const Side side, const Direction direction,  
                               const int8 speed) {

    // if the speed is negative do nothing
    if (speed < 0) return;

    // sets the mode to 0 to control one motor at a time
    command(MODE, 1);

    // initialized at full stop
    int8 real_speed = 0;

    if (direction == FORWARD) {

        if (speed > 127) {
            real_speed = 127;
        } else {
            real_speed += speed;
        }

    } else {

        if (speed > 127) {
            real_speed = -127;
        } else {
            real_speed -= speed;
        }

    }

    if (side == RIGHT) {

        if (wheel_to_patch != NONE && wheel_to_patch == RIGHT) {
            command(MOTOR_RIGHT_SPEED, real_speed * -1);
        } else {
            command(MOTOR_RIGHT_SPEED, real_speed);
        }

    } else {

        if (wheel_to_patch != NONE && wheel_to_patch == LEFT) {
            command(MOTOR_LEFT_SPEED, real_speed * -1);
        } else {
            command(MOTOR_LEFT_SPEED, real_speed);
        }

    }

}

int32 R2T2MD25::get_encoder(const Side side) {

    if (side == RIGHT) {
        command(ENCODER_GET_RIGHT);
    } else {
        command(ENCODER_GET_LEFT);
    }

    // asks the serial port to wait for 4 bytes
    // as the md25 sends 4 bytes of data 
    // to store the encoder value
    // once the 4 bytes had been received
    // store them into the serial port
    // then merge them to get the full value
    wait_for_bytes(4);

    int32 value = 0;
    value = serial->read();
    for (int8 i=0; i < 3; i++) {
        value <<= 8;  // shift 8 bits
        value += serial->read();
    }

    return abs(value);

}

void R2T2MD25::reset_encoders(void) {
    command(0x35);
}

void R2T2MD25::wait_for_bytes(const int8 bytes) {
    while (serial->available() < bytes);
}

int32 R2T2MD25::get_distance(const Side side) {

    f32 distance_per_turn = diameter * PI;
    f32 distance_per_deg = distance_per_turn / 360;

    int32 traveled_distance = (int32) get_encoder(side) * distance_per_deg;

    return traveled_distance;

}

int32 R2T2MD25::get_distance(void) {

    int32 traveled_by_right = get_distance(RIGHT);
    int32 traveled_by_left = get_distance(LEFT);

    if (traveled_by_left > traveled_by_right)
        return traveled_by_left;

    return traveled_by_right;

}

void R2T2MD25::turn(const Side side, const Direction direction, 
                    const uint16 angle, const int8 speed) {

    // the distance to travel
    int32 distance = distance_to_turn(angle, side);
    int treshold = 200;
    if (side == RIGHT) {
        move(direction, speed, distance, LEFT, treshold);
    } else {
        move(direction, speed, distance, RIGHT, treshold);
    }
}

void R2T2MD25::turn_without_treshold(const Side side, const Direction direction, 
                    const uint16 angle, const int8 speed) {
                      
    // the distance to travel
    int32 distance = distance_to_turn(angle, side);
    if (side == RIGHT) {
        move(direction, speed, distance, LEFT, -10);
    } else {
        move(direction, speed, distance, RIGHT, -10);
    }
}


void R2T2MD25::move_turn(const Side side, const Direction direction, 
                         const uint32 distance, const uint16 angle, 
                         const int8 speed) {
    
    // speed of the wheel that is supposed not to move
    int8 dead_speed = speed / 2;

    Side dead_wheel = side;
    Side moving_wheel;
    if (side == LEFT) {
        moving_wheel = RIGHT;
    } else {
        moving_wheel = LEFT;
    }

    reset_encoders();
    int32 distance_to_travel = distance_to_turn(angle, moving_wheel);
    while (get_distance(moving_wheel) < distance_to_travel) {

        set_speed_motor(dead_wheel, direction, dead_speed);
        set_speed_motor(moving_wheel, direction, speed);

    }

    stop_motor();

}

int32 R2T2MD25::distance_to_turn(const uint16 degrees, const Side side) {

    f32 radius = diameter / 2;
    int32 distance = (int32) 2*PI * radius * degrees/360;

    if (side == RIGHT) {
        return distance * left_weight;
    }

    return distance * right_weight;

}

bool R2T2MD25::seen_something(const int8& index, const int32& distance) {
    if (distance_sensor) {
        distance_sensor->measureDistances();
        return distance_sensor->distanceTreshold(index, distance);
    } else {
        Serial.println("I dont have any sensors");
    }

    return false;
}

void R2T2MD25::set_distance_sensor(CapteurTOF* sensor) {
    distance_sensor = sensor;
}