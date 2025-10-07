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
    InitPWM();
    InitADC1();
    
    //    PWMSetSpeed(10,MOTEUR_DROIT);
    //    PWMSetSpeed(10,MOTEUR_GAUCHE);
    PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
    PWMSetSpeedConsigne(20, MOTEUR_DROIT);
    PWMUpdateSpeed();
    /***********************************************************************************************/
    // Boucle Principale
    while (1) {
        if (ADCIsConversionFinished()) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            ADCValue0 = result[0];
            ADCValue1 = result[1];
            ADCValue2 = result[2];
        }
        //        LED_BLANCHE_1 = !LED_BLANCHE_1;
        //        LED_BLEUE_1 = !LED_BLEUE_1;
        //        LED_ORANGE_1 = !LED_ORANGE_1;

    } // fin main
}
