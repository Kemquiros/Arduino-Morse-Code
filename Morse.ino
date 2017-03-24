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

int cantidadLetras = 36;
String letra[36];//Letras del abecedario
String codigo[36];//etras en morse

bool leer;
void setup() {
  leer = false;
  ditTime = 100;
  dashTime = ditTime*4;
  cadena="";
  mensaje="";
  pinMode(2, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  pinMode(11,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), leerMorse, CHANGE);

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), activarMorse, CHANGE);
  Serial.begin(9600);
  state = false;
  changes = 0;

  Timer1.initialize(5000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(timerIsr ); // attach the service routine here

  iniciarDiccionario();
  enviar("LUI");
  //Serial.println(getCodigo("L"));
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH); 
  //Serial.println(reconocer("ABABCBA"));
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
  /*
  if(cad=="CBC"){
    temp= "M";
  }else if(cad=="CBCBC"){
    temp= "O";
  }
  */
  for(int i=0;i<cantidadLetras;i++){
    if(cad==codigo[i]){
      temp = letra[i];
      break;
    }
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

void iniciarDiccionario(){
  letra[0]="A";   codigo[0]="ABC";
  letra[1]="B";   codigo[1]="CBABABA";
  letra[2]="C";   codigo[2]="CBABCBA";
  letra[3]="D";   codigo[3]="CBABA";
  letra[4]="E";   codigo[4]="A";   
  letra[5]="F";   codigo[5]="ABABCBA";
  letra[6]="G";   codigo[6]="CBCBA";
  letra[7]="H";   codigo[7]="ABABABA";
  letra[8]="I";   codigo[8]="ABA";
  letra[9]="J";   codigo[9]="ABCBCBC";
  letra[10]="K";  codigo[10]="CBABC";
  letra[11]="L";  codigo[11]="ABCBABA";
  letra[12]="M";  codigo[12]="CBC";
  letra[13]="N";  codigo[13]="CBA";
  letra[14]="O";  codigo[14]="CBCBC";
  letra[15]="P";  codigo[15]="ABCBCBA";
  letra[16]="Q";  codigo[16]="CBCBABC";
  letra[17]="R";  codigo[17]="ABCBA";
  letra[18]="S";  codigo[18]="ABABA";
  letra[19]="T";  codigo[19]="C";
  letra[20]="U";  codigo[20]="ABABC";
  letra[21]="V";  codigo[21]="ABABABC";
  letra[22]="W";  codigo[22]="ABCBC";
  letra[23]="X";  codigo[23]="CBABABC";
  letra[24]="Y";  codigo[24]="CBABCBC";
  letra[25]="Z";  codigo[25]="CBCBABA";

  letra[26]="1";  codigo[26]="ABCBCBCBC";
  letra[27]="2";  codigo[27]="ABABCBCBC";
  letra[28]="3";  codigo[28]="ABABABCBC";
  letra[29]="4";  codigo[29]="ABABABABC";
  letra[30]="5";  codigo[30]="ABABABABA";
  letra[31]="6";  codigo[31]="CBABABABA";
  letra[32]="7";  codigo[32]="CBCBABABA";
  letra[33]="8";  codigo[33]="CBCBCBABA";
  letra[34]="9";  codigo[34]="CBCBCBCBA";
  letra[35]="0";  codigo[35]="CBCBCBCBC";
}

void enviar(String msg){
  //Serial.println(sizeof(msg));
  for(int i=0;i<msg.length();i++){
    char letraTemp = msg.charAt(i);
    String codigoL = getCodigo(letraTemp);
    for(int j=0;j<codigoL.length();j++){
      char simbolo = codigoL.charAt(j);
      if(simbolo=='A'){
        generateDit();
      }else if(simbolo=='B'){
        generateSymbolSpace();
      }else if(simbolo=='C'){
        generateDash();
      }
    }
  }
}

String getCodigo(char letraTemp){
  String codigoTemp = "";
  for(int i=0;i<sizeof(letra);i++){
    if(letra[i]==String(letraTemp)){
      //Serial.println(i);
      codigoTemp = codigo[i];
      break;
    }
  }
  return codigoTemp;
}

void generateDit(){
  digitalWrite(11, HIGH);
  delay(ditTime);
  digitalWrite(11, LOW);
}
void generateDash(){
  digitalWrite(11, HIGH);
  delay(dashTime);
  digitalWrite(11, LOW);
}
void generateSymbolSpace(){
  delay(ditTime);
}


