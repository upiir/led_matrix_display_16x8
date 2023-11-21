// using 16x8 LED Matrix Display from Keyestudio with Arduino UNO
// based on the sketch from Keyestudio: https://wiki.keyestudio.com/KS0357A_Keyestudio_8x16_LED_Matrix_Panel

// created by upir, 2023
// youtube channel: https://www.youtube.com/upir_upir

// Source files: https://github.com/upiir/led_matrix_display_16x8
// Youtube video: https://youtu.be/If-hiuwsQh0


// Links from the video:
// 16x8 LED Display: https://s.click.aliexpress.com/e/_DEyP8lh
// Keyestudio version of the display: https://s.click.aliexpress.com/e/_DdY0vJt
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino prototyping shield: https://s.click.aliexpress.com/e/_ApbCwx
// Breadboard wires: https://s.click.aliexpress.com/e/_Dkbngin
// Image2cpp (convert array to image): https://javl.github.io/image2cpp/
// Piskel online graphics editor: https://www.piskelapp.com/p/create/sprite
// Keyestudio documentation: https://wiki.keyestudio.com/KS0357A_Keyestudio_8x16_LED_Matrix_Panel

// Related videos with Arduino UNO and matrix displays screens:
// Multiplexing with Arduino: https://youtu.be/tHL4RYGSvg4
// Arduino Wooden Clock: https://youtu.be/50bVXHYW_9Q
// Arduino Uno R4 WIFI display: https://youtu.be/Qpi0Y5bcvlQ
// LED badges: https://youtu.be/MRTxTx8bgYg
// 8x8 LED display: https://youtu.be/oLgUtjyKO6Q
// LED Matrix Displays: https://youtu.be/jlhcDzS17vU
// Very old matrix display: https://youtu.be/PBaL9w5w-2c



//data display from right to left, from bottom to top, HIGH level display. 
#define IIC_SCL  A5
#define IIC_SDA  A4

#include "piskel_animations.h" // all animations are generated in Piskel online application and stored in separate header file

void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();


void setup() 
{
  pinMode(IIC_SCL,OUTPUT);
  pinMode(IIC_SDA,OUTPUT);
  digitalWrite(IIC_SCL,LOW);
  digitalWrite(IIC_SDA,LOW);



    /*************set the brightness display***************/ 
    IIC_start();
    //IIC_send(0x8A);// set the brightness display
    //IIC_send(B10001010);// set the brightness display
    //IIC_send(B10001000);// set the brightness display // LOWEST
    IIC_send(B10001110);// set the brightness display // BRIGHTEST
    IIC_end(); 
    /*************end the brightness display***************/ 

  
}


int play_anim = -1; // currently playing animations, -1 == no animation, just static image
int play_anim_frames[] = {14, 18, 24, 21}; // number of frames for individual animations
int play_anim_frame = 0; // currently displayed frame



void loop() 
{

if (play_anim == -1) { // no anim playing, perhaps play something from time to time
  if (random(20) == 1) {play_anim = random(0, 4);} // start the animation from time to time
} else { // some animation is playing, increase frame and if it´s the last frame, don´t keep playing
  play_anim_frame++; // increase the frame
  if (play_anim_frame > play_anim_frames[play_anim]) { // if the frame is bigger than the number of frames for the animation
    play_anim = -1; // stop the animation
    play_anim_frame = 0; // reset the current frame
    }
}


  
    /**************set the address plus 1***************/
    IIC_start();
    IIC_send(0x40);// set the address plus 1 automatically
    IIC_end();
    /************end the process of address plus 1 *****************/
    /************set the data display*****************/ 
    IIC_start();
    IIC_send(0xc0);// set the initial address as 0


    // send the individual pixels based on the animation
    // if no animation is playing = show static image
    for(char i = 0;i < 16;i++) {
        switch (play_anim) {
        case -1:
          IIC_send(piskel_static[i]);// send the display data  
        break;
        case 0:
          IIC_send(piskel_eyesheart[play_anim_frame][i]);// send the display data  
        break;
        case 1:
          IIC_send(piskel_laughing[play_anim_frame][i]);// send the display data  
        break;
        case 2:
          IIC_send(piskel_talking[play_anim_frame][i]);// send the display data  
        break;                
        case 3:
          IIC_send(piskel_xeyes[play_anim_frame][i]);// send the display data  
        break;        
        default:
          break;
        }
    }



    
    IIC_end();
    /************end the data display*****************/

    delay(50); // delay between frames
}




// the code below is from the original sketch from Keyestudio for software IIC implementation (bigbanging IIC)



/*----------------------------------------------------------------*/
void IIC_start()
{
  digitalWrite(IIC_SCL,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,LOW);
  delayMicroseconds(3);
}
void IIC_send(unsigned char send_data)
{
  for(char i = 0;i < 8;i++)
  {
      digitalWrite(IIC_SCL,LOW);
      delayMicroseconds(3); 
      if(send_data & 0x01)
      {
        digitalWrite(IIC_SDA,HIGH);
      }
      else
      {
        digitalWrite(IIC_SDA,LOW);
      }
      delayMicroseconds(3);
      digitalWrite(IIC_SCL,HIGH); 
      delayMicroseconds(3);
      send_data = send_data >> 1;
  }
}
void IIC_end()
{
  digitalWrite(IIC_SCL,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,HIGH);
  delayMicroseconds(3);
}
