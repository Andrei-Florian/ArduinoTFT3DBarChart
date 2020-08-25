// Universum | Universum Projects > BarChart3D

// Andrei Florian 2/NOV/2018 - 3/NOV/2018

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <Universum_TFTColours.h> // Universum library for colours

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Global Variables
  int valueBlock[4];
  int posBlock[4];
  int prevPosBlock[4];
  bool firstRun = true;
  
// Editable Variables
  bool proDebug = 0;
  bool displayValues = true;
  
  uint16_t graphColor = BLUE;
  uint16_t blockColor = GREEN;
  
  String graphName = "3D Bar Chart";
  String graphBlock[] = {"Pot 1", "Pot 2"};
  
  int graphRange = 1024;

// calculate values
  const int numberOfMarks = 4;
  const int originX = 45;
  const int originY = 200;
  const int sizeX = 270;
  const int sizeY = 150;
  
  int boxSize = (sizeX / numberOfMarks);
  int mark[] = {boxSize, (boxSize * 2), (boxSize * 3), (boxSize * 4), (boxSize * 5), (boxSize * 6), (boxSize * 7), (boxSize * 8)};

  const int minorSizeY = (originY + 10);
  const int minorSizeX = (originX - 10);
  
  int numberSize = (sizeY / 6);
  int number[] = {numberSize, (numberSize * 2), (numberSize * 3), (numberSize * 4), (numberSize * 5), (numberSize * 6)};

  int numberValue = (graphRange / 6);
  int val[] = {graphRange, (numberValue * 5), (numberValue * 4), (numberValue * 3), (numberValue * 2), numberValue};

void drawHome()
{
  tft.fillScreen(BLACK);
  delay(500);
  
  tft.setCursor(10, 10); // set the cursor
  tft.setTextColor(BLUE); // set the colour of the text
  tft.setTextSize(5); // set the size of the text
  tft.println("Universum");
  
  tft.setCursor(10, 80); // set the cursor
  tft.setTextColor(CYAN); // set the colour of the text
  tft.setTextSize(3); // set the size of the text
  tft.println("Graphing");

  tft.setCursor(30, 110); // set the cursor
  tft.setTextColor(CYAN); // set the colour of the text
  tft.setTextSize(2); // set the size of the text
  tft.println("3D Bar Charts");
  
  tft.setCursor(10, 160); // set the cursor
  tft.setTextColor(WHITE); // set the colour of the text
  tft.setTextSize(2); // set the size of the text
  tft.println("Andrei Florian");
  delay(4000);

  tft.fillScreen(WHITE);
  delay(500);
}

void drawGraph()
{
  // draw title
  tft.setCursor(10, 10); // set the cursor
  tft.setTextColor(BLUE); // set the colour of the text
  tft.setTextSize(3); // set the size of the text
  tft.println(graphName);
  
  // draw outline
  tft.drawLine(originX, originY, ((originX + sizeX) - 20), originY, graphColor);
  tft.drawLine(originX, originY, originX, (originY - sizeY), graphColor);
  tft.drawLine(originX, originY, (originX + 20), (originY - 20), graphColor);
  tft.drawLine(((originX + sizeX) - 20), originY, ((originX + sizeX)), (originY - 20), graphColor);

  tft.drawLine((originX + 20), (originY - 20), (originX + 20), ((originY - sizeY) - 20), graphColor);
  tft.drawLine((originX + 20), ((originY - sizeY) - 20), originX, (originY - sizeY), graphColor);
  tft.drawLine((originX + 20), (originY - 20), ((originX + sizeX)), (originY - 20), graphColor);
  
  // draw lables
  for(int i = 0; i < numberOfMarks; i++)
  {
    tft.drawLine(mark[i], originY, mark[i], minorSizeY, graphColor);
  }

  // draw lable names
  for(int i = 0; i < numberOfMarks; i += 2)
  {
    tft.setCursor((mark[i] + 6), (originY + 10));
    tft.setTextColor(graphColor);
    tft.setTextSize(2);
    tft.println(graphBlock[i / 2]);
  }

  // draw numbers
  for(int i = 0; i < 6; i++)
  {
    tft.drawLine(originX, (originY - number[i]), minorSizeX, (originY - number[i]), graphColor);
  }

  // draw number values
  for(int i = 0; i < 6; i++)
  {
    tft.setCursor((minorSizeX - 30), (number[i] + numberSize));
    tft.setTextColor(graphColor);
    tft.setTextSize(1);
    tft.println(val[i]);
  }
}

