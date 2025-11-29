/* 
 * File:   main.c
 * Author: E306-PC5
 *
 * Created on October 6, 2025, 9:24 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "ADC.h"
#include "main.h"

unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;

int main(void) {
    /***********************************************************************************************/
    //Initialisation oscillateur
    InitOscillator();
    /***********************************************************************************************/
    // Configuration des input et output (IO)
    InitIO();
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    InitTimer1();
    InitTimer23();
    InitTimer4();
    InitPWM();
    InitADC1();

    //    PWMSetSpeed(10,MOTEUR_DROIT);
    //    PWMSetSpeed(10,MOTEUR_GAUCHE);
    //PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
    //PWMSetSpeedConsigne(20, MOTEUR_DROIT);
    //PWMUpdateSpeed();
    /***********************************************************************************************/
    // Boucle Principale

    while (1) {
        
       if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
           
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 3.3;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 3.3;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 3.3;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreexDroit = 34 / volts - 3.3;
            volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreexGauche = 34 / volts - 3.3;
            
            if (robotState.distanceTelemetreCentre >= 30) {
                LED_ORANGE_1 = 0;
            } else {
                LED_ORANGE_1 = 1;
            }
            if (robotState.distanceTelemetreGauche >= 30) {
                LED_BLEUE_1 = 0;
            } else {
                LED_BLEUE_1 = 1;
            }

            if (robotState.distanceTelemetreexGauche >= 30) {
                LED_BLANCHE_1 = 0;
            }
            else {
                LED_BLANCHE_1 = 1;
            }
            if (robotState.distanceTelemetreDroit >= 30) {
                LED_ROUGE_1 = 0;
            }
            else {
                LED_ROUGE_1 = 1;
            }
            if (robotState.distanceTelemetreexDroit >= 30) {
                LED_VERTE_1 = 0;
            }
            else {
                LED_VERTE_1 = 1;
            }

        }
        //        LED_BLANCHE_1 = !LED_BLANCHE_1;
        //        LED_BLEUE_1 = !LED_BLEUE_1;
        //        LED_ORANGE_1 = !LED_ORANGE_1;

    } // fin main
}

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
            break;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 2000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            PWMUpdateSpeed();
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            if (timestamp > 1000)
                SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            if (timestamp > 1000)    
                SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            if (timestamp > 1000)
                SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            if (timestamp > 1000)
                SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            if (timestamp > 1000)    
                SetNextRobotStateInAutomaticMode();
            break;
        case STATE_ATTENTE_TOURNE:
            timestamp=0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            PWMUpdateSpeed();
            stateRobot = STATE_ATTENTE_TOURNE_EN_COURS;
            break;
        case STATE_ATTENTE_TOURNE_EN_COURS:
            if (timestamp > 1000)
                stateRobot =STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case STATE_RECULE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            PWMUpdateSpeed();
            stateRobot = STATE_RECULE_EN_COURS;
            break;
        case STATE_RECULE_EN_COURS:
            if (timestamp > 1000)
                SetNextRobotStateInAutomaticMode();
            break;
        case STATE_DEMI_TOUR:    
                PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            PWMUpdateSpeed();
            stateRobot = STATE_DEMI_TOUR_EN_COURS;
            break;
        case STATE_DEMI_TOUR_EN_COURS:
            if (timestamp > 1000)
                stateRobot=STATE_TOURNE_SUR_PLACE_DROITE;
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    
    unsigned char sensorState = 0;
    if(robotState.distanceTelemetreexGauche < 30)
        sensorState |= 0b10000;
    if(robotState.distanceTelemetreGauche < 30)
        sensorState |= 0b01000;
    if (robotState.distanceTelemetreCentre < 30)
        sensorState |=0b00100;
    if (robotState.distanceTelemetreDroit < 30)
        sensorState |=0b00010;
    if (robotState.distanceTelemetreexDroit < 30)
        sensorState |=0b00001;
    
    
    
    switch(sensorState)
    {    
    
      case 0b00000:
            nextStateRobot = STATE_AVANCE;
        break;
        case 0b00001:
            nextStateRobot = STATE_TOURNE_GAUCHE;
        break;
        case 0b00010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
        break; 
        case 0b00100:
            nextStateRobot = STATE_RECULE ;
        break; 
        case 0b01000:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b10000:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b00011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
        break;
        case 0b00101:
            nextStateRobot = STATE_TOURNE_GAUCHE;
        break;
        case 0b01001:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b10001:
            nextStateRobot = STATE_AVANCE;
        break;
        case 0b00110:
            nextStateRobot = STATE_TOURNE_GAUCHE;
        break;
        case 0b01010:
            nextStateRobot = STATE_AVANCE;
        break;
        case 0b10010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
        break;
        case 0b01100:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b10100:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b11000:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b11100:
            nextStateRobot = STATE_TOURNE_DROITE;
        break;
        case 0b11010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
        break;
        case 0b11001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00111:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b01110:
            nextStateRobot=STATE_DEMI_TOUR;
            break;
        case 0b11110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11011:
            nextStateRobot = STATE_RECULE;
            break;
        case 0b10111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11111:
            nextStateRobot = STATE_DEMI_TOUR;
            break;
        
    }
            
    
