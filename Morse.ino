#include <TimerOne.h>
bool state; //Define si el led esta prendido o apagado
int changes;
int ditTime; //milisegundos
int dashTime;
int symbolSpaceTime;
int letterSpaceTime;
int wordSpaceTime;
String cadena;
String mensaje;

bool leer;
void setup() {
  leer = false;
  ditTime = 100;
  dashTime = ditTime*4;
  cadena="";
  mensaje="";
  pinMode(2, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), leerMorse, CHANGE);

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), activarMorse, CHANGE);
  Serial.begin(9600);
  state = false;
  changes = 0;

  Timer1.initialize(5000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH); 
}

void leerMorse() {
  
  if(leer){
    state = !state;
    if (changes == 0) {
      Timer1.start();
    } else {
      float time = Timer1.read()/1000;
      //Serial.println(time);
      Timer1.restart();
      if(!state){
          if(time>ditTime-10 && time<ditTime+10){
            //dit
            Serial.println("A");
            cadena=cadena+"A";
          }else if(time>dashTime-10 && time<dashTime+10){
            //dash
            Serial.println("C");
            cadena=cadena+"C";
          }   
      }else{
        if(time>ditTime-10 && time<ditTime+10){
            //dit, pero con el led apagado
            Serial.println("B");
            cadena=cadena+"B";
          }else if(time>dashTime-10 && time<dashTime+10){
            //dash con el led apagado
            Serial.println("D");
            
            mensaje =mensaje+ reconocer(cadena);
            //mensaje= mensaje+letra;
            cadena="";
          }
      }
    }
    changes++;
    //Serial.println(changes);

  }
}
String reconocer(String cad){
  String temp="";
  if(cad=="CBC"){
    temp= "M";
  }else if(cad=="CBCBC"){
    temp= "O";
  }
  return temp;
}
void activarMorse(){
  if(!leer){
    mensaje="";
  }else{
    mensaje=mensaje+reconocer(cadena);
    cadena="";
    Serial.println(mensaje);
  }
  leer = !leer;
  Serial.println(leer);
}
void timerIsr() {

}

