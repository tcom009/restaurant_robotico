#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  




//pines Arduino
const int btnMesa1 = 7 ;
const int btnMesa2 = 8 ;
const int btnMesa3 = 10 ;  
const int btnCocina = 11 ;
const int direccion = 12;
const int pinPasos = 4;
const int sensor = 13;



//direccion del motor
const int enviar = LOW;
const int regresar = HIGH;


//velocidad del motor
const int velMotor= 2;




//distancia en CM enteros
int distMesa1 = 5;
int distMesa2 = 10;
int distMesa3 = 15;

//Estados de los botones
int estadoBtnMesa1 = 0;
int estadoBtnMesa2 = 0;
int estadoBtnMesa3 = 0;
int estadoBtnCocina = 0;


//turnos
int turno= 0;
int turnoMesa1 = 0;
int turnoMesa2 = 0;
int turnoMesa3 = 0;

// seteamos los pines del sensor ultra


void setup () {
  lcd.backlight();
  lcd.init();
  pinMode (btnMesa1, INPUT);
  pinMode (btnMesa2, INPUT);
  pinMode (btnMesa3, INPUT);
  pinMode (btnCocina, INPUT);
  pinMode (pinPasos, OUTPUT);
  pinMode (direccion, OUTPUT);
  pinMode (sensor, INPUT);
  Serial.begin (9600);
}

//Esta funcion imprime el numero de la mesa al que le toca atender y que turno tiene
void imprimirTurno (int numeroMesa, int turnoMesa) {
  lcd.setCursor (0,0);
  lcd.print ("Pedido Recibido ");
  lcd.setCursor(0,1);
  lcd.print(" Mesa:" );
  lcd.print (numeroMesa);
  lcd.print ("Turno:");
  lcd.print(turnoMesa);



  Serial.print ("LA MESA  ");
  Serial.print (numeroMesa);
  Serial.println(" ha hecho un pedido" );
  Serial.print ("Y tiene el turno numero: ");
  Serial.println(turnoMesa);

}


//ESTA FUNCION RECIBE EL ESTADO DEL BOTON Y EL NUMERO DE LA MESA AL QUE CORRESPONDE ESE ESTADO 
bool verificarEstadoBoton (int estadoBtn, int numeroMesa){ 

  if (estadoBtn == HIGH){
    delay (500); 
    turno++;


    switch(numeroMesa){
    case 1:
      turnoMesa1= turno;
      imprimirTurno (numeroMesa, turnoMesa1);
      return true;
      break;


    case 2:
      turnoMesa2= turno;
      imprimirTurno (numeroMesa, turnoMesa2);
      return true;
      break;

    case 3:
      turnoMesa3= turno;
      imprimirTurno (numeroMesa,turnoMesa3);
      return true;
      break;
    }

  }else {
    
    return false;
    }

}

//convierte la distancia en CM a los pasos que debe dar el motor para alcanzar esa distancia
//retorna un entero con los pasos.
int convertirDistanciaPasos (int distancia) {
  //Equivalencias: 200 pasos = 1 mm 
  //pasar cm a mm y luego multiplicar por su equivalencia en pasos
  distancia = distancia *10;
  int pasos = distancia * 200;
  return pasos;
}


//Se encarga de ejecutar un ciclo para poder mover el motor del carrito a una distancia determinada
//y con una direccion esecifica (enviar o regresar carrito);
void moverMotor ( int pasos, int dir){
  
  digitalWrite (direccion,dir);
  for (int x = 0; x<=pasos; x ++){
  digitalWrite (pinPasos, HIGH);
  delay (velMotor);
  digitalWrite (pinPasos, LOW);
  delay (velMotor);

  }
  
}
//Esta funcion recibe el numero de la mesa 
//envia el pedido y espera a que el cliente
//tome el pedido, la funcion sabe que el cliente ha tomado el pedido
//por medio de un sensor Ultrasonico, que al detectar
//un cambio en la distancia retorna la bandeja a la cocina automaticamente

