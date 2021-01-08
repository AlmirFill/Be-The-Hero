#include <Wire.h>

const int NumReadings = 10;                    // number of reading
int Index_1 = 0;                                 // index
int Index_2 = 0;                                 // index

int Ph1Readings[NumReadings];                  // array for store PH1 readings
double ph_ch1[NumReadings];
double Ph1Total = 0;                              // PH1 running total
double Ph1Average = 0;                            // PH1 average reading

int Ph2Readings[NumReadings];                  // array for store PH2 readings
double ph_ch2[NumReadings];
double Ph2Total = 0;                              // PH2 running total
double Ph2Average = 0;                            // PH2 average reading

double Ph7Buffer = 7;                          // For PH7 buffer solution's PH value , 7 or 6.86
double Ph4Buffer = 1;                          // For PH4 buffer solution's PH value , 4 or 4.01

double Ph7Ch1Reading = 373;                       // PH7 Buffer Solution Reading.
double Ph7Ch2Reading = 291;                       // PH7 Buffer Solution Reading.

double Ph4Ch1Reading = 428;                       // PH4 Buffer Solution Reading.
double Ph4Ch2Reading = 441;                       // PH4 Buffer Solution Reading.

double Ph1Ratio = 0;                           // PH1 Step
double Ph1Value = 0;                           // Ph1 Value in Human Reading Format after calculation

double Ph2Ratio = 0;                           // PH2 Step
double Ph2Value = 0;

void setup()
{
  Wire.begin(0x0A);                
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);          
}

void receiveEvent(int numBytes)
{
  while (1 < Wire.available())
  { 
    char td = Wire.read();
    Serial.print(td);        
  }
  int valor = Wire.read();    
  Serial.println("");
  Serial.println(valor, HEX);   
  Serial.println(valor, BIN);
  Serial.println(valor, OCT);
  Serial.println(valor);
  
}