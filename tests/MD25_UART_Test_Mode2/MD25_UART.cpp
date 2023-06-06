 /*
  MD25_UART.cpp - Library for the Devantech MD25 motor controller.
  Created by Philippe Gozlan CERI 2022.
  Par défaut Serial1 sur Mega2560
*/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "MD25_UART.h"


/*
* Constructors
*/
MD25_UART::MD25_UART()
{
     Serial1.begin(38400, SERIAL_8N2);  // mettre Serial1.begin(38400) si ça bug ?
     this->mode = 0; // Mode par défaut
}

/*
* Public Methods
*/

// Gets
long MD25_UART::getEncoder1() // Retourne le nombre de ticks de l'encodeur1. Le nombre de ticks augmente ou diminue en fonction du sens de rotation.
{
  long result1 = 0; 
  Serial1.write(CmdReg);  // A factoriser pour toutes les méthodes
  Serial1.write(EncoderOneReg);
  while(Serial1.available() < 4);          // Wait for 4 bytes, encoder 1 value
  result1 = Serial1.read();                 // First byte for encoder 1, HH.
  result1 <<= 8;
  result1 += Serial1.read();                // Second byte for encoder 1, HL
  result1 <<= 8;
  result1 += Serial1.read();                // Third byte for encoder 1, LH
  result1 <<= 8;
  result1  += Serial1.read();               // Fourth byte for encoder 1, LL
  delay(5);                                
  return result1;
}

long MD25_UART::getEncoder2()
{
  long result2 = 0;
  Serial1.write(CmdReg);
  Serial1.write(EncoderTwoReg);
  while(Serial1.available() < 4);          // Wait for 4 bytes,  encoder 2 value
  result2 = Serial1.read();
  result2 <<= 8;
  result2 += Serial1.read();
  result2 <<= 8;
  result2 += Serial1.read();
  result2 <<= 8;
  result2 += Serial1.read();
  delay(5);                                
  return result2; 
}


// Sets
void MD25_UART::resetEncoders()   // reset du nombre de ticks des 2 encodeurs
{
   Serial1.write(CmdReg);
   Serial1.write(ResetEncodersReg);                  
}

void MD25_UART::enableSpeedRegulation()   // active la régulation PID
{
   Serial1.write(CmdReg);
   Serial1.write(EnableRegulReg);
}

void MD25_UART::disableSpeedRegulation()
{
   Serial1.write(CmdReg);
   Serial1.write(DisableRegulReg);
}

void MD25_UART::enableTimeout()   // Désactive l'output des moteurs après 2 secondes sans communication
{
   Serial1.write(CmdReg);
   Serial1.write(EnableTimeOutReg);
}

void MD25_UART::disableTimeout()
{
   Serial1.write(CmdReg);
   Serial1.write(DisableTimeOutReg);
}

void MD25_UART::setMotor1Speed(byte Speed1)
{
   Serial1.write(CmdReg);
   Serial1.write(Speed1Reg);
   Serial1.write(Speed1); 
}

void MD25_UART::setMotor2Speed(byte Speed2)
{
   // Par défaut pour Modes 0 ou 1
   Serial1.write(CmdReg);
   Serial1.write(Speed2_TurnReg);
   Serial1.write(Speed2);// Vitesse du  moteur 2
}

void MD25_UART::setMotorsSpeed(byte Speeds, byte Turn) 
{
   Serial1.write(CmdReg);
   Serial1.write(Speed1Reg);
   Serial1.write(Speeds);// Vitesses des 2 moteurs
   Serial1.write(CmdReg);
   Serial1.write(Speed2_TurnReg);
   Serial1.write(Turn); // valeur du turn
 }

void MD25_UART::stopMotor1()
{
  switch(this->mode)
  {
   case 0: setMotor1Speed(128); // valeur selon mode voir documentation
            break;
   case 1 : setMotor1Speed(0);
            break;
   case 2: setMotor1Speed(128); // valeur selon mode voir documentation
           setMotor2Speed(128); //Turn=128
            break;
   case 3 : setMotor1Speed(0);
            setMotor2Speed(0); //Turn=0
            break;
   default: break;
  }
}

void MD25_UART::stopMotor2()
{
   switch(this->mode)
  {
   case 0: setMotor2Speed(128); // valeur selon mode voir documentation
            break;
   case 1 : setMotor2Speed(0);
            break;
   case 2: setMotor1Speed(128); // valeur selon mode voir documentation
           setMotor2Speed(128); //Turn=128
            break;
   case 3 : setMotor1Speed(0);
            setMotor2Speed(0); //Turn=0
            break;
   default: break;
  }
}

void MD25_UART::stopMotors()
{
   stopMotor1();
   stopMotor2();
}

void MD25_UART::setMode(byte Mode) //default Mode=0
{
  this->mode = Mode;
  Serial1.write(CmdReg);
  Serial1.write(ModeReg);                   
  Serial1.write(Mode);
}

void MD25_UART::setAccelerationRate(byte Rate) //default Rate=5
{
   Serial1.write(CmdReg);
   Serial1.write(AccRateReg);                  // Set motors acceleration rate
   Serial1.write(Rate);                        // La valeur doit être de 1, 2, 3, 5 ou 10 (5 étant la valeur par défaut)
}


/*
* Private Methods
*/




/*long MD25_UART::getEncoders() // 1 ou 2
{
  long result1 = 0; 
  long result2 = 0;
  Serial1.write(CmdReg);
  Serial1.write(EncoderOneReg);
  while(Serial1.available() < 8);          // Wait for 8 bytes, first 4 encoder 1 values second 4 encoder 2 values 
  result1 = Serial1.read();                 // First byte for encoder 1, HH.
  result1 <<= 8;
  result1 += Serial1.read();                // Second byte for encoder 1, HL
  result1 <<= 8;
  result1 += Serial1.read();                // Third byte for encoder 1, LH
  result1 <<= 8;
  result1  += Serial1.read();               // Fourth byte for encoder 1, LL
  Serial1.write(CmdReg);
  Serial1.write(EncoderTwoReg);
  while(Serial1.available() < 8);          // Wait for 8 bytes, first 4 encoder 1 values second 4 encoder 2 values
  result2 = Serial1.read();
  result2 <<= 8;
  result2 += Serial1.read();
  result2 <<= 8;
  result2 += Serial1.read();
  result2 <<= 8;
  result2 += Serial1.read();
  delay(5);                                
  return result1; 
}*/
