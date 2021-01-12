/*   Bibliotecas ultilizadas para controlar o display*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* iniciando configuração do display */
#define SCREEN_WIDTH 128 // OLED display largura, em pixels
#define SCREEN_HEIGHT 32 // OLED display altura, in pixels

#define OLED_RESET     1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
    ADICIONAR AQUI A IMPORTAÇÃO DAS LIBS DO WEBSOKET
*/

//Variavel de controle que referesse a inicialização do exame
bool exameOn = false;

String a;
String b;
int aux = 0;

void setup(){
    displayInitialize();
}

void loop(){
  //showData(); 
  getPh(); 
}

void getPh(){
  char* ph = "a";
  Wire.requestFrom(44, 12);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read();
    if(c == "a"){
      ph = "a";
    }else if(c == "a"){
      ph = "a";
    }
    
    if(ph == "a"){
      char c = Wire.read(); // receive a byte as character
      aux ++;
      if(aux <=2){
        a += c;
      }
      else if(aux > 2){
        b += c;
        if(aux > 4){
          beforeInitExameDisplay(a, b, "", "");
          a = "";
          b = "";
        }
      }else if(ph == "a"){
        char c = Wire.read(); // receive a byte as character
        aux ++;
        if(aux > 5){
          a += c;
        }
        else if(aux > 7){
          b += c;
          if(aux > 9){
            beforeInitExameDisplay("", "", a, b);
            a = "";
            b = "";
            aux = 0;
          }
      }

    }
  }
  delay(200);
}

}
/* inicialização do display*/
void displayInitialize(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // limpando o buffer
  display.clearDisplay();
  Wire.begin();

  // Show the display buffer on the screen. You MUST call display() before
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
}

// mostra se o exame esta iniciado
bool getExameOn(){
  bool stateReturn;
  
  stateReturn = exameOn;

  return stateReturn;
}

// Demostraçao de dados no display
void showData(){
    if(getExameOn()){
        beforeInitExameDisplay("9", ".22", "9", ".22");
    }else{
        afterInitExameDisplay();
    }
    delay(100);
    
}

// dados antes do exame iniciar
void beforeInitExameDisplay(String ph1, String ph1Float, String ph2, String ph2Float){
    
    display.clearDisplay();

    display.setCursor(17,2);
    display.setTextSize(1);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.println(F("CANAL 1  CANAL 2"));
    
    display.setTextSize(2); 
    // Display PH Data 
    display.setCursor(24,18);
    display.println(ph1);

    display.setTextSize(1);
    display.setCursor(46,18);
    display.println(ph1Float);

    display.setTextSize(2);
    display.setCursor(78, 18);
    display.println(ph2);  
                            // display PH1 value
    display.setTextSize(1);
    display.setCursor(88,18);
    display.println(ph2Float);

    display.display();
}

//dados depois do exame começar
void afterInitExameDisplay(){
    
    display.clearDisplay();

    display.setCursor(20,12);
    display.setTextSize(1.5);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.println(F("INICIAR EXAME"));

    display.display();
    
}
