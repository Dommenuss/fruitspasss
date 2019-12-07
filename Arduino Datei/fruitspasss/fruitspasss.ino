
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
int currentFruit;
String currentFruitName;

//Fruitarray
String fruitArray[6] = {banane, melone, apfel, mango, kiwi, bonus};

//GUI Offset per Fruit
int bananeOffset = 5;
int meloneOffset = 5;
int apfelOffset = 5;
int mangoOffset = 5;
int kiwiOffset = 6;
int ponusOffset = 5;

//Connection
boolean phoneStartsGame = false;


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





  //1. Arduino sends "start" to mobile phone = signal for second screen (Check)

  //2. Game running, if button pressed arduino sends button pressed and stops both and asks phone which fruit is shown
  //2. If latency isues check last shown fruit


  //3. Update player count correctly

  //4. After winning check arduino waits for few seconds and sends starting message to mobile phone, if playercount = 10, signal for winning screen

  //5. At the end button continues and all variables are resetted, mobile phone shows introduction

  

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
    if( bluetoothMessage == "OK" ) {
      phoneStartsGame = true;
    }
    if( bluetoothMessage == "E" ) {
       digitalWrite( LED_PIN, LOW ) ;
    }   

    //RECEIVES BANANE MESSAGE
    if( bluetoothMessage == "banane" ) {
      if(currentFruit == 0){
        sendWinningPlayer();
      }else{
        sendWrongPlayer();
      }
      updatePlayerCount();
      setDisplayToCorrectPlayer("P1");
      delay(5000);
      setDisplayToDefault();
      //THIS COULD BE A POSSIBLE ERROR SOURCE; THUS THE MESSAGE GETS LOST IN THESE 5 SECONS
      
    }


    //RECEIVES MELONE MESSAGE
    if( bluetoothMessage == "melone" ) {
      if(currentFruit == 1){
        sendWinningPlayer();
      }else{
        sendWrongPlayer();
      }
      updatePlayerCount();
      setDisplayToDefault();
      delay(5000);
    }

    //RECEIVES APFEL MESSAGE
    if( bluetoothMessage == "apfel" ) {
       if(currentFruit == 2){
        sendWinningPlayer();
      }else{
        sendWrongPlayer();
      }
      updatePlayerCount();
      setDisplayToDefault();
      delay(5000);
    }
  
    //RECEIVES MANGO MESSAGE
    if( bluetoothMessage == "mango" ) {
       if(currentFruit == 3){
        sendWinningPlayer();
      }else{
        sendWrongPlayer();
      }
      updatePlayerCount();
      setDisplayToDefault();
      delay(5000);
    }

    //RECEIVES KIWI MESSAGE
    if( bluetoothMessage == "kiwi" ) {
       if(currentFruit == 3){
        sendWinningPlayer();
      }else{
        sendWrongPlayer();
      }
      updatePlayerCount();
      setDisplayToDefault();
      delay(5000);
    }


    
    //Reset message to ""
    bluetoothMessage = "" ;
  }

 
  if(phoneStartsGame){
        //starts countdown for game
        firstButtonPressed = true;
      }







  //Handles onclick button for player 1
  
  if( digitalRead( BUTTON_PIN ) == HIGH) {
    playerOneButtonPressed = true;
    
    //THIS SENDS A MESSAGE TO THE PHONE TO START THE GAME, A FOR ANFANGEN
    if(!boutonPresse && !firstButtonPressed){
      //When a button is pressed for the first time, to start the countdown
      Serial.print("A");
    }

    //THIS CHECKS IF BUTTON WAS PRESSED IN THE RIGHT TIME AND ASKS PHONE FOR ITS FRUIT IMAGE
    if (!boutonPresse && firstButtonPressed) {    
        //SEND QUESTION WHAT FRUIT IS SHWON ON PHONE BY SENDING F FOR FRUCHT?
        if(currentFruit == 5){
          updatePlayerCount();
        }else{
          
          Serial.print("F"); 
          currentFruitName = fruitArray[currentFruit]; 
         
        }


        
        //TODO: receive fruit name from phone
        //TODO: check if correct fruits, then change player1count accordingly
         //Adjust PlayerCount
      
      
      }
      //On note qu'on vient de traiter le fait que le bouton a été pressé ( front montant ) 
      boutonPresse = true ;
      inactiveTime = millis();
      waiting = false;
      //Delay of 500 eliminates button bouncing
      delay(500) ;
    
  } else {
    //Le bouton a été relaché 
    boutonPresse = false ; 
   
  }







  //Handles onclick button for player 2
  
 if( digitalRead( BUTTON_PIN2 ) == HIGH) {
    playerTwoButtonPressed = true;
    
    //THIS SENDS A MESSAGE TO THE PHONE TO START THE GAME, A FOR ANFANGEN
    if(!boutonPresse && !firstButtonPressed){
      Serial.print("A");
      //When a button is pressed for the first time, to start the countdown
     
    }

    //THIS CHECKS IF BUTTON WAS PRESSED IN THE RIGHT TIME AND ASKS PHONE FOR ITS FRUIT IMAGE
    if ( !boutonPresse && firstButtonPressed ) {    
        //SEND QUESTION WHAT FRUIT IS SHWON ON PHONE BY SENDING F FOR FRUCHT?
        Serial.print("F"); 
        //TODO: receive fruit name from phone
        //TODO: check if correct fruits, then change player1count accordingly
         //Adjust PlayerCount
        if(firstButtonPressed && !waiting){
          if(player2count >= 10){
            Serial.print("W2");
          }else{
            player2count = player2count + 1;
          }
      }
      }
      //On note qu'on vient de traiter le fait que le bouton a été pressé ( front montant ) 
      boutonPresse = true ;
      inactiveTime = millis();
      waiting = false;
      //Delay of 500 eliminates button bouncing
      delay(500) ;
    
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
      //Sends phone an "S" for Start
      Serial.print("S");
    }




  


  //Changes name of fruit every 2000ms, updates player count automatically
  //millis() provides a non blocking behaviour so that button presses can be recognized
  //after 40 seconds of inactivity the display blocks and shows a message
  //This code is already finished and should not be changed
  
  if(millis() > time_now + 2000){
    if(millis() > inactiveTime + 40000 && !waiting){
    waiting = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You dont want to");
    lcd.setCursor(0,1);
    lcd.print("play with me? :/");
    }else if (!waiting){
     time_now = millis();
     currentFruit = random(0, 6);
     lcd.clear();
     lcd.setCursor(5,0);
     lcd.print(fruitArray[currentFruit]);
     lcd.setCursor(0,1);
     lcd.print("P1: "+String(player1count));
     lcd.setCursor(11,1);
     lcd.print("P2: "+String(player2count));
  }
  }
  }


  
}



