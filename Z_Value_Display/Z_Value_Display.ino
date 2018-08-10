#include <Adafruit_NeoPixel.h>

#define PIN 6           //Where is the LED string connected
#define TOTALPIXELS 57

/*
 * The following lines defines how the characters are built.
 * We're expecting WS2812 LEDs which are 'just' connected as a long string.
 * The segments are connected as follows:
 *    ---1---
 *   |       |
 *   6       2
 *   |       |
 *    ---7---
 *   |       |
 *   5       3
 *   |       |
 *    ---4---
 *  The digits I did didn't have consistent segment wiring and thus we need a seperate offset variable for each digit. that's the offsets
 *  the segments array is the mapping of which segments should light up for numbers 0-9. if you change the offset you will have to change these as well.
 *
 */
#define LEDSPERSEGMENT 4  //How many LEDs per segment
unsigned char offset1[] = {3,4,6,0,1,2,5}; //segment1 offset
unsigned char offset2[] = {2,3,5,6,0,1,4}; //segment2 offset
unsigned char segments1[][7] = {{1,1,1,1,1,1,0},{0,1,1,0,0,0,0},{1,1,0,1,1,0,1},{1,1,1,1,0,0,1},{0,1,1,0,0,1,1},{1,0,1,1,0,1,1},{1,0,1,1,1,1,1},{1,1,1,0,0,0,0},{1,1,1,1,1,1,1},{1,1,1,0,0,1,1}};
unsigned char segments2[][7] = {{1,1,1,1,1,1,0},{0,1,1,0,0,0,0},{1,1,0,1,1,0,1},{1,1,1,1,0,0,1},{0,1,1,0,0,1,1},{1,0,1,1,0,1,1},{1,0,1,1,1,1,1},{1,1,1,0,0,0,0},{1,1,1,1,1,1,1},{1,1,1,0,0,1,1}};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTALPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color = strip.Color(0, 255, 0);  //the color used on the display
double number = 99.9;   //the number to show on the display
double oldNumber=99.9;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(57600);  //Start serial comm.
  Serial.println("Starting..."); //nothing should be connected to TX so this is only for debug
}

void loop() 
{
  if(Serial.read()=='<')  //we're listening for the beginning of the response. if we get one then start recording the bytes
  { 
    char buf[60];
    Serial.readBytesUntil('>',buf,60);  //until we reach the end of the response.

    char search=0;
    char index=0;
    char index2=0;
    for(int i=0;i<sizeof(buf);i++)  //now the fun begins. we need to find the location of the 4th comma in the array of chars.
    {
      if(buf[i]==',')
        search++;     //we found a comma! if its on of the first 3 then just carry on
      if (search==3)
      {
        index=i;      //this is it! the 4th comma! after this, the Z-value resides
        break;
      }
    }

    //to figure out how long the Z-value is, we need to find the 5th comma which marks the end of the depth.
    for(int i=index+1;i<sizeof(buf);i++)    //we only need to start searching after the 4th comma
    {
      if(buf[i]==',')
      {
        index2=i;
        break;
      }
    }
    char newNumber[(index2-index)-1]; //we now create a char array with only the actual value in it (including sign and decimal point)
    for(int i=0;i<index2;i++)
      newNumber[i]=buf[index+i+1];
    number= round(atof(newNumber)*10)/10.0;          //we convert that into a floating point number
    Serial.println(number, DEC);      //and print it for debugging

    if(oldNumber!=number)             //only if the number is changed we need to update the display. This is to limit flickering
    {
      oldNumber=number;
      showNumber();   //update the display and show the number found
    }    
  }
} 

void showNumber()
{
  
  for(int i=0;i<TOTALPIXELS;i++)
    strip.setPixelColor(i,strip.Color(0,0,0));
  //strip.show();

  if(number<0)  //if the value is negative use RED - if positive: GREEN
    color = strip.Color(255, 0, 0);
  else
    color = strip.Color(0, 255, 0);

  if(fabs(number)<=9.9 && fabs(number)>=1) //number is below 9.9 and we can show a decimal digit
  {
    strip.setPixelColor(TOTALPIXELS-1, strip.Color(0,0,255));//light up the decimal point and
    number*=10; //multiply the number by 10 so the next if-logic block will handle the rest
  }

  
  if(fabs(number)>9.9)  //if number is larger than 9.9 we can't show the decimal place
  {
    for(int i=0;i<7;i++)  //for all segments in the 10's digit
    {
      if(segments1[(int)abs(number)/10][i]==1)  //if the segment should light up  //we're taking the absolute value to remove the sign
      {
        for(int j=0;j<LEDSPERSEGMENT;j++)
          strip.setPixelColor((offset1[i]*LEDSPERSEGMENT)+j, color);
      }
    }
    for(int i=0;i<7;i++)  //for all segments in the 1's digit
    {
      if(segments2[(int)abs(number)%10][i]==1)  //if the segment should light up
      {
        for(int j=0;j<LEDSPERSEGMENT;j++)
          strip.setPixelColor((offset2[i]*LEDSPERSEGMENT)+j+28, color);
      }
    }
  }
  
  else if(fabs(number<1))
  {
    strip.setPixelColor(TOTALPIXELS-1, strip.Color(0,0,255));//light up the decimal point
    
    //if you want a leading 0 then you should uncomment this next section
    /*
    for(int i=0;i<7;i++)  //for all segments in the 10's digit
    {
      if(segments1[0][i]==1)  //if the segment should light up
      {
        for(int j=0;j<LEDSPERSEGMENT;j++)
          strip.setPixelColor((offset1[i]*LEDSPERSEGMENT)+j, color);
      }
    }
    */
    for(int i=0;i<7;i++)  //for all segments in the 1's digit
    {
      if(segments2[(int)abs(number*10)][i]==1)  //if the segment should light up
      {
        for(int j=0;j<LEDSPERSEGMENT;j++)
          strip.setPixelColor((offset2[i]*LEDSPERSEGMENT)+j+28, color);
      }
    }
  }
    
  strip.show();
}
