void plotLines(void)
{
  scaleForGraph(pvStats.instantaneousPower,graphLine,pvStats.len);
  tftDrawGraph(GRAPHBORDER,GRAPH_Y_START,GRAPHBORDER+GRAPHWIDTH,GRAPHHEIGHT+GRAPH_Y_START,ILI9341_GREEN,graphLine);  
} 



void PrepareDataForGraph(void)
{
  int i;
  for(i=0;i<REQUEST_SIZE;i++)
  {
    pvStats.instantaneousPower[i] = random(0,20000);
  } 
}

void screenTask(void)
{
  //Clear dataplaces
  tft.fillRect(GRAPHBORDER,GRAPH_Y_START,GRAPHWIDTH,200-GRAPH_Y_START,ILI9341_BLACK);
  plotLines();
 
  
  if (pvStatus.energyGeneration<10000)
    tft.setTextSize(4);
  else
    tft.setTextSize(3);
  tft.setCursor(TEXT_START_X+12,22);
  tft.print(pvStatus.energyGeneration);
  tft.setCursor(TEXT_START_X+12,54);
  tft.setTextSize(2);
  tft.print("Watt");
  tft.setCursor(TEXT_START_X+12,SCREENHEIGTH/2+22);
  tft.setTextSize(4);
  
  if (pvStatus.instantaneousPower<1000)
  {
    tft.print(pvStatus.instantaneousPower);
    tft.setCursor(TEXT_START_X+12,SCREENHEIGTH/2+54);
    tft.setTextSize(2);
    tft.print("Wh  ");
  }
  else
  {
     tft.print(pvStatus.instantaneousPower/1000);
     tft.print(".");
     tft.print((pvStatus.instantaneousPower%1000)/100); 
     tft.setCursor(TEXT_START_X+12,SCREENHEIGTH/2+54);
     tft.setTextSize(2);
     tft.print("kWh");  
   }
}   
  
  


void tftSetup(void){
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(TEXT_START_X+1,0,SCREENWIDTH-1-TEXT_START_X,SCREENHEIGTH/2,INTERFACECOLOR);
  tft.drawRect(TEXT_START_X+1,SCREENHEIGTH/2,SCREENWIDTH-TEXT_START_X-1,SCREENHEIGTH/2,INTERFACECOLOR); 
  tft.drawLine(TEXT_START_X+2,1,TEXT_START_X+2,SCREENHEIGTH/2,INTERFACECOLOR);
  tftDrawGraphTimeLegend(GRAPHBORDER,GRAPH_Y_START,TEXT_START_X,220,ILI9341_WHITE,0,24,0,1200);

  tft.setTextSize(2);
  tft.setCursor(TEXT_START_X+12,4);
  tft.print("Cur. gen");
  tft.setCursor(TEXT_START_X+12,SCREENHEIGTH/2+4);
  tft.print("Tot. gen");
  delay(500);
}  
 
void tftDrawGraph(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t aColor,uint8_t data[])
{
   int x;
   int xlength=x1-x0-1;
   for (x=0;x<(xlength);x++)
   {
     tft.drawLine(x+x0, y1-data[x], x+1+x0, y1-data[x+1], aColor);   
   }
}

void tftDrawGraphTimeLegend(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t aColor, uint8_t minhour, uint8_t maxhour, uint16_t minY, uint16_t maxY)
{
  uint8_t i;
  //X line
  tft.drawLine(x0-2,y0,x0-2,y1,aColor);
  tft.drawLine(x0-1,y0,x0-1,y1,aColor);
  
  //Y Line
  tft.drawLine(x0-2,y1+1,x1,y1+1,aColor);
  tft.drawLine(x0-2,y1+2,x1,y1+2,aColor);

  String text = "Watt";
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);  
  for(i=0;i<4;i++)
  {
    tft.setCursor(x0-13, y0+i*16);
    tft.print(text[i]);
  }
  text = "Time";
  tft.setCursor(x1-5*11,y1+6);
  tft.print(text);
}

void scaleForGraph(const int* data_in,uint8_t* data_out,int aLength)
{
    int maxy=0;
    int i;
    for(i=0;i<aLength;i++)
    {
     if(data_in[i]>maxy)
     {
       maxy=data_in[i];
     }
    }
    if (aLength>GRAPHWIDTH)
    {
      for(i=0;i<GRAPHWIDTH;i++)
      {
        data_out[i] = map(data_in[i+(aLength-GRAPHWIDTH)],0,maxy,0,GRAPHHEIGHT);      
      }
    }
    else
    {
      for(i=0;i<aLength;i++)
      {
        data_out[i+(GRAPHWIDTH-aLength)] = map(data_in[i],0,maxy,0,GRAPHHEIGHT);      
      }
      for(i=0;i<(GRAPHWIDTH-aLength);i++)
      {
        data_out[i] = 0;
      }
    }    
}