//Functions
void setDisplayToCorrectPlayer(String player){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(player);
        lcd.setCursor(1,1);
        lcd.print("was correct!");
}


void setDisplayToDefault(){
        phoneStartsGame = false;
        firstButtonPressed = false;
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print(start);
        lcd.setCursor(1,1);
        lcd.print(pressButton);
}


void sendWinningPlayer(){
  //TODO: send correct player: O = ONE, T = TWO
  Serial.print("O");
}

void sendWrongPlayer(){
  //TODO: send correct player
  Serial.print("P1F");
}


void updatePlayerCount(){
  if(playerOneButtonPressed){
    player1count = player1count + 1;
  }
  if(playerTwoButtonPressed){
    player2count = player2count + 1;
  }
  if(player1count >= 10){
    if(!waiting){
     Serial.print("W1");
     lcd.clear();
     lcd.setCursor(5,0);
     lcd.print("player 1");
     lcd.setCursor(1,1);
     lcd.print("wins!");
     delay(10000);
     lcd.clear();
     lcd.setCursor(5,0);
     lcd.print(start);
     lcd.setCursor(1,1);
     lcd.print(pressButton);
    } 
    //TODO, skip to winning screen
  }
  if(player2count >= 10){
    if(!waiting){
     Serial.print("W2");
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("player 2");
     lcd.setCursor(0,1);
     lcd.print("wins");
     delay(10000);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(start);
     lcd.setCursor(0,1);
     lcd.print(pressButton);
    //TODO, skip to winning screen
    }
  }
}
