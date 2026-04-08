#include <DHT.h>                                               // chargement de la librairie DHT pour le capteur
#include <Wire.h>                                              // chargement des bibliothèques de fonctions de l'écran
#include <LiquidCrystal.h>                                     // Controle de LCD
#include <FastLED.h>                                           // Librairie généraliste pour gérer le RGB

#define DATA_PIN    7                                          // pin 7 pour le rgb
#define LED_TYPE    WS2811                                   //*************************************
#define COLOR_ORDER GRB                                      //
#define NUM_LEDS    10                                       //
CRGB leds[NUM_LEDS];                                         // horrible truc pour init fastled

#define BRIGHTNESS         100                               //
#define FRAMES_PER_SECOND  60                               //*************************************

DHT dht(8,DHT11);                                              // définition d'un capteur DHT11 branché en entrée digitale 8
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                         // Définition des broches RS, E, et Data (DB4 à DB7)
bool initd = true;
float h = 30;
float t = 30;
unsigned long time_since_init = millis();           //on remplace par millis() qui fait la mm chose de maniere moins ghetto TwT
unsigned long n_incr = 0;
float croissance = 0;
unsigned long temps_ouverture = 10000;

/*const int color R = 255; // initialisation de la couleur rouge à 255
const int colorG = 0;   // initialisation de la couleur verte à 0
const int colorB = 0;   // initialisation de la couleur bleue à 0 */


void setup() {
  dht.begin();                                                 // démarrage de la mesure du capteur
  Serial.begin(9600);                                          // définition de la vitesse de transmission des données à 9600 bit/s

  lcd.begin(16, 2);                                            // initialisation d'un lcd à 16 colonnes et 2 lignes
  lcd.print(" hello, world ! ");                               // test du LCD
  pinMode(13, OUTPUT);                                         // pin 13 convertie en sortie électrique pour par exemple alimenter un ventilo
  pinMode(10, INPUT);                                         
  pinMode(9, INPUT);                                           //boutons

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // pour init fastled avec les données
  FastLED.setBrightness(BRIGHTNESS);                           // set master brightness control
}
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  time_since_init = millis();
  float h = dht.readHumidity();                                //définition d'un réel h qui a pour valeur l'humidité mesurée
  float t = dht.readTemperature();                             //définition d'un réel t qui a pour valeur la température mesurée
  
  //lcd.clear();
  if (time_since_init % 12 == 0){
    Serial.print("Humidity : ");
    Serial.println(h);                                           // affichage de l'humidité dans le moniteur série 
    Serial.print("Temperature: ");
    Serial.println(t);                                           // affichage de la température dans le moniteur série

    lcd.setCursor(0, 0);                                         //***********************************************************
    lcd.print("Temp = ");                                        //
    lcd.print(t);                                                //           Affiche les données temp et hum
    lcd.print(" *C ");                                           //                     sur la LCD
    lcd.setCursor(0, 1);                                         //
    lcd.print(" Hum = ");                                        //
    lcd.print(h);                                                //
    lcd.print(" % ");                                            //***********************************************************

    n_incr++;
    croissance = (croissance*n_incr+t)/(n_incr+1);
    Serial.print("k_croissance: ");                             //moyenne de la temperature depuis allumage
    Serial.println(croissance);
    if (time_since_init == temps_ouverture) allumage_servo();
    }

  // Call the current pattern function once, updating the 'leds' array  
  if (h > 37) fill_solid(leds, NUM_LEDS, CRGB::Red);
  else fill_rainbow(leds, NUM_LEDS, gHue, 7);
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 10 ) { gHue=gHue+3; } // slowly cycle the "base color" through the rainbow
 
  //time_since_init++;  //get time i guess ????
  if (h > 33) digitalWrite(13, HIGH);                                    // envoie le courant dans pin 13
  else digitalWrite(13, LOW);  

  val_tiroirs = digitalRead(9);
  val_dechets = digitalRead(10);
  if (val_tiroirs == HIGH) allumage_servo();
  if (val_dechets == HIGH) trappe_dechets();
  
}
// %25 = tt les 1s je crois

void allumage_servo() {
    //methode servo a implementer
}

void trappe_dechets() {
    //methode servo a implementer
}
