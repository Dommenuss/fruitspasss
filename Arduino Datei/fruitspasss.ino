
//Chaine de caractères pour recevoir les données.
String bluetoothMessage = "" ;
int LED_PIN = 13 ;
int BUTTON_PIN = 7 ; 
int BUTTON_PIN2 = 8 ; 
boolean boutonPresse = false ; 
boolean ledOn = false ; 

//Initialisation de la communication 
void setup() {
  //Initialise la vitesse communication  arduino and the shield 
  Serial.begin( 9600 ) ; 
  //Initialise the builtin led 
  pinMode( LED_PIN, OUTPUT ) ;
  pinMode( BUTTON_PIN, INPUT ) ;
  pinMode( BUTTON_PIN2, INPUT ); 
}

void loop() {


  //Bluetoothzeug
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

  //Si le bouton est pressé 
  if( digitalRead( BUTTON_PIN ) == HIGH || digitalRead ( BUTTON_PIN2 ) ) {
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
      //Stability Fix : pour éviter les rebonds de manière logiciel.
      delay(100) ;
    }
  } else {
    //Le bouton a été relaché 
    boutonPresse = false ; 
  }
}
