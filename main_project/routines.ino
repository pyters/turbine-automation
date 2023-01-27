bool FLAG_FIN = 0;

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
    lcdDisplaySpeed(getActualSpeed());
    //Serial.println(STATE);

    if(getFinEndSensorNegativeStatus()){
      setStopLed(!digitalRead(ledStopPin));
    }
  
  }



  if(!getFinEndSensorNegativeStatus()){ // AO ACIONAR O SENSOR...
    setMotorFins(0);
    //FLAG_FIN = 1;
  }

  if(getFinEndSensorNegativeStatus()){ // SENSOR NAO PRESSIONADO
    setMotorFins(-1);
    //FLAG_FIN = 1;
  }
  
  
  if(getStartButtonStatus() && !getFinEndSensorNegativeStatus()){
      STATE = 1; //CHANGE TO 1!!!!    
      setStartLed(1);
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
    
    CONTROL_LOGIC();

    lcdDisplaySpeed(controlActualSpeed);
    Serial.println(FLAG_FIN);

  }

}


void STOP(){

    setStartLed(0);
    setMotorFins(0);
    setStopLed(1);

///0.5s time slot
  if ((millis()-lastTimeMillis0s5) > time0s5){
    //Serial.print("DT 0.5s = "); Serial.println(millis()-lastTimeMillis0s5);  
    lastTimeMillis0s5 = millis();
    lcdDisplayMessage("STOP", "MODE");

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