/*************************************************** 
  Version 2: start and end delay for each Box.
  This is firmware to control 8 LED panel controller.
  Each panel have 3 channels. 

  by Alexander Santos Lozada
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40, Wire);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);



//PWM1(0x40) contains Box 1 and Box 2
#define REDB1P1   5
#define BLUEB1P1  4
#define UVB1P1    3
#define REDB1P2   1
#define BLUEB1P2  2
#define UVB1P2    0
 
#define REDB2P1   10
#define BLUEB2P1  11
#define UVB2P1    9
#define REDB2P2   7
#define BLUEB2P2  8
#define UVB2P2    6
   
//PWM2(0x41) contains Box 3 and Box 4
#define REDB3P1   1
#define BLUEB3P1  2
#define UVB3P1    0
#define REDB3P2   4
#define BLUEB3P2  5
#define UVB3P2    3
    
#define REDB4P1   7
#define BLUEB4P1  8
#define UVB4P1    6
#define REDB4P2   10
#define BLUEB4P2  11
#define UVB4P2    9

// 
// ** CPU running at 84Mhz
// This is to compensate the 
// propagation delay in ms
//
#define PROPAGATION_DELAY_MS1 0
#define PROPAGATION_DELAY_MS2 4
#define T_OFFSET 100
#define TotalOffsetProdelay (T_OFFSET-PROPAGATION_DELAY_MS1)

char recvchars[32];
boolean newdata=false;
int p=0;

unsigned long SecTimeStart=0;
unsigned long OffSetTime=0;
 
unsigned long TimeOneB1P1S=TotalOffsetProdelay;
unsigned long TimeTwoB1P1S=0;
unsigned long TimeOneB1P2S=TotalOffsetProdelay;
unsigned long TimeTwoB1P2S=0;

unsigned long TimeOneB2P1S=TotalOffsetProdelay;
unsigned long TimeTwoB2P1S=0;
unsigned long TimeOneB2P2S=TotalOffsetProdelay;
unsigned long TimeTwoB2P2S=0;

unsigned long TimeOneB3P1S=TotalOffsetProdelay;
unsigned long TimeTwoB3P1S=0;
unsigned long TimeOneB3P2S=TotalOffsetProdelay;
unsigned long TimeTwoB3P2S=0;

unsigned long TimeOneB4P1S=TotalOffsetProdelay;
unsigned long TimeTwoB4P1S=0;
unsigned long TimeOneB4P2S=TotalOffsetProdelay;
unsigned long TimeTwoB4P2S=0;

int B1P1=1;
int B1P2=1;
int B2P1=1;
int B2P2=1;
int B3P1=1;
int B3P2=1;
int B4P1=1;
int B4P2=1;

/*
//Minimum value to TimeOneBxPx and TimeTwoBxPx is 5mS. Max frequency is 100Hz with a 10% of error. 

                                     
/* <Sessiontime(0),OffSetTime(1),TimeOneB1P1(2),TimeTwoB1P1(3),intensityTimeONER(4),intensityTimeTWOR(5),intensityTimeONEB(6),intensityTimeTWOB(7),intensityTimeONEU(8),intensityTimeTWOU(9),
 *                               TimeOneB1P2(10),TimeTwoB1P2(11),intensityTimeONER(12),intensityTimeTWOR(13),intensityTimeONEB(14),intensityTimeTWOB(15),intensityTimeONEU(16),intensityTimeTWOU(17),
 *                               TimeOneB2P1(18),TimeTwoB2P1(19),intensityTimeONER(20),intensityTimeTWOR(21),intensityTimeONEB(22),intensityTimeTWOB(23),intensityTimeONEU(24),intensityTimeTWOU(25),
 *                               TimeOneB2P2(26),TimeTwoB2P2(27),intensityTimeONER(28),intensityTimeTWOR(29),intensityTimeONEB(30),intensityTimeTWOB(31),intensityTimeONEU(32),intensityTimeTWOU(33),
 *                               TimeOneB3P1(34),TimeTwoB3P1(35),intensityTimeONER(36),intensityTimeTWOR(37),intensityTimeONEB(38),intensityTimeTWOB(39),intensityTimeONEU(40),intensityTimeTWOU(41),
 *                               TimeOneB3P2(42),TimeTwoB3P2(43),intensityTimeONER(44),intensityTimeTWOR(45),intensityTimeONEB(46),intensityTimeTWOB(47),intensityTimeONEU(48),intensityTimeTWOU(49),
 *                               TimeOneB4P1(50),TimeTwoB4P1(51),intensityTimeONER(52),intensityTimeTWOR(53),intensityTimeONEB(54),intensityTimeTWOB(55),intensityTimeONEU(56),intensityTimeTWOU(57),
 *                               TimeOneB4P2(58),TimeTwoB4P2(59),intensityTimeONER(60),intensityTimeTWOR(61),intensityTimeONEB(62),intensityTimeTWOB(63),intensityTimeONEU(64),intensityTimeTWOU(65)>
 */
 /*
 //In this new code you will the user will be capable to start the Boxes with a offset time. The offset time is the second value in the array.
 //If the user enter 2 seconds(2000 miliseconds) for the offsettime value then the first Box will start in time 0 and ending at Sessiontime, but the second Box will be starting at offsettime that is 2 seconds and ending in Sessiontime + offsettime.

  For a better inderstanding please see the table bellow:

 Panel  | TimeStart    | Timeending
  B1P1  |    0         | Sessiontime
  B1P2  |    0         | Sessiontime 
  B2P1  | OffSetTime   | Sessiontime + OffSetTime
  B2P2  | OffSetTime   | Sessiontime + OffSetTime
  B3P1  | OffSetTime*2 | Sessiontime + OffSetTime*2
  B3P2  | OffSetTime*2 | Sessiontime + OffSetTime*2
  B4P1  | OffSetTime*3 | Sessiontime + OffSetTime*3
  B4P2  | OffSetTime*3 | Sessiontime + OffSetTime*3

  If we define OffseTime as 3000(3 seconds) and Sessiontime as 10000(10 seconds) then we will see a table like this:
  
Panel  | TimeStart    | Timeending
  B1P1  |    0        | 10 sec
  B1P2  |    0        | 10 sec
  B2P1  |   3 sec     | 13 sec
  B2P2  |   3 Sec     | 13 sec
  B3P1  |   6 sec     | 16 sec
  B3P2  |   6 sec     | 16 sec
  B4P1  |   9 sec     | 19 sec
  B4P2  |   9 sec     | 19 sec

  
Example array: 
SessionTime is 10 sec and OffsetTime of 1 sec. With a ON/OFF time of 100 miliseconds and completely ON(4095) in TimeOne and completely OFF(0) in TimeTwo.
  <10000,1000,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0,100,100,4095,0,4095,0,4095,0>
*/
int parameters[67];

