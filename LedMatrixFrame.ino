/**********************************************
 * PinOut
 *  0-   Right touch button
 *  1-   Return touch button
 *  2-   SmartMatrix 
 *  3-   SmartMatrix 
 *  4-   SmartMatrix 
 *  5-   SmartMatrix 
 *  6-   SmartMatrix 
 *  7-   NEO pixel rings
 *  8-   SmartMatrix
 *  9-   ESP8266
 *  10-  ESP8266
 *  11-  sound or SD card
 *  12-  sound or SD card
 *  13-  sound or SD card
 *  14-  SmartMatrix 
 *  15-  IR sensor
 *  16-  Select touch button
 *  17-  ESP enable
 *  18-  Left touch button
 *  19-  Down touch button
 *  20-  SmartMatrix 
 *  21-  SmartMatrix 
 *  22-  Setup touch button
 *  23-  Up touch button
 * 
 *  
 * 
 ***********************************************/
#include <SmartLEDShieldV4.h> //for matrix setup and control
#include <TimeLib.h> //real time clock (RTC) functionality

//#include <LMF_Setup.h>
//#include <LMF_Options.h>
//test code begin*********************************************************************************************
#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 64;        // known working: 16, 32, 48, 64
const uint8_t kMatrixHeight = 32;       // known working: 32, 64, 96, 128
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);

const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);
//test code end*****************************************************************************************************



const int8_t numberOfButtons = 7;
enum  buttonLabel {Up, Left, Select, Right, Setup, Down, Return };
const int8_t touchButton[] = {23, 18, 16, 0, 22, 19, 1};
//const *char[] buttonLabel = {"Up", "Left", "Select", "Right",
//                            "Setup", "Down", "Return" };

const int8_t touchSampleSize = 5;


void setup() {
//test code begin********************************************
    matrix.addLayer(&backgroundLayer);
    matrix.addLayer(&indexedLayer); 
    matrix.begin();

  //  indexedLayer.setFont(gohufont11b);
  //  indexedLayer.setIndexedColor(1, {0,0,0});
    matrix.setBrightness(255);
Serial.begin(9600);

    
//test code end***********************************************
}

