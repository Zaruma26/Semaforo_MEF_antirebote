#include "teclas.h"
#include "Semaforo.h"
#include "uart.h"
//Se declaran los Pines
const int16_t SW1 = 15;
const int16_t SW2 = 2;
const int16_t LED_Verde = 23;
const int16_t LED_Amarillo = 22;
const int16_t LED_Rojo = 21;
//Teniendo la estructura de "teclas.h" se declara el constructor
fsmTecla Tecla;
//Se declara el constructor de las demas estructuras, inicializandolas
dbn_t estructura = {0, 500, Sin_cambio1};
dbn_t estructura2 = {0, 500, Sin_cambio1};
//Se declara el constructor del semaforo de "Semaforo.h"
estado_semaforo estado_semaforo_t;

// put function declarations here:

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);//Se inicaliza el serial para que se vea lo que se pone
  Tecla = fsmInicializar(); //Se inicializa la fsm de las teclas antirebote
  estado_semaforo_t = semaforo_inicio();//Se inicializa el estado del semaforo
  //Se declaran los pines como entradas o salidas
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED_Verde, OUTPUT);
  pinMode(LED_Amarillo, OUTPUT);
  pinMode(LED_Rojo, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  //Se llama a las funciones, cada una con su respectivo retardo anti bloqueo
  boolean cambio1 =fsmAnti_rebote(SW1, &Tecla);
  boolean cambio2 = fsmAnti_rebote2(SW2, &Tecla);
  actualizarMEF(&estructura, &estructura2, SW1, SW2);
  int cambio = semaforo_cambio(SW1, SW2, &estado_semaforo_t, LED_Amarillo, LED_Rojo, LED_Verde);
  imprimir_pantalla(SW1, cambio);
  imprimir_pantalla2(SW2, SW1, cambio1, cambio2);
}

// put function definitions here: