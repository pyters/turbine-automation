bool FLAG_FIN = 0;
<<<<<<< Updated upstream
// bool STATUS_INI = 1;

int controlCountContactor = 0;
int COUNT_STABLE_CONTACTOR = 5;
bool STABLE_CONTROL_CONTACTOR_FLAG = 0;

bool STOP_BUTTON_FLAG = 0;
bool STOP_PHASE_FAULT_FLAG = 0;
bool STOP_FIN_POSITIVE_FLAG = 0;
bool STOP_FIN_NEGATIVE_FLAG = 0;

bool STOP_MODE_FINISHED_FLAG = 0;

bool MESSAGE_SWAP_FLAG = 0;

=======
>>>>>>> Stashed changes

//FUNCTIONS TO DEAL WITH 

void INITIALIZATION(){

  if(getPhaseFaultCondition()){
    STATE = 2; // STOP CONDITION;
    STOP_MODE_FINISHED_FLAG = 1;
    STOP_PHASE_FAULT_FLAG = 1;

    //lcdDisplayMessage("ERRO SENSOR", "FALTA FASE");
  }

  // Button STOP
  if(getStopButtonStatus()){
      // setMotorFins(0);
      STATE = 2;
      // FLAG_STOP_MESSAGE = 1;
      STOP_BUTTON_FLAG = 1;
      STOP_MODE_FINISHED_FLAG = 1;     
  }
  

  ///1s time slot
  if ((millis()-lastTimeMillis1s0) > time1s0){
    
    //things that have a 1s period;
    lastTimeMillis1s0 = millis();
    setStartLed(!digitalRead(ledStartPin));

  }

  ///0.5s time slot
  if ((millis()-lastTimeMillis0s5) > time0s5){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s5 = millis();

    //actualSpeed = getActualSpeed();
    lcdDisplaySpeed(getActualSpeed());
    //Serial.println(STATE);


    setStopLed(!digitalRead(ledStopPin));
    
  }



  if(!getFinEndSensorNegativeStatus()){ // AO ACIONAR O SENSOR...
    setMotorFins(0);
    //FLAG_FIN = 1;
  }

  if(getFinEndSensorNegativeStatus()){ // SENSOR NAO PRESSIONADO
    setMotorFins(-1);
    //FLAG_FIN = 1;
  }
<<<<<<< Updated upstream


  // condition to start the control. button START + fin negative sensor DESACTIVATED + 
  if(getStartButtonStatus() && (FLAG_FIN==1) && (getFinEndSensorNegativeStatus())){
      STATE = 1; //CHANGE TO 1!!!!  
      FLAG_FIN = 0;
      // setStartLed(1);
      // STATUS_INI = 0;
=======
  
  
  if(getStartButtonStatus() && !getFinEndSensorNegativeStatus()){
      STATE = 1; //CHANGE TO 1!!!!    
      setStartLed(1);
>>>>>>> Stashed changes
  }




}


void CONTROL(){

  // phase fault condition;
  if(getPhaseFaultCondition()){
    STATE = 2; // STOP CONDITION;
    STOP_PHASE_FAULT_FLAG = 1;
    STOP_MODE_FINISHED_FLAG = 1; 

    // lcdDisplayMessage("ERRO SENSOR", "FALTA FASE");
  }

  // button stop watching
  if(getStopButtonStatus()){
      STATE = 2;
      STOP_MODE_FINISHED_FLAG = 1; 
      STOP_BUTTON_FLAG = 1;   
  }

  // if(!getFinEndSensorNegativeStatus()){ //se ATIVADO
  //     // primeiro verifica se ja foi DESATIVADO;
  //   if(FLAG_FIN){
  //     STATE = 2;
  //     FLAG_FIN = 0;
  //   }
  // }
  // if(getFinEndSensorNegativeStatus()){  //se DESativado
  //   FLAG_FIN = 1;
  // } 
  
  if(!getFinEndSensorNegativeStatus()){
      STATE = 2;
  }

  if(!getFinEndSensorPositiveStatus()){
      STATE = 2;
  }


  // ===== CONTROL =====
  ///0.5s time slot 
  if ((millis()-lastTimeMillis0s5) > time0s5){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s5 = millis();
    
    CONTROL_LOGIC();

<<<<<<< Updated upstream
    lcdDisplaySpeed(controlActualSpeed);  
   
=======
    lcdDisplaySpeed(controlActualSpeed);
    Serial.println(FLAG_FIN);

>>>>>>> Stashed changes
  }
  
  
  //0.3s time slot
  if ((millis()-lastTimeMillis0s3) > time0s3){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s3 = millis();

    // CONTACTOR 
    if(STABLE_CONTROL_CONTACTOR_FLAG){
      setStartLed(1);
      setContactor(1);

    }else{
      setStartLed(!digitalRead(ledStartPin));
      setContactor(0);

    }
  }

}


void STOP(){

    controlCountContactor = 0;
    STABLE_CONTROL_CONTACTOR_FLAG = 0;

    setMotorFins(0);
    setContactor(0);

    setStartLed(0);
    setStopLed(1);

<<<<<<< Updated upstream

  // RETURN THE FIN MOTOR TO HOME POSITION;
  if(!getFinEndSensorNegativeStatus()){ // AMOTOR FIM DE CURSO
    setMotorFins(0);
    // reseting flags
    STOP_MODE_FINISHED_FLAG = 0;
    // STOP_BUTTON_FLAG = 0;
    //STOP_PHASE_FAULT_FLAG = 0;
    
    // STATUS_INI = 0;

    //setStopLed(0);
    //FLAG_FIN = 1;
  }

  if(getFinEndSensorNegativeStatus()){ // SENSOR NAO PRESSIONADO
    // OLD
    setMotorFins(-1);
    // FLAG_FIN = 1;
  }
  


  // 3s time slot
  if ((millis()-lastTimeMillis2s0) > time2s0){

    lastTimeMillis2s0 = millis();

    if(STOP_BUTTON_FLAG){
    //   lcdDisplayMessage("== STOP MODE ==", "BOTAO STOP PRESS");

      if(MESSAGE_SWAP_FLAG){
        lcdDisplayMessage("== STOP MODE ==", "BOTAO STOP PRESS");
        MESSAGE_SWAP_FLAG = 0;
      }else{
        lcdDisplayMessage("== STOP MODE ==", "PRESSIONE START");
        MESSAGE_SWAP_FLAG = 1;
      }
      
    }

    if(STOP_PHASE_FAULT_FLAG){
      // lcdDisplayMessage("== STOP MODE ==", "FALTA DE FASE");
      if(MESSAGE_SWAP_FLAG){
        lcdDisplayMessage("== STOP MODE ==", "FALTA DE FALSE");
        MESSAGE_SWAP_FLAG = 0;
      }else{
        lcdDisplayMessage("== STOP MODE ==", "PRESSIONE START");
        MESSAGE_SWAP_FLAG = 1;
      }
    }
  }

    // if(!STOP_MODE_FINISHED_FLAG){
    //   lcdDisplayMessage("== STOP MODE ==", "PRESSIONE START");
    // }

// so deixar apertar start se o a falta de fase estiver desacionada;
  if(getStartButtonStatus()){
    if (!STOP_MODE_FINISHED_FLAG && !getPhaseFaultCondition()){
      STATE = 0;
      STOP_BUTTON_FLAG = 0;
      STOP_PHASE_FAULT_FLAG = 0;
    }
=======
///0.5s time slot
  if ((millis()-lastTimeMillis0s5) > time0s5){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s5 = millis();
    lcdDisplayMessage("STOP", "MODE");

>>>>>>> Stashed changes
  }    

}


<<<<<<< Updated upstream

void CONTROL_LOGIC_DZ(){
=======
void CONTROL_LOGIC(){
>>>>>>> Stashed changes

    controlActualSpeed = getActualSpeed();
    controlReferenceSpeed = getSpeedReference();
    controlError = controlReferenceSpeed - controlActualSpeed;

<<<<<<< Updated upstream
    
    // Serial.print("erro   =");Serial.println(controlError);
    
    if( (controlError > controlHist) ){
            controlLastState = 1;
            setMotorFins(1);

            controlCountContactor = 0;
            STABLE_CONTROL_CONTACTOR_FLAG = 0;
            
        }

    if( (controlError < -1*controlHist) ){
            controlLastState = -1;
            setMotorFins(-1);
            
            controlCountContactor = 0;
            STABLE_CONTROL_CONTACTOR_FLAG = 0;
        }
    
    if( (controlError > -1*controlHistDead) && (controlError < controlHistDead) ){
        controlLastState = 0;
        setMotorFins(0);
        if(controlCountContactor < COUNT_STABLE_CONTACTOR){
          controlCountContactor = controlCountContactor + 1;
        }else {
          STABLE_CONTROL_CONTACTOR_FLAG = 1; // ready to contactor;
        }     
        //Serial.println("entrou dz");       
    }

}

// void CONTROL_LOGIC(){

//     controlActualSpeed = getActualSpeed();
//     controlReferenceSpeed = getSpeedReference();
//     controlError = controlReferenceSpeed - controlActualSpeed;

//     if((controlError > controlHist)){
//         if(controlLastState != 1){
//             Serial.println("entrou");
//             controlLastState = 1;
//             //setMotorFins(0); delay(100);
//             setMotorFins(1);
//         }
//     }
//     if((controlError < controlHist)){
//         if(controlLastState != -1){
//             controlLastState = -1;
//             //setMotorFins(0); delay(100);
//             setMotorFins(-1);
//         }
//     }  

// }
=======
    if((controlError > controlHist)){
        if(controlLastState != 1){
            Serial.println("entrou");
            controlLastState = 1;
            //setMotorFins(0); delay(100);
            setMotorFins(1);
        }
    }
    if((controlError < controlHist)){
        if(controlLastState != -1){
            controlLastState = -1;
            //setMotorFins(0); delay(100);
            setMotorFins(-1);
        }
    }  

}
>>>>>>> Stashed changes
