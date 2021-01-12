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
  Wire.begin(44);                
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);

  for (int PhThisReading = 0; PhThisReading < NumReadings; PhThisReading++){        // initialize all the Ph readings to 0:
     Ph1Readings[PhThisReading] = 0;
     Ph2Readings[PhThisReading] = 0;
   }  
   
   Ph1Ratio = (Ph4Ch1Reading - Ph7Ch1Reading) / (Ph7Buffer - Ph4Buffer);                 // Calculate Ph1 Ratio   
   Ph2Ratio = (Ph4Ch2Reading - Ph7Ch2Reading) / (Ph7Buffer - Ph4Buffer);                 // Calculate Ph2 Ratio        
}

void loop(){
  reading_1();
}

void requestEvent() {
  Serial.println(String(Ph1Value).c_str());
  Wire.write(String(Ph1Value).c_str());
}

float reading_1(){                                 // Reading PH Data
  // Samplin PH Value     
  Ph1Total = Ph1Total - Ph1Readings[Index_1];    // subtract the last reading:    
  
  Ph1Readings[Index_1] = analogRead(A1);          // read from the sensor : PH1
   
  Ph1Total = Ph1Total + Ph1Readings[Index_1];     // add the reading to the ph1 total:
       
  Index_1 = Index_1 + 1;                           // advance to the next position in the array:                

  if (Index_1 >= NumReadings){                   // if we're at the end of the array...         
    Index_1 = 0;                                 // ...wrap around to the beginning:                         
    Ph1Average = Ph1Total / NumReadings;       // calculate the average1:

    for(int i = 0; i<NumReadings; i++){
      ph_ch1[i] = (Ph7Ch1Reading - Ph1Readings[i]) / Ph1Ratio + Ph7Buffer;
    }
    
  }

  Ph1Value = (Ph7Ch1Reading - Ph1Average) / Ph1Ratio + Ph7Buffer;    // Calculate PH ch1

}

int reading_2(){                                 // Reading PH Data
  // Samplin PH Value     
  Ph2Total = Ph2Total - Ph2Readings[Index_2];      
  
  Ph2Readings[Index_2] = analogRead(2);          // read from the sensor : PH2  
   
  Ph2Total = Ph2Total + Ph2Readings[Index_2];     // add the reading to the ph2 total:
       
  Index_2 = Index_2 + 1;                           // advance to the next position in the array:                

  if (Index_2 >= NumReadings){                   // if we're at the end of the array...         
    Index_2 = 0;                                 // ...wrap around to the beginning:                         
    Ph1Average = Ph1Total / NumReadings;       // calculate the average1:
    Ph2Average = Ph2Total / NumReadings;       // calculate the average2:

    for(int i = 0; i<NumReadings; i++){
      ph_ch2[i] = (Ph7Ch2Reading - Ph2Readings[i]) / Ph1Ratio + Ph7Buffer;
    }
    
  }
  
  Ph2Value = (Ph7Ch2Reading - Ph2Average) / Ph2Ratio + Ph7Buffer;    // Calculate PH ch2

}


void receiveEvent(int howMany)
{

}