void setup() {
  Serial.begin(115200);
  //recvdata();

  //parameters[]
  pwm1.begin();
  pwm2.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm1.setOscillatorFrequency(25000000);
  pwm2.setOscillatorFrequency(25000000);
  pwm1.setPWMFreq(1600);  // This is the maximum PWM frequency (1600)
  pwm2.setPWMFreq(1600);
  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
  ChannelsOFF();
  ChannelsON();
  delay(1000);
  ChannelsOFF();
pinMode(47, OUTPUT);   //RED LED       
pinMode(46, OUTPUT);  //Green LED
digitalWrite(46,LOW); //Green LED
digitalWrite(47,HIGH);  //RED LED       
while(Serial.read() != 'H')
{
  //Serial.println(TotalOffsetProdelay);
Serial.println("A");
}
digitalWrite(47,LOW); // RED LED
digitalWrite(46,HIGH); //Green LED
//ChannelsON();
//delay(2000);
//ChannelsOFF();
  //recvdata();
  delay(10);
 

  
}

void loop() {
Start:
ChannelsOFF();
clearparameters();
recvdata();
B1P1=1;
B1P2=1;
B2P1=1;
B2P2=1;
B3P1=1;
B3P2=1;
B4P1=1;
B4P2=1;
  
SecTimeStart=millis();
OffSetTime=parameters[1];
TimeOneB1P1S=millis()+parameters[2];
TimeOneB1P2S=millis()+parameters[10];
TimeOneB2P1S=millis()+parameters[18]+OffSetTime;
TimeOneB2P2S=millis()+parameters[26]+OffSetTime;
TimeOneB3P1S=millis()+parameters[34]+(OffSetTime*2);
TimeOneB3P2S=millis()+parameters[42]+(OffSetTime*2);
TimeOneB4P1S=millis()+parameters[50]+(OffSetTime*3);
TimeOneB4P2S=millis()+parameters[58]+(OffSetTime*3);

//LED execution

  while(((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*3)))){
   if (Serial.available()){
    if(Serial.read()=='S'){
      goto Start;
    }
   }
   
   //Box 1 Panel 1
   if((millis()-SecTimeStart) < parameters[0]){
    if(((TimeOneB1P1S-millis())>T_OFFSET) && B1P1==1){
      pwm1.setPWM(REDB1P1, 0,parameters[4]);
      pwm1.setPWM(BLUEB1P1, 0, parameters[6]);
      pwm1.setPWM(UVB1P1, 0, parameters[8]);  
    }
    else if(((TimeOneB1P1S-millis())<T_OFFSET) && B1P1==1){
      TimeTwoB1P1S=millis()+parameters[3]+T_OFFSET-PROPAGATION_DELAY_MS2;
      B1P1=2;
    }

    if(((TimeTwoB1P1S-millis())>T_OFFSET) && B1P1==2){
      pwm1.setPWM(REDB1P1, 0,parameters[5]);
      pwm1.setPWM(BLUEB1P1, 0, parameters[7]);
      pwm1.setPWM(UVB1P1, 0, parameters[9]); 
    }
    else if(((TimeTwoB1P1S-millis())<T_OFFSET) && B1P1==2){
      TimeOneB1P1S=millis()+parameters[2]+T_OFFSET-PROPAGATION_DELAY_MS1;
      B1P1=1;
    }
   }
   //turn LED to 0
   else{
      pwm1.setPWM(REDB1P1, 0,4096);
      pwm1.setPWM(BLUEB1P1, 0,4096);
      pwm1.setPWM(UVB1P1, 0, 4096);

      
   }
   //Box 1 Panel 2
   if((millis()-SecTimeStart) < parameters[0]){
    if(((TimeOneB1P2S-millis())>T_OFFSET) && B1P2==1){
      pwm1.setPWM(REDB1P2, 0,parameters[12]);
      pwm1.setPWM(BLUEB1P2, 0, parameters[14]);
      pwm1.setPWM(UVB1P2, 0, parameters[16]);    
    }
    else if(((TimeOneB1P2S-millis())<T_OFFSET) && B1P2==1){
      TimeTwoB1P2S=millis()+parameters[11]+T_OFFSET-PROPAGATION_DELAY_MS2;
      B1P2=2;
    }

    if(((TimeTwoB1P2S-millis())>T_OFFSET) && B1P2==2){
      pwm1.setPWM(REDB1P2, 0,parameters[13]);
      pwm1.setPWM(BLUEB1P2, 0, parameters[15]);
      pwm1.setPWM(UVB1P2, 0, parameters[17]);
    }
    else if(((TimeTwoB1P2S-millis())<T_OFFSET) && B1P2==2){
      TimeOneB1P2S=millis()+parameters[10]+T_OFFSET-PROPAGATION_DELAY_MS1;
      B1P2=1;
    }
   }
   //turn LED to 0
   else{
      pwm1.setPWM(REDB1P2, 0, 4096);
      pwm1.setPWM(BLUEB1P2, 0, 4096);
      pwm1.setPWM(UVB1P2, 0, 4096);
   }
    

   //Box 2 Panel 1
   if(((millis()-SecTimeStart)>OffSetTime) && ((millis()-SecTimeStart) < (parameters[0]+OffSetTime))){
    if(((TimeOneB2P1S-millis())>T_OFFSET) && B2P1==1){
      pwm1.setPWM(REDB2P1, 0,parameters[20]);
      pwm1.setPWM(BLUEB2P1, 0, parameters[22]);
      pwm1.setPWM(UVB2P1, 0, parameters[24]);
    }
    else if(((TimeOneB2P1S-millis())<T_OFFSET) && B2P1==1){
      TimeTwoB2P1S=millis()+parameters[19]+T_OFFSET-PROPAGATION_DELAY_MS2;
      B2P1=2;
    }

    if(((TimeTwoB2P1S-millis())>T_OFFSET) && B2P1==2){
      pwm1.setPWM(REDB2P1, 0,parameters[21]);
      pwm1.setPWM(BLUEB2P1, 0, parameters[23]);
      pwm1.setPWM(UVB2P1, 0, parameters[25]);
     
    }
    else if(((TimeTwoB2P1S-millis())<T_OFFSET) && B2P1==2){
      TimeOneB2P1S=millis()+parameters[18]+T_OFFSET-PROPAGATION_DELAY_MS1;
      B2P1=1;
    }
   }
   //turn LED to 0
   else{ 
      pwm1.setPWM(REDB2P1, 0, 4096);
      pwm1.setPWM(BLUEB2P1, 0, 4096);
      pwm1.setPWM(UVB2P1, 0, 4096);
   }
   

    //Box 2 Panel 2
    if(((millis()-SecTimeStart)>OffSetTime) && ((millis()-SecTimeStart) < (parameters[0]+OffSetTime))){
     if(((TimeOneB2P2S-millis())>T_OFFSET) && B2P2==1){
       pwm1.setPWM(REDB2P2, 0,parameters[28]);
       pwm1.setPWM(BLUEB2P2, 0, parameters[30]);
       pwm1.setPWM(UVB2P2, 0, parameters[32]);
     }
     else if(((TimeOneB2P2S-millis())<T_OFFSET) && B2P2==1){
       TimeTwoB2P2S=millis()+parameters[27]+T_OFFSET-PROPAGATION_DELAY_MS2;
       B2P2=2;
     }
  
     if(((TimeTwoB2P2S-millis())>T_OFFSET) && B2P2==2){
       pwm1.setPWM(REDB2P2, 0,parameters[29]);
       pwm1.setPWM(BLUEB2P2, 0, parameters[31]);
       pwm1.setPWM(UVB2P2, 0, parameters[33]);
       
     }
     else if(((TimeTwoB2P2S-millis())<T_OFFSET) && B2P2==2){
       TimeOneB2P2S=millis()+parameters[26]+T_OFFSET-PROPAGATION_DELAY_MS1;
       B2P2=1;
     }
   }
   //turn LED to 0
   else{
      pwm1.setPWM(REDB2P2, 0, 4096);
      pwm1.setPWM(BLUEB2P2, 0, 4096);
      pwm1.setPWM(UVB2P2, 0, 4096);
   }



    //Box 3 Panel 1
    if(((millis()-SecTimeStart)>(OffSetTime*2)) && ((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*2)))){
    //if((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*4))){
     if(((TimeOneB3P1S-millis())>T_OFFSET) && B3P1==1){
       pwm2.setPWM(REDB3P1, 0,parameters[36]);
       pwm2.setPWM(BLUEB3P1, 0, parameters[38]);
       pwm2.setPWM(UVB3P1, 0, parameters[40]);
     }
     else if(((TimeOneB3P1S-millis())<T_OFFSET) && B3P1==1){
       TimeTwoB3P1S=millis()+parameters[35]+T_OFFSET-PROPAGATION_DELAY_MS2;
       B3P1=2;
     }

     if(((TimeTwoB3P1S-millis())>T_OFFSET) && B3P1==2){
       pwm2.setPWM(REDB3P1, 0,parameters[37]);
       pwm2.setPWM(BLUEB3P1, 0, parameters[39]);
       pwm2.setPWM(UVB3P1, 0, parameters[41]);
     
     }
     else if(((TimeTwoB3P1S-millis())<T_OFFSET) && B3P1==2){
       TimeOneB3P1S=millis()+parameters[34]+T_OFFSET-PROPAGATION_DELAY_MS1;
       B3P1=1;
     }
    }
    //turn LED to 0
    else{
       pwm2.setPWM(REDB3P1, 0, 4096);
       pwm2.setPWM(BLUEB3P1, 0, 4096);
       pwm2.setPWM(UVB3P1, 0, 4096); 
    }


    //Box 3 Panel 2
    if(((millis()-SecTimeStart)>(OffSetTime*2)) && ((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*2)))){
    //if((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*5))){
     if(((TimeOneB3P2S-millis())>T_OFFSET) && B3P2==1){
       pwm2.setPWM(REDB3P2, 0,parameters[44]);
       pwm2.setPWM(BLUEB3P2, 0, parameters[46]);
       pwm2.setPWM(UVB3P2, 0, parameters[48]);
     }
     else if(((TimeOneB3P2S-millis())<T_OFFSET) && B3P2==1){
       TimeTwoB3P2S=millis()+parameters[43]+T_OFFSET-PROPAGATION_DELAY_MS2;
       B3P2=2;
     }
 
     if(((TimeTwoB3P2S-millis())>T_OFFSET) && B3P2==2){
       pwm2.setPWM(REDB3P2, 0,parameters[45]);
       pwm2.setPWM(BLUEB3P2, 0, parameters[47]);
       pwm2.setPWM(UVB3P2, 0, parameters[49]);
     
     }
     else if(((TimeTwoB3P2S-millis())<T_OFFSET) && B3P2==2){
       TimeOneB3P2S=millis()+parameters[42]+T_OFFSET-PROPAGATION_DELAY_MS1;
       B3P2=1;
     }
    }
    //turn LED to 0
    else{
       pwm2.setPWM(REDB3P2, 0, 4096);
       pwm2.setPWM(BLUEB3P2, 0, 4096);
       pwm2.setPWM(UVB3P2, 0, 4096);
    }



   //Box 4 Panel 1
   if(((millis()-SecTimeStart)>(OffSetTime*3)) && ((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*3)))){
   //if((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*6))){
    if(((TimeOneB4P1S-millis())>T_OFFSET) && B4P1==1){
      pwm2.setPWM(REDB4P1, 0,parameters[52]);
      pwm2.setPWM(BLUEB4P1, 0, parameters[54]);
      pwm2.setPWM(UVB4P1, 0, parameters[56]);
    }
    else if(((TimeOneB4P1S-millis())<T_OFFSET) && B4P1==1){
      TimeTwoB4P1S=millis()+parameters[51]+T_OFFSET-PROPAGATION_DELAY_MS2;
      B4P1=2;
    }

    if(((TimeTwoB4P1S-millis())>T_OFFSET) && B4P1==2){
      pwm2.setPWM(REDB4P1, 0,parameters[53]);
      pwm2.setPWM(BLUEB4P1, 0, parameters[55]);
      pwm2.setPWM(UVB4P1, 0, parameters[57]);
     
    }
    else if(((TimeTwoB4P1S-millis())<T_OFFSET) && B4P1==2){
      TimeOneB4P1S=millis()+parameters[50]+T_OFFSET-PROPAGATION_DELAY_MS1;
      B4P1=1;
    }
   }
   //turn LED to 0
    else{
      pwm2.setPWM(REDB4P1, 0, 4096);
      pwm2.setPWM(BLUEB4P1, 0, 4096);
      pwm2.setPWM(UVB4P1, 0, 4096);
    }
   



   //Box 4 Panel 2
   if(((millis()-SecTimeStart)>(OffSetTime*3)) && ((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*3)))){
   //if((millis()-SecTimeStart) < (parameters[0]+(OffSetTime*7))){
    if(((TimeOneB4P2S-millis())>T_OFFSET) && B4P2==1){
      pwm2.setPWM(REDB4P2, 0,parameters[60]);
      pwm2.setPWM(BLUEB4P2, 0, parameters[62]);
      pwm2.setPWM(UVB4P2, 0, parameters[64]);
    }
    else if(((TimeOneB4P2S-millis())<T_OFFSET) && B4P2==1){
      TimeTwoB4P2S=millis()+parameters[59]+T_OFFSET-PROPAGATION_DELAY_MS2;
      B4P2=2;
    }

    if(((TimeTwoB4P2S-millis())>T_OFFSET) && B4P2==2){
      pwm2.setPWM(REDB4P2, 0,parameters[61]);
      pwm2.setPWM(BLUEB4P2, 0, parameters[63]);
      pwm2.setPWM(UVB4P2, 0, parameters[65]);
     
    }
    else if(((TimeTwoB4P2S-millis())<T_OFFSET) && B4P2==2){
      TimeOneB4P2S=millis()+parameters[58]+T_OFFSET-PROPAGATION_DELAY_MS1;
      B4P2=1;
    }
   }
   //turn LED to 0
    else{
      pwm2.setPWM(REDB4P2, 0, 4096);
      pwm2.setPWM(BLUEB4P2, 0, 4096);
      pwm2.setPWM(UVB4P2, 0, 4096);
    }
    
  }
 ChannelsOFF();
}
