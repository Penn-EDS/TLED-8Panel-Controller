void ChannelsOFF(){
  for (int pwmnum=0; pwmnum < 16; pwmnum++) {
      pwm1.setPWM(pwmnum, 0, 0);
      pwm2.setPWM(pwmnum, 0, 0);
    }
}

void ChannelsON(){
  for (int pwmnum=0; pwmnum < 16; pwmnum++) {
      pwm1.setPWM(pwmnum, 0, 4095);
      pwm2.setPWM(pwmnum, 0, 4095);
    }
}


void clearparameters(){
  for(int x=0; x<100; x++){
    parameters[x]=0;
  }
//  for(int g=0; g<66; g++){
//      //Serial.println(recvchars[g]);
//       Serial.println((String)"g:"+g+" "+parameters[g]);
//      // Serial.println((String)"p:"+p+","+PLDinfo[p]);
//       delay(20);
//      }
}


void clearrecvchars(){
  for(int x=0; x<32; x++){
    recvchars[x]=0;
  }
}

void recvdata(){
  newdata=false;
  static boolean recvinprogress=false;
  static byte ndx=0;
  char startmarker='<';
  char comma=',';
  char endmarker='>';
  char c;
  clearrecvchars();
  while(1){

  while(Serial.available()>0 && newdata==false){
    c=Serial.read();

    if(c==startmarker){
    recvinprogress=true;
    }

    if(c==comma && recvinprogress==true){
      //Serial.println(recvchars);
      parameters[p]=atol(recvchars);
      //delay(100);
      
      ndx=0;
      clearrecvchars();
      p++;
    }

    if(c==endmarker && recvinprogress==true){
      parameters[p]=atol(recvchars);
      recvinprogress=false;
      ndx=0;
      //p=0;
      newdata=true;

//      for(int g=0; g<(p+1); g++){
//      //Serial.println(recvchars[g]);
//       Serial.println((String)"g:"+g+" "+parameters[g]);
//      // Serial.println((String)"p:"+p+","+PLDinfo[p]);
//       delay(20);
//      }
     p=0;

     goto OUT;
    }
    
    if(recvinprogress==true && c!=comma && c!=startmarker && c!=']'){
      recvchars[ndx]=c;
      ndx++;  
    }
    
    
  }
  }
OUT:delay(1);
}
