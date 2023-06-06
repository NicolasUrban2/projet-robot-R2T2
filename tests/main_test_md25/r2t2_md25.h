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

#ifndef R2T2MD25_HPP
#define R2T2MD25_HPP

// lib
#include "HardwareSerial.h"
#include "CapteurTOF.h"
#include "Lidar.h"


// types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef long int32;
typedef long long int64;

typedef float f32;
typedef double f64;

// consts
#define BAUD_RATE 38400
#define SPEED_STOP 0x0

// registers
#define COMMAND 0x0
#define MODE 0x34
#define ACCELERATION 0x33

#define ENCODER_GET_RIGHT 0x24
#define ENCODER_GET_LEFT 0x23

#define MOTOR_RIGHT_SPEED 0x32
#define MOTOR_LEFT_SPEED 0x31

// enums
enum Direction {

    FORWARD,
    BACKWARD,

};

enum Side {

    LEFT,
    RIGHT,
    NONE,

};


class R2T2MD25 {

    // attributes
    private:
    // To move the robot without position
        // the diameter of the wheels
        uint16 diameter;
        // the serial port used for communication
        HardwareSerial* serial;
        // stores the wheel to patch
        Side wheel_to_patch = NONE;
        // stores the weight for the left motor
        f32 left_weight = 1;
        // stores the weight for the right motor
        f32 right_weight = 1;


    // To move the robot with position
        // To setup coefficient for encoder for distance (tic->mm)
        f32 coeffLeftDistance;
        f32 coeffRightDistance;

        // To setup coefficient for encoder for angle (tic->deg)
        f32 coeffLeftAngle;
        f32 coeffRightAngle;

        // To store and maj current value of encoders
        uint32 currentEncoderValueLeft;
        uint32 currentEncoderValueRight;

        // To store current orientation of robot
        f64 orientation;

        // To store current coordinates of robot
        f64 Cx;
        f64 Cy;

        // distance sensors
        CapteurTOF* distance_sensor;

        Lidar* lidar;


    // methods
    public: 

        /**
         * Sets up the library
         *
         * @param serial_port - the serial port used for communication
         * @param diameter - the diameter of the wheels
         */
        R2T2MD25(HardwareSerial*, const uint16);

        /**
         * Sets up the librairie
         * @param serial serial to communicate with
         * @param coeffLeftDist coefficient of encoders about distance (LEFT)
         * @param coeffRightDist coefficient of encoders about distance (RIGHT)
         * @param coeffLeftAngle coefficient of encoders about angle (LEFT)
         * @param coeffRightAngle coefficient of encoders about angle (RIGHT)
         */
        R2T2MD25(HardwareSerial*, const f32, const f32, const f32, const f32);

        /**
         * Sets up the serial port to use
         *
         * @param serial_port - the serial port to use
         */
        void set_serial(HardwareSerial*);

        /**
         * Sets up the coeff for encoder left distance
         * 
         * @param coeff 
         */
        void R2T2MD25::set_coeff_left_distance(const f32);

        /**
         * Sets up the coeff for encoder right distance
         * 
         * @param coeff 
         */
        void R2T2MD25::set_coeff_right_distance(const f32);

        /**
         * Sets up the coeff for encoder left angle
         * 
         * @param coeff 
         */
        void R2T2MD25::set_coeff_left_angle(const f32);

        /**
         * Sets up the coeff for encoder right angle
         * 
         * @param coeff 
         */
        void R2T2MD25::set_coeff_right_angle(const f32);

        /**
         * Sets up the diameter
         *
         * @param diameter - the diameter of the wheels
         */
        void set_diameter(const uint16);

        /**
         * Sets the wheel to patch
         * 
         * @param side - the side of the wheel
         */
         void set_wheel_to_patch(const Side);

        /**
         * Sets the weight of the motor
         * 
         * @param side - the motor
         * @param weight - the weight
         */
        void set_weight_motor(const Side, const f32);
        
        /**
         * Sends a command with a register and its value to the md25
         *
         * @param reg - the register
         * @param value - the register's value
         */
        void command(const uint8, const int32);

        /**
         * Sends a command with a register to the md25
         *
         * @param reg - the register
         */
        void command(const uint8);

        /**
         * Makes the EMG30 to move according to the direction and speed
         * until they reach the distance
         *
         * @param direction - the direction
         * @param speed - the speed
         * @param distance - the distance to travel
         */
        void move(const Direction, const int8, const uint32);
        
        /**
         * Makes the EMG30 to move according to the direction and speed
         * until they reach the distance, take care of the distance
         * sensor
         *
         * @param direction
         * @param speed
         * @param distance - the distance to travel
         * @param treshold - the distance treshold for the distance sensor
         */
        void move(const Direction, const int8, const uint32, const int16);

        /**
         * Makes the EMG30 to move according to the direction and speed
         * until they reach the distance. This methods controls only one 
         * motor.
         *
         * @param direction - the direction
         * @param speed - the speed
         * @param distance - the distance
         * @param side - the motor
         */
        void move(const Direction, const int8, const uint32, const Side, const int);

        /**
         * Changes the speed of the requested motor
         *         
         * @param side - the side
         * @param direction - the direction
         * @param speed - the speed
         */
        void set_speed_motor(const Side, const Direction, const int8);

        /**
         * Stops the motor
         *
         * @param side - the motor
         */
        void stop_motor(const Side);

        /**
         * Stops both motors
         */ 
        void stop_motor(void);

        /**
         * Returns the encoders value
         *
         * @param side - the side of the encoder
         */
        int32 get_encoder(const Side);

        /**
         * Resets the encoders count
         */
        void reset_encoders(void);

        /**
        * Waits until the Serial port received the required
        * amount of bytes
        *
        * @param bytes - the amount of bytes expected
        */
        void wait_for_bytes(const int8);
        
        /**
         * Returns the distance in cm traveled by a single wheel
         *
         * @param side - the side of the wheel
         */
        int32 get_distance(const Side);

        /**
         * Returns the distance in cm traveled according to the wheel
         * that has traveled the longest distance
         */
        int32 get_distance(void);

        /**
         * Makes a turn of the requested angle, in the requested direction and
         * in the requested side and speed
         *
         * @param side - the side
         * @param direction - the direction
         * @param angle - the angle
         * @param speed - the speed
         */
        void turn(const Side, const Direction, const uint16, const int8);

        /**
         * Makes a turn while moving according to the side, direction, angle
         * and speed
         *
         * @param side - the side
         * @param direction - the direction
         * @param distance - the distance to travel
         * @param angle - the turning angle
         * @param speed - the speed
         */
        void move_turn(const Side, const Direction, const uint32, const uint16, const int8);

        /**
         * Returns the distance required to make a turn of the requested
         * amount of degrees
         *
         * @param degrees - the amount of degrees to turn
         * @param side - the side
         */
        int32 distance_to_turn(const uint16, const Side);

        /**
         * Tells if the sensor did see something
         *
         * @param index - the sensor's index
         * @param distance - the distance
         */
        bool seen_something(const int8&, const int32&);

        /**
         * Set the distance sensor
         *
         * @param sensor - the distance sensor
         */
        void set_distance_sensor(CapteurTOF*);

        void turn_without_treshold(const Side side, const Direction direction, const uint16 angle, const int8 speed);


};

#endif  // R2T2MD25_HPP