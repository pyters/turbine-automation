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
    actualSpeed = getActualSpeed();
    lcdDisplaySpeed(actualSpeed);
    //Serial.println(STATE);


    setStopLed(!digitalRead(ledStopPin));
    
  }



  if(!getFinEndSensorNegativeStatus()){ // AO ACIONAR O SENSOR...
    FLAG_FIN = 1;
    setMotorFins(1);

    // OLD
    //setMotorFins(0);
    //setStopLed(0);
    //FLAG_FIN = 1;
  }
  // START CONTROL ROUTINE MOTOR FIN

  if(getFinEndSensorNegativeStatus()){ // SENSOR NAO PRESSIONADO
    if(FLAG_FIN==0){
      setMotorFins(-1);
      FLAG_FIN==0;
    }
    if(FLAG_FIN==1){
      //FLAG_FIN = 0;
      setMotorFins(0);
      setStopLed(0);
    }
    // OLD
    // setMotorFins(-1);
    // FLAG_FIN = 1;
  }


  // condition to start by START BUTTON;
  if(getStartButtonStatus() && (FLAG_FIN==1) && getFinEndSensorNegativeStatus()){
      STATE = 1; //CHANGE TO 1!!!!  
      FLAG_FIN = 0;
      // setStartLed(1);
      // STATUS_INI = 0;
  }

  // condition to start BY PHASE FAULT AUTO RESTART 
  if(STOP_PHASE_FAULT_AUTO_RESTART_FLAG && (FLAG_FIN==1) && (getFinEndSensorNegativeStatus())){
      STATE = 1; //CHANGE TO 1!!!!  
      FLAG_FIN = 0;
      STOP_PHASE_FAULT_AUTO_RESTART_FLAG = 0;
      // setStartLed(1);
      // STATUS_INI = 0;
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
      STOP_FIN_FLAG = 1;
      STOP_MODE_FINISHED_FLAG = 1; 
      
  }

  if(!getFinEndSensorPositiveStatus()){
      STATE = 2;
      STOP_FIN_FLAG = 1;
      STOP_MODE_FINISHED_FLAG = 1; 
  }


  // ===== CONTROL =====
  ///0.5s time slot 
  if ((millis()-lastTimeMillis0s5) > time0s5){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s5 = millis();
    
    CONTROL_LOGIC_DZ();

    lcdDisplaySpeed(controlActualSpeed);  
   
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
    // setStopLed(1);


  // RETURN THE FIN MOTOR TO HOME POSITION;
  if(!getFinEndSensorNegativeStatus()){ // AMOTOR FIM DE CURSO
    
    setMotorFins(0);
    STOP_MODE_FINISHED_FLAG = 0;

  }

  if(getFinEndSensorNegativeStatus()){ // SENSOR NAO PRESSIONADO
    // OLD
    setMotorFins(-1);
    // FLAG_FIN = 1;
  }
  


  // 2s time slot
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
        lcdDisplayMessage("== STOP MODE ==", "ESPERA RETORNO");
        MESSAGE_SWAP_FLAG = 1;
      }
    }

    if(STOP_FIN_FLAG){
      // lcdDisplayMessage("== STOP MODE ==", "FALTA DE FASE");
      if(MESSAGE_SWAP_FLAG){
        lcdDisplayMessage("== STOP MODE ==", "FIM DE CURSO FIN");
        MESSAGE_SWAP_FLAG = 0;
      }else{
        lcdDisplayMessage("== STOP MODE ==", "PRESSIONE START");
        MESSAGE_SWAP_FLAG = 1;
      }
    }
  }

  // 0.5 slot time for the stop LED blink
  if ((millis()-lastTimeMillis0s5) > time0s5){
  lastTimeMillis0s5 = millis();

    if(STOP_MODE_FINISHED_FLAG){
      setStopLed(!digitalRead(ledStopPin)); 
    }else{
      setStopLed(1);
    }
    
  }

  // 10s slot time for auto check for phase fault
  if ((millis()-lastTimeMillis10s0) > 2000){
    lastTimeMillis10s0 = millis();
    if (!STOP_MODE_FINISHED_FLAG && !getPhaseFaultCondition()){ // se a rotina terminou e nao tem mais phase condition
       STOP_PHASE_FAULT_AUTO_RESTART_FLAG = 1;
    }
    Serial.println(STOP_PHASE_FAULT_AUTO_RESTART_FLAG);
    
  }



// so deixar apertar start se o a falta de fase estiver desacionada;
  if(getStartButtonStatus()){
    if (!STOP_MODE_FINISHED_FLAG && !getPhaseFaultCondition()){
      STATE = 0;
      STOP_BUTTON_FLAG = 0;
      STOP_PHASE_FAULT_FLAG = 0;
      STOP_FIN_FLAG = 0;
      STOP_PHASE_FAULT_AUTO_RESTART_FLAG = 0;
    }
  }    


  if (STOP_PHASE_FAULT_AUTO_RESTART_FLAG && !STOP_BUTTON_FLAG && !STOP_FIN_FLAG){
      STATE = 0;
      STOP_BUTTON_FLAG = 0;
      STOP_PHASE_FAULT_FLAG = 0;
      STOP_FIN_FLAG = 0;
  }

}



void CONTROL_LOGIC_DZ(){

    controlActualSpeed = getActualSpeed();
    controlReferenceSpeed = getSpeedReference();
    controlError = controlReferenceSpeed - controlActualSpeed;

    
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