void graph()
{
  valueBlock[0] = analogRead(A15);
  valueBlock[1] = analogRead(A14);
  
  if(proDebug)
  {
    Serial.println(valueBlock[0]);
    Serial.println(valueBlock[1]);
    Serial.println("");
  }

  for(int i = 0; i < 2; i++)
  {
    posBlock[i] = map(valueBlock[i], 0, graphRange, originY, (originY - sizeY));
  }
  
  if(proDebug)
  {
    Serial.println(posBlock[0]);
    Serial.println(posBlock[1]);
    Serial.println("");
  }
  
  for(int i = 0; i < 2; i++)
  {
    if(posBlock[i] > (prevPosBlock[i] + 2) || posBlock[i] < (prevPosBlock[i] - 2) || firstRun)
    {
      tft.fillRect((mark[i * 2] - 1), ((originY - sizeY) - 20), (boxSize + 22), (sizeY + 20), WHITE);
    }
  }

  if(valueBlock[0] < 5)
  {
    tft.drawLine((originX + 20), (originY - 20), ((originX + sizeX) / 2), (originY - 20), graphColor);
  }

  if(valueBlock[1] < 5)
  {
    tft.drawLine((((originX + 20) * 2) + 25), (originY - 20), ((originX + sizeX)), (originY - 20), graphColor);
  }
    
  for(int i = 0; i < 2; i++)
  {
    if(posBlock[i] > (prevPosBlock[i] + 2) || posBlock[i] < (prevPosBlock[i] - 2) || firstRun)
    {
      tft.drawLine((mark[(i * 2) + 1]), originY, (mark[(i * 2) + 1] + 20), (originY - 20), BLACK);
      tft.drawLine((mark[(i * 2)] + 20), (posBlock[i] - 20), mark[(i * 2)], posBlock[i], BLACK);
      
      if(valueBlock[i] > 20)
      {
        tft.fillRect((mark[i * 2] + 1), (posBlock[i] + 1), (boxSize - 1), (originY - posBlock[i] - 1), blockColor);

        tft.drawLine((mark[(i * 2) + 1] + 20), (originY - 20), (mark[(i * 2) + 1] + 20), (posBlock[i] - 20), BLACK);
        tft.drawLine((mark[(i * 2) + 1] + 20), (posBlock[i] - 20), mark[(i * 2) + 1], posBlock[i], BLACK);
        tft.drawLine((mark[(i * 2)] + 20), (posBlock[i] - 20), (mark[(i * 2) + 1] + 20), (posBlock[i] - 20), BLACK);
  
        tft.drawLine(mark[(i * 2)], posBlock[i], (mark[(i * 2) + 1]), posBlock[i], BLACK);
        tft.drawLine(mark[(i * 2)], originY, mark[(i * 2)], posBlock[i], BLACK);
        tft.drawLine(mark[(i * 2) + 1], originY, mark[(i * 2) + 1], posBlock[i], BLACK);
  
        tft.fillTriangle((mark[(i * 2) + 1] + 1), (originY - 2), ((mark[(i * 2) + 1] + 20) - 1), ((originY - 20) - 1), ((mark[(i * 2) + 1] + 20) - 1), ((posBlock[i] - 20) + 2), blockColor);
        tft.fillTriangle((mark[(i * 2) + 1] + 1), posBlock[i], (mark[(i * 2) + 1] + 1), (originY - 2), ((mark[(i * 2) + 1] + 20) - 1), ((posBlock[i] - 20) + 2), blockColor);
  
        tft.fillTriangle((mark[(i * 2) + 1]), (posBlock[i] - 1), ((mark[(i * 2) + 1] + 20) - 2), ((posBlock[i] - 20) + 1), (mark[(i * 2)] + 2), (posBlock[i] - 1), blockColor);
        tft.fillTriangle((mark[(i * 2)] + 1), (posBlock[i] - 1), ((mark[(i * 2) + 1] + 20) - 2), ((posBlock[i] - 20) + 1), ((mark[(i * 2)] + 20)), ((posBlock[i] - 20) + 1), blockColor);
      }
    }
  }

  for(int i = 0; i < 2; i++)
  {
    if(posBlock[i] > (prevPosBlock[i] + 2) || posBlock[i] < (prevPosBlock[i] - 2) || firstRun)
    {
      prevPosBlock[i] = posBlock[i];
      if(displayValues)
      {
        tft.setCursor((mark[i * 2] + 5), (originY - 20));
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.println(valueBlock[(i *2) / 2]);
      }
    }
  }
  
  firstRun = false;
}

void setup()
{
  if(proDebug)
  {
    Serial.begin(9600);
    while(!Serial) {};
  }
  
  tft.reset();
  delay(500);
  uint16_t identifier = tft.readID();
  identifier=0x9341;

  tft.begin(identifier);
  tft.setRotation(1);

  drawHome();
  drawGraph();
}

void loop()
{
  graph();
  delay(200);
}
