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