void enviarPedido (int numeroMesa){ 
  int pasosMesa =0;
  int bandeja =0;

  
  //en esta parte segun el numero de la mesa 
  //le asignamos su distancia (declarada en las varibles al inicio del programa)
  //y las convertimos a pasos por medio de la funcion convertirDistanciaPasos()
  switch (numeroMesa){
  case 1:
  pasosMesa =  convertirDistanciaPasos (distMesa1);
  break;
  case 2:
  pasosMesa =  convertirDistanciaPasos (distMesa2);
  break;
  case 3:
  pasosMesa =  convertirDistanciaPasos (distMesa3);
  break;
  }
  //Serial.println("Enviando pedido");
   lcd.clear ();
   lcd.setCursor(0,0);
   lcd.print("Enviando pedido...");
  moverMotor (pasosMesa, enviar);
  lcd.clear ();
  lcd.setCursor(0,0);
  lcd.print("El carrito ha llegado");
  //aqui inicia el sensor a detectar si el cliente ha tomado o no la bandeja 
  lcd.setCursor(0,1);
  lcd.print ("Esperando clte");
  
  do
  {
    bandeja = digitalRead (sensor);
  }
  while (bandeja == 0);
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("El cliente ha to");
  lcd.setCursor (0,1);
  lcd.print("mado su pedido");
  delay (5000);
  lcd.clear();
  lcd.print ("Retornando carrito");
  Serial.println ("Retornando carrito");
  moverMotor (pasosMesa, regresar);
  Serial.println ("El carrito ha llegado a la cocina");
  Serial.print ("Hoy ha atendido  ");
  Serial.print (turno);
  Serial.println (" pedidos.");
  Serial.println ("Listo para recibir mas pedidos");
  lcd.clear ();
  lcd.setCursor (0,0);
  lcd.print ("Hoy ha atendido");
  lcd.setCursor(0,1);
  
  lcd.print ("Total pdidos ");
  lcd.print (turno);

}


//esta funcion verifica si hay o no pedido y recibe el numero de la mesa que lo hizo
//luego de verificar espera a que la cocina presione el boton para
//enviar el pedido a la mesa correspondiente
//el boton de enviar puede ser accionado unicamente si hay un pedido en la bandeja
void aceptarPedido (bool pedidoMesa,int mesa){
  int bandeja = 0;
  bool mantenerCarrito = true;
  if (pedidoMesa==true){ // si hay pedido
    Serial.println ("Coloque el pedido en el carrito");
    
    do {
      bandeja = digitalRead (sensor);
      if (bandeja==0){
        //Serial.println (verificarBandejaColocada());
        estadoBtnCocina = digitalRead (btnCocina);
        
        if (estadoBtnCocina == HIGH) {
        delay (1000);
        enviarPedido (mesa);
        mantenerCarrito = false;
        }
        
      }
    }
    while (mantenerCarrito == true);
  }
}

void loop  () {
  //verificamos si el estado ha cambiado
  //aceptamos el pedido si pedidoMesaN cumple con la condicion y le enviamos el numero de la mesa
  lcd.setCursor(0,0);
  lcd.print("Esperando pedido");
  estadoBtnMesa1 = digitalRead (btnMesa1);
  bool pedidoMesa1 = verificarEstadoBoton (estadoBtnMesa1, 1);
  aceptarPedido (pedidoMesa1,1);
  

  estadoBtnMesa2 = digitalRead (btnMesa2);
  bool pedidoMesa2 = verificarEstadoBoton (estadoBtnMesa2, 3);
  aceptarPedido (pedidoMesa2,3);


  estadoBtnMesa3 = digitalRead (btnMesa3);
  bool pedidoMesa3 = verificarEstadoBoton (estadoBtnMesa3, 2);
  aceptarPedido (pedidoMesa3,2);

}
