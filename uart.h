#include <Arduino.h>
#include <stdio.h>
//La funcion recibe como argumentos el pulsante y si este afecta a la maquina de estados del semaforo
void imprimir_pantalla(int16_t SW1, int cambio)
{
    static unsigned long tiempo = 0;
    int16_t tiempo_anti_rebote = 300;
    if (millis() - tiempo > tiempo_anti_rebote)
    {
        if (!digitalRead(SW1) && cambio == 1)
        {
            const char *str = "Se cambia al estado desconectado";
            printf("%s\n", str);
        }else if(!digitalRead(SW1) && cambio == 2)
        {
            const char *str = "Se cambia al estado alarma";
            printf("%s\n", str);
        }else if (!digitalRead(SW1) && cambio == 3)
        {
            const char *str = "Volviendo al estado normal";
            printf("%s\n", str);
        }
        tiempo = millis();
    }
}

//Se recibe como argumento los switches y variables boleanas, ya que el antirebote trata de liberar y pulsar
void imprimir_pantalla2(int16_t SW1, int16_t SW2, boolean cambio1, boolean cambio2){
    //Se declaran variables globales asi como estaticas debido a que se entra bastantes veces a la funcion
    static unsigned long tiempo = 0;
    int16_t tiempo_anti_rebote = 300;
    static int cont = 0; 
    static int cont2 = 0;
    //Bucle if para el retarno no bloqueante
    if (millis() - tiempo > tiempo_anti_rebote)
    {
        //Detecta si se pulsa o no el switch correspondiente ademas de la variable cambio 1 y 2 para saber si se pulso y libero
        if (!digitalRead(SW1) && digitalRead(SW2) && cambio1 == true)
        {
            const char *str = "Se pulsa 1";
            printf("%s\n", str);
            cont ++;
        }else if(digitalRead(SW1) && digitalRead(SW1) && cambio1 == false && cont == 1)
        {
            const char *str = "Se libera 1";
            printf("%s\n", str);
            cont = 0;
        }
        else if (digitalRead(SW1) && !digitalRead(SW2) && cambio2 == true)
        {
            const char *str = "Se pulsa 2";
            printf("%s\n", str);
            cont2 ++;
        }
        else if (digitalRead(SW1) && digitalRead(SW2) && cambio2 == false && cont2 == 1)
        {
            const char *str = "Se libera 2";
            printf("%s\n", str);
            cont2 = 0;
        }
        tiempo = millis();
    }
}