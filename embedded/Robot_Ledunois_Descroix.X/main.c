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
    PWMSetSpeed(30);
    /***********************************************************************************************/
    // Boucle Principale
    while (1) {
        LED_BLANCHE_1 = !LED_BLANCHE_1;
        LED_BLEUE_1 = !LED_BLEUE_1;
        LED_ORANGE_1 = !LED_ORANGE_1;
    } // fin main
}
