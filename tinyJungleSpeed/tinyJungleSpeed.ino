#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>

  static const unsigned short randMix[] = {
    3961, 57166, 37426, 345, 
    4727, 31105, 32898, 58696, 
    40631, 49699, 28450, 16900, 
    55992, 41894, 14318, 18762};
  static unsigned short cheapRndA = 0x1923,cheapRndB = 0x4232,cheapRndC = 0x2393;
  static unsigned short cheapRnd() {
    unsigned short t;
    cheapRndA ^= (cheapRndA >> 5);
    cheapRndA ^= cheapRndA << 1;  
      t = cheapRndA;
      cheapRndA = cheapRndB;
      cheapRndB = cheapRndC;
      cheapRndC = t ^ cheapRndA ^ cheapRndB;
      return cheapRndC;
  }
  static void cheapRndSeed(unsigned short x, unsigned short y) {
    cheapRndA = x ^ randMix[((x*7)>>2)&15];
    cheapRndB = y ^ randMix[((y*3)>>2)&15];
    cheapRndC = (x+y) ^ randMix[(cheapRndA^cheapRndB)&15];
  }
  static unsigned char cheapRndA8 = 0x1923,cheapRndB8 = 0x4232,cheapRndC8 = 0x2393;
  static unsigned char cheapRnd8() {
    unsigned char t;
    cheapRndA8 += 41;
      t = cheapRndA8;
      cheapRndA8 = cheapRndB8;
      cheapRndB8 = cheapRndC8;
      cheapRndC8 = t ^ cheapRndA8 ^ cheapRndB8;
      return cheapRndC8;
  }
  static void cheapRndSeed8(unsigned short x, unsigned short y) {
    cheapRndA8 = x ^ randMix[(x-3281)&15];
    cheapRndB8 = y ^ randMix[(y+17)&15];
    cheapRndC8 = (x+y) ^ randMix[(y-x)&15];
  }

TinyScreen display = TinyScreen(TinyScreenPlus);

uint8_t randomColor(){
  //black is not used
  uint8_t colors[11] ={TS_8b_Gray,TS_8b_White, TS_8b_Blue ,TS_8b_DarkBlue ,TS_8b_Red ,TS_8b_DarkRed ,TS_8b_Green ,TS_8b_DarkGreen ,TS_8b_Brown ,TS_8b_DarkBrown ,TS_8b_Yellow };
  int r = cheapRnd()%11;
  return colors[r];
}
static bool ok_to_run;
static bool debug;
static int debugIndex;

void setup(void) {
  Wire.begin();
  display.begin();
  display.setBrightness(7);
  writeInstructions();
  ok_to_run = true;
  debug = false;
  debugIndex = 0;
  while(!getInput());
  draw_countdown();
}

void loop() {
  if(ok_to_run){
    if(debug){
      drawSign(debugIndex);
      ++debugIndex;
    }
    else {
      int r = cheapRnd()%17;
      bool good = drawSign(cheapRnd()%10);
    }
    delay(1500);
    ok_to_run = false;
    //drawBitmap();
  }
  else {
    if(getInput()) ok_to_run = true;
    if(ok_to_run) draw_countdown();
  }
}

void draw_countdown(){
  display.clearScreen();
  display.setFont(thinPixel7_10ptFontInfo);
  int width=display.getPrintWidth("N!");
  display.setCursor(48-(10),20);
  //fontColor(text color, background color);//sets text and background color
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.print("3!");
  delay(1000);

  display.clearScreen();
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.print("2!");
  delay(1000);

  display.clearScreen();
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.print("1!");
  delay(1000);

}

int getInput() {
  if (!digitalRead(44) || !digitalRead(45))
    return 1;
  if (abs((int)analogRead(42) - 512) > 300)
    return 1;
  if (abs((int)analogRead(1) - 512) > 300)
    return 1;
  return 0;
}

