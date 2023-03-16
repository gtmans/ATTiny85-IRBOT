/* MyIRBot (c)Mans Rademaker 2023
 * board:ATTiny25/45/85 processor:ATTiny85 programmer Arduino as ISP Use_Tiny:yes
 * board:digispark      processor:digi     programmer Microneus      Use_Tiny:no 
 */
 
#define     Use_Tiny         // use digispark drivers or Tiny4Koled
#ifdef      Use_Tiny         // ATTiny straight
  #include "TinyWireM.h"
  #include "Tiny4kOLED.h"
#else                        // Digispark board
  #include <DigisparkOLED.h>
  #include <Wire.h>
#endif
#include "my_eyes.h"

bool test     = false;
bool flat     = false;
int  sleeping = 0;
int  analog3; // left
int  analog2; // right
int  digital; // updown
int  xpos;
int  ypos;
int  old_xpos;
int  old_ypos;

void setup() {
  #ifdef Use_Tiny
    oled.begin            (128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
    oled.setInternalIref  (true);
    oled.setFont          (FONT6X8P);
  #else
    oled.begin            ();  
  #endif
  pinMode                 (PB1,INPUT);//PB1
  pinMode                 (A2, INPUT);//PB4
  pinMode                 (A3, INPUT);//PB3  
  //demo txt
  oled.clear();      
  oled.setFontX2          (FONT6X8P);
  oled.setCursor          (16, 1);
  oled.print              ("DigiMans");
  oled.setCursor          (16, 4); 
  oled.print              ("(c)2023");
  oled.on                 ();
  delay                   (1000);
  WhatsUp                 ();         // move eyes a bit around 
}

void loop() {
  digital=digitalRead(PB1);
  analog2=analogRead (A2);
  analog3=analogRead (A3);
  if  (analog2<50){if(xpos<3){xpos++;}}
  if  (analog3<50){if(xpos>0){xpos--;}}
  if  (analog3<50&&analog2<50){flat=true;old_xpos=9;}else{flat=false;}
  if  (digital==false){ypos--;}else{ypos++;}
  // watch bounderies
  if  (xpos==0||xpos==3){if (ypos<2){ypos=2;};if (ypos>4){ypos=4;}}
  else                      {if (ypos<1){ypos=1;};if (ypos>5){ypos=5;}}   
  // move eyes if changed
  if  (xpos!=old_xpos||ypos!=old_ypos){DrawEyes(xpos);sleeping=0;
  } else {
      sleeping++;
      if (sleeping>100){WhatsUp();sleeping=0;}//after prox 10 seconds nothing changed move eyes
  }
  delay   (100);
}

void DrawEyes   (int i){   // 0-3 = left to right 
  if (flat){
  //oled.bitmap (START X IN PIXELS, START Y IN ROWS OF 8 PIXELS, END X IN PIXELS, END Y IN ROWS OF 8 PIXELS, IMAGE ARRAY);
    oled.bitmap ((i*8)         ,0,48+(i*8)   ,8,      oflat);     //48x64 
    oled.bitmap ((i*8)+56      ,0,48+(i*8)+56,8,      oflat);
    oled.bitmap ((i*12)+10,ypos,(i*12)+26    ,ypos+1, pupilhalf); //16x8
    oled.bitmap ((i*12)+66,ypos,(i*12)+82    ,ypos+1, pupilhalf);
  }else {     
    if   (ypos==4&&(xpos==3||xpos==0)){
      if (xpos==3){      
        oled.bitmap ((i*8)         ,0,(i*8)+48   ,8,      winkl);     //48x64 
        oled.bitmap ((i*8)+56      ,0,(i*8)+104  ,8,      winkl);
      } else {
        oled.bitmap ((i*8)         ,0,(i*8)+48   ,8,      winkr);     //48x64 
        oled.bitmap ((i*8)+56      ,0,(i*8)+104  ,8,      winkr);
      }
    } else {
      if (ypos==1){
        oled.bitmap   ((i*8)       ,0,(i*8)+48   ,8,      oflat);     //48x64 
        oled.bitmap   ((i*8)+56    ,0,(i*8)+104  ,8,      oflat);
      } else {
        oled.bitmap   ((i*8)       ,0,(i*8)+48   ,8,      eye);       //48x64 
        oled.bitmap   ((i*8)+56    ,0,(i*8)+104  ,8,      eye);
      }
    }
    oled.bitmap     ((i*12)+10,ypos,(i*12)+26    ,ypos+2, pupil);     //16x16
    oled.bitmap     ((i*12)+66,ypos,(i*12)+82    ,ypos+2, pupil);
  }
  #ifdef Use_Tiny
    oled.on     ();
  #endif  
  old_xpos=xpos;
  old_ypos=ypos;
}

void WhatsUp(){
  oled.clear();  
  xpos=0;ypos=4;DrawEyes(xpos);
  delay(500);
  xpos=1;ypos=1;DrawEyes(xpos);
  delay(250);
  xpos=2;ypos=1;DrawEyes(xpos);
  delay(250);
  xpos=3;ypos=4;DrawEyes(xpos);
  delay(500);
  oled.clear();  
  xpos=2;ypos=5;DrawEyes(xpos);
}
