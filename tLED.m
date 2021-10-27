%{
The ARRAY variables:

 * <Sessiontime(0),OffSetTime(1),TimeOneB1P1(2),TimeTwoB1P1(3),intensityTimeONER(4),intensityTimeTWOR(5),intensityTimeONEB(6),intensityTimeTWOB(7),intensityTimeONEU(8),intensityTimeTWOU(9),
 *                               TimeOneB1P2(10),TimeTwoB1P2(11),intensityTimeONER(12),intensityTimeTWOR(13),intensityTimeONEB(14),intensityTimeTWOB(15),intensityTimeONEU(16),intensityTimeTWOU(17),
 *                               TimeOneB2P1(18),TimeTwoB2P1(19),intensityTimeONER(20),intensityTimeTWOR(21),intensityTimeONEB(22),intensityTimeTWOB(23),intensityTimeONEU(24),intensityTimeTWOU(25),
 *                               TimeOneB2P2(26),TimeTwoB2P2(27),intensityTimeONER(28),intensityTimeTWOR(29),intensityTimeONEB(30),intensityTimeTWOB(31),intensityTimeONEU(32),intensityTimeTWOU(33),
 *                               TimeOneB3P1(34),TimeTwoB3P1(35),intensityTimeONER(36),intensityTimeTWOR(37),intensityTimeONEB(38),intensityTimeTWOB(39),intensityTimeONEU(40),intensityTimeTWOU(41),
 *                               TimeOneB3P2(42),TimeTwoB3P2(43),intensityTimeONER(44),intensityTimeTWOR(45),intensityTimeONEB(46),intensityTimeTWOB(47),intensityTimeONEU(48),intensityTimeTWOU(49),
 *                               TimeOneB4P1(50),TimeTwoB4P1(51),intensityTimeONER(52),intensityTimeTWOR(53),intensityTimeONEB(54),intensityTimeTWOB(55),intensityTimeONEU(56),intensityTimeTWOU(57),
 *                               TimeOneB4P2(50),TimeTwoB4P2(59),intensityTimeONER(60),intensityTimeTWOR(61),intensityTimeONEB(62),intensityTimeTWOB(63),intensityTimeONEU(64),intensityTimeTWOU(65)>
 
  If one pannel is not going to be use you can put 0 to all the variables of that panel.
  The same with a entire box(chamber). You can put 0 to all 8 variable of that panel.
  Minimum value to TimeOneBxPx and TimeTwoBxPx is 5mS. Max frequency is 100Hz with a 10% of error. At > 6ms the error % is 0.
 
 In this new code you will the user will be capable to start the panels with a offset time. The offset time is the second value in the array.
 If the user enter 2 seconds(2000 miliseconds) for the offsettime value, then the first panel will start in time 0 and ending at Sessiontime, but the second panel will be starting at offsettime that is 2 seconds and ending in Sessiontime + offsettime.

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
%}
 checkport=false;
 %g=exist device;
 try
     readline(device);
 catch
     clear all;

 end
if (exist('device')==1)
 if(readline(device)=="A")
  e=true;
  f='H';
  writeline(device,f);
  
 else
     clear all;
     disp("Serial port not connected")
    checkport=true;
    e=false;
 end
 else
     clear all;
     disp("Serial port not exist")
    checkport=true;
    e=false;
end

pause(1);
go=true;
d=serialportlist("available");
i=0;
b=length(d);

  
  
 while (checkport==1) && (i<b)
  i=i+1;
  disp("Searching for Serial port")
  try
  device = serialport(d(1,i),115200);
  configureTerminator(device,"CR/LF");
  checkport=false;
  catch
  checkport=true;
  end
  if(checkport==false)
     %c='arduino';
     clear c;
     c=readline(device);
     %c=read(device,1,"char");
     
    if(class(c)~='double')     
     if(c == "A"  )
      disp('serial port found')
      f='H';
      writeline(device,f);
      e=true;
     else
         checkport=true;
         e==false;
     end
    else
        checkport=true;
        e=false;
    end
     
  end
 end
 
 if (e==false)
     disp('serial port NOT found')
     clear all;
 end
 
 
 
while go && e               
a= input('Enter Array: ','s');
writeline(device,a);

 while go
  clear a;
  a= input('Enter S to STOP the Session OR to Re-Enter Array: ','s');
  
   if (a == 'S')
    disp('Session STOP or Completed');
    f='S';
    writeline(device,f);
    go=false;
    clear a;
   end
 end
 go=true;
end
