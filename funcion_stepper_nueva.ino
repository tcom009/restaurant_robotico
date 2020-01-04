const int direccion = 12;
const int pinPasos = 4;
const int sensor = 13;



//direccion del motor
const int enviar = LOW;
const int regresar = HIGH;


//velocidad del motor
const int velMotor= 2;


//distancia por revolucion segun el actuador 
//pasos por milimetro (suponiendo que el actuador son 200 pasos 1mm )
const pasosPorMilimetro = 200;


//distancia en CM enteros
float distMesa1 = 5;
float distMesa2 = 10;
float distMesa3 = 15;


void setup(){
  
  pinMode (pinPasos, OUTPUT);
  pinMode (direccion, OUTPUT);
  
  Serial.begin (9600);
}


//convierte la distancia en CM a los pasos que debe dar el motor para alcanzar esa distancia
//retorna un entero con los pasos.
long convertirDistanciaPasos (int distancia) {
  //Equivalencias: 200 pasos = 1 mm 
  //pasar cm a mm y luego multiplicar por su equivalencia en pasos
  distancia = distancia *10;
  long pasos = distancia * 200;
  return pasos;
}


//Se encarga de ejecutar un ciclo para poder mover el motor del carrito a una distancia determinada
//y con una direccion esecifica (enviar o regresar carrito);
void moverMotorLong ( int pasos, int dir){
  
  digitalWrite (direccion,dir);
  for (int x = 0; x<=pasos; x ++){
  digitalWrite (pinPasos, HIGH);
  delay (velMotor);
  digitalWrite (pinPasos, LOW);
  delay (velMotor);

  }
  
}



//convertimos de cm a mm
int convertirDistanciaAMilimetros (float distanciaCm)
    distanciaMm = distanciaCm * 10;
    int (distanciaMm);
    return distanciaMm;



void moverMotorLoops (float distancia, int dir){
    int milimetros = convertirDistanciaAMilimetros(distancia)

    for (int x = 0; x<= milimetros; x++){
        digitalWrite (direccion, dir)
        for (int r = 0; r<= pasosPorMilimetro; r++){
            digitalWrite (pinPasos, HIGH);
            delay  (velMotor);
            digitalWrite(pinPasos, low);
            delay (velMotor);
        }

    Serial.print ("He recorrido ");
    Serial.print (x);
    Serial.println (" milimetros");
    }
Serial.println ("He terminado este recorrido");
}




void loop (){  
    
Serial.prinln ("Moviendo a la mesa 1");
moverMotorLoops (distMesa1, enviar);
Serial.println ("esperando para retornar");
delay (3000);
Serial.println ("retronando");
moverMotorLoops (mesa1, retornar);

}