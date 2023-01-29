bool FLAG_FIN = 0;
bool STATUS_INI = 1;

//FUNCTIONS TO DEAL WITH 

void INITIALIZATION(){

  if(getPhaseFaultCondition()){
    STATE = 2; // STOP CONDITION;
    lcdDisplayMessage("ERRO SENSOR", "FALTA FASE");
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

    if(getFinEndSensorNegativeStatus()){
      setStopLed(!digitalRead(ledStopPin));
    }
  
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
  
  if(getStartButtonStatus() && (FLAG_FIN==1) && (getFinEndSensorNegativeStatus())){
      STATE = 1; //CHANGE TO 1!!!!  
      FLAG_FIN = 0;
      setStartLed(1);
      STATUS_INI = 0;
  }

  if(getStopButtonStatus()){
      setMotorFins(0);
      STATE = 2;     
  }


}


void CONTROL(){

  // phase fault condition;
  if(getPhaseFaultCondition()){
    STATE = 2; // STOP CONDITION;
    lcdDisplayMessage("ERRO SENSOR", "FALTA FASE");
  }

  // button stop watching
  if(getStopButtonStatus()){
      STATE = 2;    
  }

  if(!getFinEndSensorNegativeStatus()){ //se ATIVADO
      // primeiro verifica se ja foi DESATIVADO;
    if(FLAG_FIN){
      STATE = 2;
      FLAG_FIN = 0;
    }
  }
  if(getFinEndSensorNegativeStatus()){  //se DESativado
    FLAG_FIN = 1;
  } 


  if(!getFinEndSensorPositiveStatus()){
      STATE = 2;
  }


  ///0.5s time slot ===== CONTROL =====
  if ((millis()-lastTimeMillis0s5) > time0s5){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s5 = millis();
    
    CONTROL_LOGIC_DZ();

    lcdDisplaySpeed(controlActualSpeed);
   
  }

}

bool FLAG_STOP_MESSAGE = 0;
void STOP(){

    setStartLed(0);
    setMotorFins(0);
    setStopLed(1);

/// SANDBOX

  if(!getFinEndSensorNegativeStatus()){ // AO ACIONAR O SENSOR...
    setMotorFins(0);
    STATUS_INI = 0;

    //setStopLed(0);
    //FLAG_FIN = 1;
  }
  // START CONTROL ROUTINE MOTOR FIN

  if(getFinEndSensorNegativeStatus()){ // SENSOR NAO PRESSIONADO
    // OLD
    setMotorFins(-1);
    // FLAG_FIN = 1;
  }
  


/// SANBOX

///0.5s time slot
  if ((millis()-lastTimeMillis3s0) > time3s0){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis3s0 = millis();
    if(FLAG_STOP_MESSAGE == 0){
      FLAG_STOP_MESSAGE = 1;
      lcdDisplayMessage("STOP", "MODE");
    }else{
      FLAG_STOP_MESSAGE = 0;
      lcdDisplayMessage("PRESSIONE START", "PARA RECOMECAR");
    }
    

  }

// so deixar apertar start se o a falta de fase estiver desacionada;
if (!getPhaseFaultCondition()){
  if(getStartButtonStatus()){
    STATE = 0;
  
}
  }    

}


void CONTROL_LOGIC_DZ(){

    controlActualSpeed = getActualSpeed();
    controlReferenceSpeed = getSpeedReference();
    controlError = controlReferenceSpeed - controlActualSpeed;

    
    Serial.print("erro   =");Serial.println(controlError);
    
    if( (controlError > controlHist) ){
            controlLastState = 1;
            setMotorFins(1);
        }

    if( (controlError < -1*controlHist) ){
            controlLastState = -1;
            setMotorFins(-1);
        }
    
    if( (controlError > -1*controlHistDead) && (controlError < controlHistDead) ){
        controlLastState = 0;
        setMotorFins(0);     
        //Serial.println("entrou dz");       
    }

}

void CONTROL_LOGIC(){

    controlActualSpeed = getActualSpeed();
    controlReferenceSpeed = getSpeedReference();
    controlError = controlReferenceSpeed - controlActualSpeed;

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