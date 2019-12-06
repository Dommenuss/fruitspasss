
//include libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


//Chaine de caractères pour recevoir les données.
String bluetoothMessage = "" ;
int LED_PIN = 13 ;
int BUTTON_PIN = 7 ; 
int BUTTON_PIN2 = 8 ; 
boolean boutonPresse = false ; 
boolean ledOn = false ; 
unsigned long time_now = 0;
unsigned long inactiveTime = 0;


//Screen
boolean firstButtonPressed = false;
boolean gameFinished = false;
boolean playerOneButtonPressed = false;
boolean playerTwoButtonPressed = false;
boolean gameAlreadyStarted = false;
boolean buttonInGamePressed = false;
boolean newFruit = true;
boolean on = false;
boolean waiting = false;


//Fruitnames
String banane = "Panane";
String melone = "Melone";
String apfel = "Opfel";
String mango = "Mongo";
String kiwi = "Kiwi";
String bonus = "Ponus";
String start = "Ready?";
String pressButton = "Press a Button!";
String ueff = "Üff!";
String player1wins = "Player 1 wins!";
String player2wins = "Player 2 wins!";
int player1count = 0;
int player2count = 0;

//Fruitarray
String fruitArray[6] = {banane, melone, apfel, mango, kiwi, bonus};

//GUI Offset per Fruit
int bananeOffset = 5;
int meloneOffset = 5;
int apfelOffset = 5;
int mangoOffset = 5;
int kiwiOffset = 6;
int ponusOffset = 5;


//LCD address = 0x27, 16 Chars, 2 Line Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Initialisation de la communication 
void setup() {
  //Initialise la vitesse communication  arduino and the shield 
  Serial.begin( 9600 ) ; 
  //Initialise the builtin led 
  pinMode( LED_PIN, OUTPUT ) ;
  pinMode( BUTTON_PIN, INPUT ) ;
  pinMode( BUTTON_PIN2, INPUT ); 


  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(5,0);
  lcd.print(start);
  lcd.setCursor(1,1);
  lcd.print(pressButton);
 
  
  
}

void loop() {


  //TODO: Bluetoothzeug
  //While there is data to read
  
  while( Serial.available() ) {
    //Stability fix : Small delay between reads 
    delay(10) ;    
    //Read one character 
    char c = Serial.read() ;
    //append character if one has been read 
    if( c != -1 ) {
      bluetoothMessage += c ;
    }
  }
  if( bluetoothMessage.length () > 0 ) {    
    if( bluetoothMessage == "A" ) {
       digitalWrite( LED_PIN, HIGH ) ;
    }
     if( bluetoothMessage == "E" ) {
       digitalWrite( LED_PIN, LOW ) ;
    }          
    //Reset message to ""
    bluetoothMessage = "" ;
  }






  //Handles onclick button for player 1
  
  if( digitalRead( BUTTON_PIN ) == HIGH) {
    //mais qu'il n'était pas pressé à l'execution précédente de loop()
     
    if ( !boutonPresse ) {    
      if( ledOn ) {
        Serial.print("E") ; 
        ledOn = false ; 
      } else {
        Serial.print("A") ; 
        ledOn = true ;       
      }
      //On note qu'on vient de traiter le fait que le bouton a été pressé ( front montant ) 
      boutonPresse = true ;

      //Adjust PlayerCount
      if(firstButtonPressed && !waiting){
      player1count = player1count + 1;
      }
      
     //When a button is pressed for the first time, to start the countdown
      firstButtonPressed = true;
      inactiveTime = millis();
      waiting = false;
      
      //Delay of 500 eliminates button bouncing
      delay(500) ;
    }
  } else {
    //Le bouton a été relaché 
    boutonPresse = false ; 
   
  }







  //Handles onclick button for player 2
  
  if( digitalRead( BUTTON_PIN2 ) == HIGH) {
    //mais qu'il n'était pas pressé à l'execution précédente de loop()
     
    if ( !boutonPresse ) {    
      if( ledOn ) {
        Serial.print("E") ; 
        ledOn = false ; 
      } else {
        Serial.print("A") ; 
        ledOn = true ;       
      }
      //On note qu'on vient de traiter le fait que le bouton a été pressé ( front montant ) 
      boutonPresse = true ;

      //TODO: Adjusts PlayerCount correctly every time a button is pressed 
      if(firstButtonPressed && !waiting){
      player2count = player2count + 1;
      }
     
      //When a button is pressed for the first time, to start the countdown
      firstButtonPressed = true;
      inactiveTime = millis();
      waiting = false;
      
      //Delay of 500 eliminates button bouncing
      delay(500) ;
    }
  } else {
    //Le bouton a été relaché 
    boutonPresse = false ; 
   
  }






  

  //TODO if time but not necessary: Display blinks "ready" before game starts
  if(!firstButtonPressed){
   
  }







  //Starts the countdown at the beginning of the game
  //This code is already finished and should not be changed
  
  else {
    if(firstButtonPressed && !gameAlreadyStarted){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Starting in 3");
      delay(1000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Starting in 2");
      delay(1000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Starting in 1");
      gameAlreadyStarted = true;
      delay(1000);
    }




  


  //Changes name of fruit every 2000ms, updates player count automatically
  //millis() provides a non blocking behaviour so that button presses can be recognized
  //after 20 seconds of inactivity the display blocks and shows a message
  //This code is already finished and should not be changed
  
  if(millis() > time_now + 2000){
    if(millis() > inactiveTime + 20000 && !waiting){
    waiting = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You dont want to");
    lcd.setCursor(0,1);
    lcd.print("play with me? :/");
    }else if (!waiting){
     time_now = millis();
     int shuffled = random(0, 6);
     lcd.clear();
     lcd.setCursor(5,0);
     lcd.print(fruitArray[shuffled]);
     lcd.setCursor(0,1);
     lcd.print("P1: "+String(player1count));
     lcd.setCursor(11,1);
     lcd.print("P2: "+String(player2count));
  }
  }
  }


  
}



//Functions
