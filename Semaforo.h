#include <Arduino.h>

// Se declara variables de tipo enumerado para los estados del semaforo
typedef enum
{
    normal,
    desconectado,
    alarma,
} estado_semaforo;
// La funcion devuelve el valor del estado inicial del semaforo
estado_semaforo semaforo_inicio(void)
{
    estado_semaforo estado_semaforo_t = normal;
    return estado_semaforo_t;
}
// Esta funcion hace todo el proceso de cambio de estados
int semaforo_cambio(int16_t SW1, int16_t SW2, estado_semaforo *estado_semaforo_t, int16_t LED_Amarillo, int16_t LED_Rojo, int16_t LED_Verde)
{
    //Se declaran banderas para que se entre al condicional if ya sea una o mas veces, con el cambio del static
    static boolean bandera_LED_Verde = true;
    static boolean bandera_LED_Amariillo = false;
    static boolean bandera_LED_Rojo = false;
    static boolean bandera_desconectado = false;
    static boolean bandera_LED_Amariillo_p = true;
    static boolean bandera_LED_Rojo_p = true;
    //Se declaran las varibles para los retardos no bloqueantes
    static unsigned long tiempo = 0;
    static unsigned long tiempo1 = 0;
    static unsigned long tiempo2 = 0;
    static unsigned long tiempo3 = 0;
    static unsigned long tiempo4 = 0;
    static unsigned long tiempo5 = 0;
    static unsigned long tiempo6 = 0;
    static unsigned long tiempo_SW2 = 0;
    //Es el tiempo que se demora para adquirir de nuevo otro valor, intermoniendose para que no se pulse mas de una vez seguidamente
    int16_t tiempo_anti_rebote = 300;
    //Rapidez del encendido y apagado de leds
    static int16_t Pulso_1 = 3000;
    static int16_t Pulso_2 = 500;
    static int16_t Pulso_3 = 2000;
    static int16_t cont = 0;
    int retorno;
    //Se adquiere el puntero de la variable para cambiar entre estados
    switch (*estado_semaforo_t)
    {
    case normal:
        //Entra a este if una sola vez para inicializar los leds si estan prendidos o apagados
        if (bandera_LED_Verde == true && bandera_LED_Rojo == false && bandera_LED_Amariillo == false && bandera_desconectado == false)
        {
            digitalWrite(LED_Verde, HIGH);
            digitalWrite(LED_Rojo, LOW);
            digitalWrite(LED_Amarillo, LOW);
            bandera_LED_Amariillo = true;
            bandera_desconectado = true;
            tiempo2 = millis();
        }
        //Entra a este if despues de que se haya cumplido con el tiempo del Pulso_1 que controla la velocidad de encendido
        if (millis() - tiempo2 > Pulso_1 && bandera_LED_Verde == true && bandera_LED_Rojo == false && bandera_LED_Amariillo == true)
        {
            digitalWrite(LED_Verde, HIGH);
            digitalWrite(LED_Amarillo, HIGH);
            digitalWrite(LED_Rojo, LOW);
            bandera_LED_Amariillo = false;
            bandera_LED_Verde = false;
            bandera_LED_Rojo = true;
            tiempo3 = millis();
        }
        //Entra a este if despues de que se haya cumplido con el tiempo del Pulso_2 que controla la velocidad de encendido
        if (millis() - tiempo3 > Pulso_2 && bandera_LED_Verde == false && bandera_LED_Rojo == true && bandera_LED_Amariillo == false)
        {
            digitalWrite(LED_Rojo, HIGH);
            digitalWrite(LED_Amarillo, LOW);
            digitalWrite(LED_Verde, LOW);
            bandera_LED_Rojo = false;
            bandera_LED_Amariillo = false;
            bandera_LED_Verde = true;
            bandera_desconectado = true;
            tiempo4 = millis();
        }
        //Entra a este if despues de que se haya cumplido con el tiempo del Pulso_3 que controla la velocidad de encendido
        if (millis() - tiempo4 > Pulso_3 && bandera_LED_Verde == true && bandera_LED_Rojo == false && bandera_LED_Amariillo == false && bandera_desconectado == true)
        {
            digitalWrite(LED_Verde, HIGH);
            digitalWrite(LED_Rojo, LOW);
            digitalWrite(LED_Amarillo, LOW);
            bandera_LED_Amariillo = true;
            tiempo2 = millis();
        }
        //Entra a este if para verificar si se ha pulsado o no el boton 1, siempre con el retarno antibloqueo
        if (millis() - tiempo > tiempo_anti_rebote)
        {
            if (!digitalRead(SW1)) //Pasa al siguiente estado del semaforo que es el desconectado
            {
                *estado_semaforo_t = desconectado;
                //const char *str = "Se cambia al estado desconectado";
                //printf("%s\n", str);
                retorno = 1;
                bandera_LED_Verde = true;
                bandera_LED_Amariillo = false;
                bandera_LED_Rojo = false;
                bandera_desconectado = false;
            }
            tiempo = millis();
        }
        //Entra a este if si se ha pulsado o no el boton 2 para cambiar la velocidad de encendido y apagado de los leds
        if (millis() - tiempo_SW2 > tiempo_anti_rebote)
        {
            if (!digitalRead(SW2) && cont == 0)
            {
                //const char *str = "Se cambia rapido";
                //printf("%s\n", str);
                Pulso_1 = 3000 * 0.5;
                Pulso_2 = 500 * 0.5;
                Pulso_3 = 2000 * 0.5;
                cont++;
            }
            else if (!digitalRead(SW2) && cont == 1)
            {
                //const char *str = "Se cambia normal";
                //printf("%s\n", str);
                Pulso_1 = 3000 * 1;
                Pulso_2 = 500 * 1;
                Pulso_3 = 2000 * 1;
                cont++;
            }
            else if (!digitalRead(SW2) && cont == 2)
            {
                //const char *str = "Se cambia lento";
                //printf("%s\n", str);
                Pulso_1 = 3000 * 2;
                Pulso_2 = 500 * 2;
                Pulso_3 = 2000 * 2;
                cont = 0;
            }
            tiempo_SW2 = millis();
        }
        break;
    case desconectado:
        digitalWrite(LED_Rojo, LOW);
        digitalWrite(LED_Verde, LOW);
        //Igual que en el estado normal se utiliza retardos no bloqueantes para el encendido y apagado de los leds
        if (millis() - tiempo5 > 500 && bandera_LED_Amariillo_p == true)
        {
            digitalWrite(LED_Amarillo, HIGH);
            bandera_LED_Amariillo_p = false;
            tiempo5 = millis();
        }

        if (millis() - tiempo5 > 500 && bandera_LED_Amariillo_p == false)
        {
            digitalWrite(LED_Amarillo, LOW);
            bandera_LED_Amariillo_p = true;
            tiempo5 = millis();
        }
        //Se verifica si se ha pulsado o no el boton 1
        if (millis() - tiempo > tiempo_anti_rebote)
        {
            if (!digitalRead(SW1))
            {
                //const char *str = "Se cambia al estado alarma";
                //printf("%s\n", str);
                retorno = 2;
                *estado_semaforo_t = alarma;
            }
            tiempo = millis();
        }
        break;

    case alarma:
        digitalWrite(LED_Amarillo, LOW);
        digitalWrite(LED_Verde, LOW);
        //Mismo caso que el estado desconectado
        if (millis() - tiempo6 > 1000 && bandera_LED_Rojo_p == true)
        {
            digitalWrite(LED_Rojo, HIGH);
            bandera_LED_Rojo_p = false;
            tiempo6 = millis();
        }

        if (millis() - tiempo6 > 1000 && bandera_LED_Rojo_p == false)
        {
            digitalWrite(LED_Rojo, LOW);
            bandera_LED_Rojo_p = true;
            tiempo6 = millis();
        }
        //Se verifica si el boton 1 se ha pulsado para cambiar al estado normal
        if (millis() - tiempo > tiempo_anti_rebote)
        {
            if (!digitalRead(SW1))
            {
                //const char *str = "Volviendo al estado normal";
                //printf("%s\n", str);
                retorno = 3;
                *estado_semaforo_t = normal;
            }
            tiempo = millis();
        }
        break;
    default:
        break;
    }
    return retorno;
}