//    unsigned char positionObstacle = PAS_D_OBSTACLE;
//    //éDtermination de la position des obstacles en fonction des ééètlmtres
//    if (robotState.distanceTelemetreDroit < 30 || 
//            robotState.distanceTelemetreexDroit < 30 &&
//            robotState.distanceTelemetreCentre > 30 &&
//            robotState.distanceTelemetreGauche > 30) //Obstacle àdroite
//        positionObstacle = OBSTACLE_A_DROITE;
//    else if (robotState.distanceTelemetreDroit > 30 && 
//            robotState.distanceTelemetreexDroit < 30 &&
//            robotState.distanceTelemetreCentre < 30 &&
//            robotState.distanceTelemetreGauche > 30) //Obstacle à droite et au milieu
//        positionObstacle = OBSTACLE_A_DROITE;
//    else if (robotState.distanceTelemetreDroit > 30 &&
//            robotState.distanceTelemetreCentre > 30 &&
//            robotState.distanceTelemetreGauche < 30||
//            robotState.distanceTelemetreexGauche < 30) //Obstacle àgauche
//        positionObstacle = OBSTACLE_A_GAUCHE;
//      else if (robotState.distanceTelemetreDroit > 30 &&
//            robotState.distanceTelemetreCentre < 30 &&
//            robotState.distanceTelemetreGauche > 30||
//            robotState.distanceTelemetreexGauche < 30) //Obstacle àgauche
//        positionObstacle = OBSTACLE_A_GAUCHE;
//    else if (robotState.distanceTelemetreCentre < 30){ //Obstacle en face
//        positionObstacle = OBSTACLE_EN_FACE;
//    }
//    else if (robotState.distanceTelemetreDroit > 30 &&
//            robotState.distanceTelemetreCentre > 30 &&
//            robotState.distanceTelemetreGauche > 30 &&
//            robotState.distanceTelemetreexGauche > 30 &&
//            robotState.distanceTelemetreexDroit > 30) //pas d?obstacle
//        positionObstacle = PAS_D_OBSTACLE;
//    //éDtermination de lé?tat àvenir du robot
//    if (positionObstacle == PAS_D_OBSTACLE)
//        nextStateRobot = STATE_AVANCE;
//    else if (positionObstacle == OBSTACLE_A_DROITE)
//        nextStateRobot = STATE_TOURNE_GAUCHE;
//    else if (positionObstacle == OBSTACLE_A_GAUCHE)
//        nextStateRobot = STATE_TOURNE_DROITE;
//    else if (positionObstacle == OBSTACLE_EN_FACE)
//        nextStateRobot = STATE_ATTENTE;
    //Si l?on n?est pas dans la transition de lé?tape en cours
    if (nextStateRobot != stateRobot - 1)
       stateRobot = nextStateRobot;
}
