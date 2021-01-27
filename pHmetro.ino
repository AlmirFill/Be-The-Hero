//  #include <SoftwareSerial.h>
//  #include <LiquidCrystal_I2C.h>
 #include <EEPROM.h>
 #include <SPI.h>
 #include <SD.h>
 #include <Wire.h>
 #include "RTClib.h"
 RTC_DS1307 rtc;

 //Pino CS do modulo cartao SD
 const int chipSelect = 10;
 File dataFile;

 DateTime now = rtc.now();

// // //LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
//  LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);   // Inicializa o display no endereco 0x27

// // // define some values used by the panel and buttons

// #define btnFood 6
#define btnSymptom 7
#define btnLye 5

#define TIMER_LONG 5000
#define TIMER_LITLE 500

long timerInit;

bool exameOn = false;


// const int NumReadings = 10;                    // number of reading
// int Index_1 = 0;                                 // index
// int Index_2 = 0;                                 // index

// int Ph1Readings[NumReadings];                  // array for store PH1 readings
// double ph_ch1[NumReadings];
// double Ph1Total = 0;                              // PH1 running total
// double Ph1Average = 0;                            // PH1 average reading

// int Ph2Readings[NumReadings];                  // array for store PH2 readings
// double ph_ch2[NumReadings];
// double Ph2Total = 0;                              // PH2 running total
// double Ph2Average = 0;                            // PH2 average reading

// double Ph7Buffer = 7;                          // For PH7 buffer solution's PH value , 7 or 6.86
// double Ph4Buffer = 1;                          // For PH4 buffer solution's PH value , 4 or 4.01

// double Ph7Ch1Reading = 373;                       // PH7 Buffer Solution Reading.
// double Ph7Ch2Reading = 291;                       // PH7 Buffer Solution Reading.

// double Ph4Ch1Reading = 428;                       // PH4 Buffer Solution Reading.
// double Ph4Ch2Reading = 441;                       // PH4 Buffer Solution Reading.

// double Ph1Ratio = 0;                           // PH1 Step
// double Ph1Value = 0;                           // Ph1 Value in Human Reading Format after calculation

// double Ph2Ratio = 0;                           // PH2 Step
// double Ph2Value = 0;                           // Ph2 Value in Human Reading Format after calculation

// long previousMillis = 0;                       // Variável de controle do tempo
// long Interval = 4000;                          // Tempo em ms do intervalo a ser executado

// boolean setDisplay = true;

int reading_1(){                                 // Reading PH Data
  // Samplin PH Value     
  Ph1Total = Ph1Total - Ph1Readings[Index_1];    // subtract the last reading:    
  
  Ph1Readings[Index_1] = analogRead(3);          // read from the sensor : PH1
   
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

void setup(){
  initialize();
}

void loop(){ 
  reading_1();
  reading_2();
  
  showData();

  if(getExameOn()){
    saveData();
    handleBtnLye();
    handleBtnFood();
    handleBtnSymptom();
  }

  delay(1); 
}

void initialize(){
   lcd.begin(16, 2);    // start LCD library
   
   Serial.begin(9600);  // Serial Monitor

  pinMode(btnFood, INPUT);
  pinMode(btnLye, INPUT);
  pinMode(btnSymptom, INPUT);
   
    //----------------Inicializa o RTC-------------------   
    if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
      // This will reflect the time that your sketch was compiled
      rtc.adjust(DateTime(__DATE__, __TIME__));
    }
    rtc.adjust(DateTime(__DATE__, __TIME__));
    
    //---------------------------------------------------
    
    //----------Inicializa o Modulo SD Card--------------
     Serial.println("Inicializando SD card...");
     if (!SD.begin(chipSelect)) {
      Serial.println("Inicializacao falhou!");
      return;
     }
     Serial.println("Inicializacao finalizada.");
    //---------------------------------------------------


   for (int PhThisReading = 0; PhThisReading < NumReadings; PhThisReading++){        // initialize all the Ph readings to 0:
     Ph1Readings[PhThisReading] = 0;
     Ph2Readings[PhThisReading] = 0;
   }  
   
   Ph1Ratio = (Ph4Ch1Reading - Ph7Ch1Reading) / (Ph7Buffer - Ph4Buffer);                 // Calculate Ph1 Ratio   
   Ph2Ratio = (Ph4Ch2Reading - Ph7Ch2Reading) / (Ph7Buffer - Ph4Buffer);                 // Calculate Ph2 Ratio


}

bool getExameOn(){
  bool stateReturn;
  
  stateReturn = exameOn;

  return stateReturn;

}

void showData(){
  if(getExameOn()){
    lcd.clear();
    lcd.setCursor(3,0);                        // set the LCD cursor position 
    lcd.print("Ch1");
    lcd.setCursor(10,0); 
    lcd.print("Ch2");
            
    // Display PH Data 
    lcd.setCursor(3,1);
    lcd.print(Ph1Value,1);                        // display PH1 value
      
    // Display PH Data 
    lcd.setCursor(10,1);
    lcd.print(Ph2Value,1);
  }else{
    lcd.clear();
    
    lcd.setCursor(9,0);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);

    lcd.setCursor(15,0);
    lcd.print("*");
    lcd.setCursor(1,1);                        // set the LCD cursor position 
    lcd.print("|inicia exame|");
    delay(500);
  }                       // display PH2 value
}

void saveData(){
  //Send do SDCard
    

    dataFile = SD.open("dataPH.txt", FILE_WRITE);  //define 
  
    // if the file opened okay, write to it:
     // if the file opened okay, write to it:
    if (dataFile  && getExameOn()) {
      
      //Send to data.txt

      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);
      dataFile.print(";");
      dataFile.print(Ph1Value);
      dataFile.print(";");
      dataFile.println(Ph2Value);
      
      //Send to Serial
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.print("; ");
      Serial.print(Ph1Value);
      Serial.print("; ");
      Serial.println(Ph2Value);

      // close the file data.txt:
      dataFile.close();
    }
}

void calibrarPh7(){
  Ph7Ch1Reading = Ph1Average;
  Ph7Ch2Reading = Ph2Average;
}