bool drawSign(int r){
   display.clearScreen();
   display.clearWindow(20,20,56,24);
   bool is_good = false;
   
   if(r == 0) {
      display.drawLine(0,0,95,63,randomColor());
      display.drawLine(0,63,95,0,randomColor()); 
   }
   else if(r == 1){
      display.drawLine(0,40,95,63,randomColor());
      display.drawLine(5,0,80,42,randomColor());
      is_good = true;
   }
   else if(r == 2){
      display.drawLine(0,0,95,63,randomColor());
      display.drawLine(5,0,90,63,randomColor());
   }
   else if(r == 3){
      drawCircle(30,30,10,randomColor());
      display.drawLine(5,0,90,63,randomColor());
   }
   else if(r == 4){
      drawCircle(30,30,10,randomColor());
      drawCircle(50,30,15,randomColor());
   }
   else if(r == 5){
      display.drawLine(0,0,95,63,randomColor());
      display.drawLine(5,0,105,55,randomColor());
   }
   else if(r == 6){
      drawCircle(0,0,40,randomColor());
      drawCircle(10,10,20,randomColor());
      is_good = true;
   }
   else if(r == 7){
      drawCircle(0,0,40,randomColor());
      drawCircle(17,17,20,randomColor());
      is_good = true;
   }
   else if(r == 8){
      drawCircle(0,0,30,randomColor());
      display.drawLine(5,0,105,55,randomColor());
   }
   else if(r == 9){
      display.drawLine(0,0,15,15,randomColor());
      display.drawLine(15,22,25,13,randomColor());
      display.drawLine(5,30,70,45,randomColor());
      display.drawLine(70,5,70,20,randomColor());
      display.drawLine(5,50,80,55,randomColor());
      is_good = true;
   }
   else if(r == 10){
       drawCircle(50,30,10,randomColor());
       is_good = true;
   }
   else if(r == 11){
      drawCircle(0,0,40,randomColor());
      drawCircle(10,10,20,randomColor());

      drawCircle(40,40, 10, randomColor());
      drawCircle(45,52, 10, randomColor());
      is_good = true;
   }
   else if(r == 12){
      drawCircle(0,0,40,randomColor());
      drawCircle(17,17,20,randomColor());
      is_good = true;
   }
   else if(r == 13){
      display.drawLine(0,0,15,15,randomColor());
      display.drawLine(15,22,57,13,randomColor());
      display.drawLine(5,30,70,45,randomColor());
      display.drawLine(22,5,70,20,randomColor());
      display.drawLine(5,50,80,55,randomColor());
   }
   else if(r == 14){
      display.drawRect(10,12,20,30, 0, randomColor());
      drawCircle(50, 15, 12, randomColor());
   }
   else if(r == 15){
      display.drawRect(10,12,40,30, 0, randomColor());
      drawCircle(50, 15, 12, randomColor());
   }
   else if(r == 16){
      display.drawRect(10,12,10,30, 0, randomColor());
      drawCircle(50, 15, 12, randomColor());
      display.drawRect(65,22,20,20, 0, randomColor());
   }
   else {
      display.drawLine(0,0,95,63,randomColor());
      display.drawLine(0,63,95,0,63,0,0); 
   }
   
   return is_good;
}

void drawCircle(int x0, int y0, int radius, uint8_t color)
{
  int x = radius;
  int y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    //drawPixel(x,y,color);//set pixel (x,y) to specified color. This is slow because we need to send commands setting the x and y, then send the pixel data.
    display.drawPixel(x + x0, y + y0, color);
    display.drawPixel(y + x0, x + y0, color);
    display.drawPixel(-x + x0, y + y0, color);
    display.drawPixel(-y + x0, x + y0, color);
    display.drawPixel(-x + x0, -y + y0, color);
    display.drawPixel(-y + x0, -x + y0, color);
    display.drawPixel(x + x0, -y + y0, color);
    display.drawPixel(y + x0, -x + y0, color);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x) + 1;
    }
  }
}

void writeInstructions(){
  display.clearScreen();
  display.setFont(thinPixel7_10ptFontInfo);
  
  display.setCursor(5,0);
  display.fontColor(TS_8b_Black,TS_8b_Red);
  display.print("Tiny Tottem Speed!");

 display.setCursor(21,10);
  display.fontColor(TS_8b_Blue,TS_8b_Black);
  display.print("Pick a totem");
  
  display.setCursor(20,20);
  display.fontColor(TS_8b_Green,TS_8b_Black);
  display.print("Take it when");
  display.setCursor(10,30);
  display.fontColor(TS_8b_Green,TS_8b_Black);
  display.print("in the image are");
    display.setCursor(10,40);
  display.fontColor(TS_8b_Green,TS_8b_Black);
  display.print("NO intersections");
  
  display.setCursor(10,50);
  display.fontColor(TS_8b_Red,TS_8b_Black);
  display.print("Don't otherwhise!");
  
  
}