void loop() {
     //Serial.print(millis());
     //Serial.print("    ");
//maintain touch calibration
    static uint32_t nextCalibrationRead = 0;
    static uint8_t buttonPushed = 0;
    static uint8_t samplePoolFilled = 0;
    static uint16_t touchRange[numberOfButtons][touchSampleSize];
    //check all the buttons once per second spread evenly in time
    if( millis() >= nextCalibrationRead && buttonPushed == 0){
        static uint8_t i = 0;
        static uint8_t j = 0;

        touchRange[i][j] = touchRead(touchButton[i]);

        Serial.print(i);
        Serial.print("  ");
        Serial.print(j);
        Serial.print("  ");
        Serial.println(touchRange[i][j]);


        
        //set next read time
        if(samplePoolFilled == 1){
            nextCalibrationRead = millis() + (1000 / numberOfButtons);
        }
        //roll i and j over
        if( ++i == numberOfButtons){
            i = 0;
            if( ++j == touchSampleSize){
                j = 0;
                samplePoolFilled = 1;
            }
        }
    }
    
    //variables used to capture the min and max values for the buttons at rest
    static uint32_t nextMinMaxCheck = 0;
    static uint32_t touchMinMax[numberOfButtons][2];
    static uint16_t thresholdBuffer[numberOfButtons]; 
    static uint8_t minMaxDone = 0;
    //set min/max for buttons once after sample ing is done and again every n time
    if( millis() > nextMinMaxCheck && samplePoolFilled){
        static uint8_t p = 0;

        Serial.print(p);
        Serial.print("   ");

        //reset the min/max to something in the range
        touchMinMax[p][0] = touchRange[p][0];
        touchMinMax[p][1] = touchRange[p][0];

        //iterate trough the gathered readings saving the highest and lowest values. 
        for( uint8_t j = 0; j < touchSampleSize; j++){
            if( touchRange[p][j] < touchMinMax[p][0]){
                touchMinMax[p][0] = touchRange[p][j];
            }else if( touchRange[p][j] > touchMinMax[p][1]){
                touchMinMax[p][1] = touchRange[p][j];
            }
        }

        Serial.print(touchMinMax[p][0]);
        Serial.print("    ");
        Serial.print(touchMinMax[p][1]);
        Serial.println("");
        //set the threshold for deteting a button push
        thresholdBuffer[p] = touchMinMax[p][1] + 40;//+ ( 5 * ( touchMinMax[i][1] - touchMinMax[i][0]));

        p++;

        if( p == numberOfButtons){
            p = 0;
            minMaxDone = 1;
        }

        if( minMaxDone == 1){
            nextMinMaxCheck = millis() + (10000 / numberOfButtons);
        }
    }
    static uint8_t buttonNumberPressed[numberOfButtons];
    //check for button touches
    buttonPushed = 0;
    if( minMaxDone == 1){
    for( int i = 0; i < numberOfButtons; i++){
        if( touchRead(touchButton[i]) > thresholdBuffer[i]){
            buttonNumberPressed[i] = 1;
            buttonPushed = 1;
        }else{
            buttonNumberPressed[i] = 0;
        }
    }}

    //check for IR input
     
    //update time

    //update flags

    //sellect animation based on flags

    //reset

    //test code************************************

  static int firstReactionToPress = 1;
  static int x = 32;
  static int y = 16;
  static int r = 0;
  static long color = 0;
  static int g = 0;
  static int b = 0;
  if( buttonPushed && firstReactionToPress ){
    if( buttonNumberPressed[Up]){
        y--;
        //firstReactionToPress = 0;
    }else if( buttonNumberPressed[Down]){
        y++;
        //firstReactionToPress = 0;
    }else if( buttonNumberPressed[Right]){
        x++;
        //firstReactionToPress = 0;
    }else if( buttonNumberPressed[Left]){
        x--;
        //firstReactionToPress = 0;
    }else if( buttonNumberPressed[Select]){
        x = 32;
        y = 16;
        //firstReactionToPress = 0;
    }else if( buttonNumberPressed[Setup]){
        color++;
        firstReactionToPress = 0;
    }else if( buttonNumberPressed[Return]){
        color--;
        firstReactionToPress = 0;
//    }else{ 
//        firstReactionToPress = 1;
    }
  }else if(!buttonPushed){
    firstReactionToPress = 1;
  }
  if(x > 61){
    x = 61;
  }
  if(x < 0){
    x = 0;
  }
  if(y > 29){
    y = 29;
  }
  if(y < 0){
    y = 0;
  }
  if( color > 5){
    color = 0;
  }
  if( color < 0){
    color = 5;
  }

  switch(color){
    case 0:
        r=255;g=0;b=0;break;
    case 1:
        r=255;g=255;b=0;break;
    case 2:
        r=0;g=255;b=0;break;
    case 3:
        r=0;g=255;b=255;break;
    case 4:
        r=0;g=0;b=255;break;
    case 5:
        r=255;g=0;b=255;break;
  }
  backgroundLayer.fillScreen({0, 0, 255});
  //indexedLayer.fillScreen(0);
  if( minMaxDone == 1){backgroundLayer.fillRectangle(x,y,(x+3),(y+3),{r,g,b});}
  //x = (kMatrixWidth / 2) - (Image.width/2)+((i>0)? i*-1 :((i<-29)? -58+(i*-1): i));
  //y = (kMatrixHeight / 2) - (Image.height/2);
  //drawBitmap(x, y, (const gimp32x32bitmap*)&Image);
  
  //indexedLayer.drawString(30+i, kMatrixHeight / 2, 1, "@ScarceSam");
  backgroundLayer.swapBuffers();
  //indexedLayer.swapBuffers();
}
