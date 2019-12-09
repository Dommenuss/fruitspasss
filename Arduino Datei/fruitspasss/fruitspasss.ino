
//include libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


//General
int winByPoints = 5;
String bluetoothMessage = "" ;
int LED_PIN = 13 ;
int BUTTON_PIN = 7 ; 
int BUTTON_PIN2 = 8 ; 
boolean boutonPresse = false ; 
boolean ledOn = false ; 
unsigned long time_now = 0;
unsigned long inactiveTime = 0;
int player1count = 0;
int player2count = 0;
String currentWinner;



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
String player1wins = "Player 1 wins!";
String player2wins = "Player 2 wins!";
String fruitPhone;

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
boolean correctPress;


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

    //RECEIVES BANANE MESSAGE
    if( bluetoothMessage == "banane" ) {
      if(currentFruit == 0){
        correctPress = true;
        sendWinningPlayer();
      }else{
        correctPress = false;
        sendWinningPlayer();
      }
      fruitPhone = "banane";
      setDisplayToCorrectPlayer();
      updatePlayerCount();
      resetVariables();
      sendA();
      //setDisplayToDefault();
      delay(300);
      //THIS COULD BE A POSSIBLE ERROR SOURCE; THUS THE MESSAGE GETS LOST IN THESE 5 SECONS
      
    }


    //RECEIVES MELONE MESSAGE
    if( bluetoothMessage == "melone" ) {
      if(currentFruit == 1){
        correctPress = true;
        sendWinningPlayer();
      }else{
        correctPress = false;
        sendWinningPlayer();
      }
      fruitPhone = "melone";
      setDisplayToCorrectPlayer();
      updatePlayerCount();
      resetVariables();
      sendA();
      //setDisplayToDefault();
      delay(300);
    }

    //RECEIVES APFEL MESSAGE
    if( bluetoothMessage == "apfel" ) {
       if(currentFruit == 2){
        correctPress = true;
        sendWinningPlayer();
      }else{
        correctPress = false;
        sendWinningPlayer();
      }
      fruitPhone = "apfel";
      setDisplayToCorrectPlayer();
      updatePlayerCount();
      resetVariables();
      sendA();
      //setDisplayToDefault();
      delay(300);
    }
  
    //RECEIVES MANGO MESSAGE
    if( bluetoothMessage == "mango" ) {
       if(currentFruit == 3){
       correctPress = true;
        sendWinningPlayer();
      }else{
        correctPress = false;
        sendWinningPlayer();
      }
      fruitPhone = "mango";
      setDisplayToCorrectPlayer();
      updatePlayerCount();
      resetVariables();
      sendA();
      //setDisplayToDefault();
      delay(300);
    }

    //RECEIVES KIWI MESSAGE
    if( bluetoothMessage == "kiwi" ) {
       if(currentFruit == 3){
        correctPress = true;
        sendWinningPlayer();
       }else{
        correctPress = false;
        sendWinningPlayer();
       }
      fruitPhone = "kiwi";
      setDisplayToCorrectPlayer();
      updatePlayerCount();
      resetVariables();
      sendA();
      //setDisplayToDefault();
      delay(300);
    }


    
    //Reset message to ""
    bluetoothMessage = "" ;
  }

 
  if(phoneStartsGame){
        //starts countdown for game
        firstButtonPressed = true;
  }







  //Handles onclick button for player 1
  
  if( digitalRead( BUTTON_PIN ) == HIGH && !playerTwoButtonPressed) {
    playerOneButtonPressed = true;

    //THIS CHECKS IF BUTTON WAS PRESSED IN THE RIGHT TIME AND ASKS PHONE FOR ITS FRUIT IMAGE
    if (!boutonPresse && firstButtonPressed) {    
        //SEND QUESTION WHAT FRUIT IS SHWON ON PHONE BY SENDING F FOR FRUCHT?
        if(currentFruit != 5){
          Serial.print("F"); 
          currentFruitName = fruitArray[currentFruit]; 
        }else{
          Serial.print("O");
          correctPress = true;
          setDisplayToCorrectPlayer();
          updatePlayerCount();
          resetVariables();
          sendA();
          //setDisplayToDefault();
          delay(200);
         
        }
    }
    //THIS SENDS A MESSAGE TO THE PHONE TO START THE GAME, A FOR ANFANGEN
    if(!boutonPresse && !firstButtonPressed){
      //When a button is pressed for the first time, to start the countdown
      Serial.print("A");
     
    }
      //On note qu'on vient de traiter le fait que le bouton a été pressé ( front montant ) 
      boutonPresse = true ;
      inactiveTime = millis();
      waiting = false;
      //Delay of 500 eliminates button bouncing
      delay(400) ;
    
  } else {
    //Le bouton a été relaché 
    boutonPresse = false ; 
   
  }







  //Handles onclick button for player 2
  
 if( digitalRead( BUTTON_PIN2 ) == HIGH && !playerOneButtonPressed) {
    playerTwoButtonPressed = true;

      //THIS CHECKS IF BUTTON WAS PRESSED IN THE RIGHT TIME AND ASKS PHONE FOR ITS FRUIT IMAGE
    if (!boutonPresse && firstButtonPressed) {    
        //SEND QUESTION WHAT FRUIT IS SHWON ON PHONE BY SENDING F FOR FRUCHT?
        if(currentFruit != 5){
          Serial.print("F"); 
          currentFruitName = fruitArray[currentFruit]; 
        }else{
          Serial.print("T");
          correctPress = true;
          setDisplayToCorrectPlayer();
          updatePlayerCount();
          resetVariables();
          sendA();
          //setDisplayToDefault();
          delay(200);
         
        }
    }
      
    //THIS SENDS A MESSAGE TO THE PHONE TO START THE GAME, A FOR ANFANGEN
    if(!boutonPresse && !firstButtonPressed){
      //When a button is pressed for the first time, to start the countdown
      Serial.print("A");
       
    }
      
      //On note qu'on vient de traiter le fait que le bouton a été pressé ( front montant ) 
      boutonPresse = true ;
      inactiveTime = millis();
      waiting = false;
      //Delay of 500 eliminates button bouncing
      delay(400) ;
    
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
      playerOneButtonPressed = false;
      playerTwoButtonPressed = false;
      gameAlreadyStarted = true;
      delay(1000);
      //Sends phone an "S" for Start
      Serial.print("S");
    }




  


  //Changes name of fruit every 2000ms, updates player count automatically
  //millis() provides a non blocking behaviour so that button presses can be recognized
  //after 40 seconds of inactivity the display blocks and shows a message
  //This code is already finished and should not be changed
  
  if(millis() > time_now + 2000 && phoneStartsGame && !playerOneButtonPressed && !playerTwoButtonPressed){
    if(millis() > inactiveTime + 60000 && !waiting){
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
void setDisplayToCorrectPlayer(){
      
      if(correctPress && playerOneButtonPressed){
        currentWinner = "P1";
      }else if(correctPress && playerTwoButtonPressed){
        currentWinner = "P2";
      }else if(!correctPress && playerOneButtonPressed){
        currentWinner = "P2";
      }else if(!correctPress && playerTwoButtonPressed){
        currentWinner = "P1";
      }else{
        //Game Error
        Serial.print("E");
      }
      String debug;
      if(correctPress == 0){
         debug = "false";
      } else {
        debug = "right";
      }
    
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Point for " +currentWinner+"!");
        lcd.setCursor(0,1);
        lcd.print(currentFruitName + "|" + fruitPhone + "|" + debug);
        delay(3000);
}


void resetVariables(){
        phoneStartsGame = false;
        firstButtonPressed = true;
        gameFinished = false;
        playerOneButtonPressed = false;
        playerTwoButtonPressed = false;
        gameAlreadyStarted = false;
        buttonInGamePressed = false;
        newFruit = true;
        on = false;
        waiting = false;
        
}

void sendA(){

  Serial.print("A");
}


void setDisplayToDefault(){
       
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print(start);
        lcd.setCursor(1,1);
        lcd.print(pressButton);
}


void sendWinningPlayer(){
  //TODO: send correct player: O = ONE, T = TWO
  if(correctPress && playerOneButtonPressed){
    Serial.print("O");
  }else if(correctPress && playerTwoButtonPressed){
    Serial.print("T");
  }else if(!correctPress && playerOneButtonPressed){
    Serial.print("T");
  }else if(!correctPress && playerTwoButtonPressed){
    Serial.print("O");
  }else{
    //Game Error
    Serial.print("E");
  }
  
}

void resetCounter(){
  player1count = 0;
  player2count = 0;
}



void updatePlayerCount(){
  if(correctPress && playerOneButtonPressed){
        player1count = player1count + 1;
  }else if(correctPress && playerTwoButtonPressed){
        player2count = player2count + 1;
  }else if(!correctPress && playerOneButtonPressed){
        player2count = player2count + 1;
  }else if(!correctPress && playerTwoButtonPressed){
        player1count = player1count + 1;
  }
  if(player1count >= winByPoints){
    if(!waiting){
     Serial.print("Y");
     resetVariables();
     
     resetCounter();
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Player one wins!");
     lcd.setCursor(1,1);
     lcd.print("... next round?");
     firstButtonPressed = false;
     delay(10000);
     
     setDisplayToDefault();
     
    } 
    //TODO, skip to winning screen
  }
  if(player2count >= winByPoints){
    if(!waiting){
     Serial.print("Z");
     resetVariables();
     
     resetCounter();
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Payer two wins!");
     lcd.setCursor(1,1);
     lcd.print("... next round?");
     firstButtonPressed = false;
     delay(10000);
     
     setDisplayToDefault();
     
    }
  }
}
