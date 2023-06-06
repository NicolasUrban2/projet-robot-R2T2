/* Librairie dédiée à la communication série asynchronre (UART RS232)
de la nouvelle génération de carte MD25.
La vitesse ici sera de 38400baud(2 cavaliers mis en place) avec 1 start bit, 2 stop bits, no parity.
L'utilsation de la MEGA2560 permet de dédier Serial1 à la carte.



// Si la valeur 0x00 du registre de commande  risque d'être interprétée comm NULL on la castera  
en byte.(byte)0x00.

//On conserve les noms des registres de la documentation.
// Ici les registres essentiels. On peut compléter...
/*#define CMD               0x00              // Registre principal qui doit être appellé avant tous les autres Specifique à la commande "Serial1"
#define Set_Speed1          0x31                    // Registre pour la Vitesse du moteur1
#define Set_Speed2_Turn     0x32					// Registre pour la Vitesse du moteur2 ou valeur du Turn selon Mode
#define Set_Acceleration    0x33                   //  Registre pour l'accélération
#define Reset_Encoders      0x35                    // Registre pour effectuer le Reset des encodeurs
#define Set_Mode            0x34                    // Registre des Modes (0 défaut, 1,2 et 3) 
#define Get_Encoder1        0x23 					// Registre de Lecture de l'encodeur_1.
#define Get_Encoder2        0x24 					// Registre de Lecture de l'encodeur_2.
#define Get_Encoders        0x25                    // Registre de Lecture des 2 encodeurs.
#define Enable_Regulation   0x36 					// Registre Valider la régulation de vistesse.
#define Disable_Regulation  0x37   					// Registre Valider la régulation de vistesse.
#define Enable_TimeOut		  0x39*/

#ifndef MD25_UART_h
#define MD25_UART_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MD25_UART
{
  public:
  	/* constructor*/
    MD25_UART(); 
    /* others functions*/
    void stopMotor1();
    void stopMotor2();
    void stopMotors();
    void enableSpeedRegulation();
    void disableSpeedRegulation();
    void enableTimeout();
    void disableTimeout();
    void resetEncoders();
    /* get functions*/
    long  getEncoder1();
    long  getEncoder2();
    //long  getEncoders();

     /*set functions*/
    void setMotorsSpeed(byte speed, byte turn) ;
    void setMotor1Speed(byte Speed);
    void setMotor2Speed(byte Speed);
    void setMode(byte Mode);
    void setAccelerationRate(byte Rate);
    
  private:
    
    long getEncoders(byte EncReg);

    byte mode;  // Le mode de vitesse (0, 1, 2 ou 3)

    static byte const CmdReg		  		  = 0x00;  // command register
    static byte const Speed1Reg		  		= 0x31;  // speed to first motor
    static byte const Speed2_TurnReg  	= 0x32;  // speed to second motor
    static byte const EncoderOneReg	 		= 0x23;  // motor encoder 1 (first byte)
    static byte const EncoderTwoReg	  	= 0x24;  // motor encoder 2 (first byte)
    //static byte const EncodersReg       = 0x25;
    static byte const DisableRegulReg   = 0x36;  // Power output not regulated by encoder feedback
    static byte const EnableRegulReg   	= 0x37;  // Power output regulated by encoder feedback
    static byte const DisableTimeOutReg = 0x38;  // MD25 will continuously output with no regular command
    static byte const EnableTimeOutReg  = 0x39;  // MD25 MD25 will stop after 2 seconds without communication
    static byte const AccRateReg			  = 0x33;  // acceleration rate
    static byte const ModeReg				    = 0x34;  // mode of operation
    static byte const ResetEncodersReg	= 0x35;  // reset des 2 encodeurs
};

#endif
