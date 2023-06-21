#include <Arduino.h>
#include <stdio.h>
// Se define la estructura de variables enumeradas
typedef enum
{
    Pulsado,
    Liberado,
} fsmTecla;

typedef enum
{
    Cambia1,
    Sin_cambio1,
    Cambia2,
    Sin_cambio2,
} estadoMEF;

typedef struct
{
    int16_t tecla;    // Nombre de la tecla
    int16_t retardo;  // Retarno no bloqueante
    estadoMEF estado; // Estados
} dbn_t;

// Función que define cuando se pulsa una tecla
// Recibe como argumentos el boton y la tecla asociada
int buttonPressed(int16_t tecla, fsmTecla Tecla) // Envia un entero para saber si es pulsado o liberado
{
    if (!digitalRead(tecla))
    {
        //const char *str = "Se pulsa el la tecla 1"; // Se guarda en un puntero str el string
        //printf("%s\n", str);
        Tecla = Pulsado;
    }
    return Tecla;
}

int buttonPressed1(int16_t tecla, fsmTecla Tecla)
{
    if (!digitalRead(tecla))
    {
        //const char *str = "Se pulsa el la tecla 2"; // Se guarda en un puntero str el string
        //printf("%s\n", str);
        Tecla = Pulsado;
    }
    return Tecla;
}
// Funcion que define cuando se libera un boton
// Recibe como argumento el boton y la tecla asociada
int buttonReleassed(int16_t tecla, fsmTecla Tecla) // Envia un entero para saber si es pulsado o liberado
{
    if (digitalRead(tecla))
    {
        //const char *str = "Se libera la tecla 1";
        //printf("%s\n", str);
        Tecla = Liberado;
    }
    return Tecla;
}

int buttonReleassed1(int16_t tecla, fsmTecla Tecla)
{
    if (digitalRead(tecla))
    {
        //const char *str = "Se libera la tecla 2";
        //printf("%s\n", str);
        Tecla = Liberado;
    }
    return Tecla;
}

// Funcion que inicializa la máquina de estados
fsmTecla fsmInicializar(void)
{ // Se envia como parametro la variable enumerada
    fsmTecla Tecla = Liberado;
    return Liberado;
}
// Funcion que permite el antirebote de la tecla
boolean fsmAnti_rebote(int16_t SW, fsmTecla *Tecla)
{
    // Se declaran las variables para el funcionamiento del retardo no bloqueante
    int presiona;
    unsigned long tiempo_millis = millis();
    static int tiempo = 0; // Se define como estatico ya que entra a la funcion muchas veces
    unsigned long tiempo_anti_rebote = 200;
    boolean retorno;
    // Para la maquina de estados se declara dos casos el que se pulsa y se deja de pulsar
    switch (*Tecla)
    {
    // El caso pulsado usa el retardo no bloqueante y la funcion cuando se presiona el boton
    case Pulsado:
        if (tiempo_millis - tiempo > tiempo_anti_rebote)
        {
            presiona = buttonReleassed(SW, *Tecla); // Funcion cuando se presiona el boton
            if (presiona == 1)
            {
                //const char *str = "Se pulsa el la tecla 1"; // Se guarda en un puntero str el string
                //printf("%s\n", str);
                *Tecla = Liberado; // Cambia de estado al liberado cuando se presiona
                retorno = true;
            }
            tiempo = millis(); // Se actualiza el tiempo de la funcion millis()
        }
        break;
    // El caso liberado usa el retardo no bloqueante y la funcion cuando se libera el boton
    case Liberado:
        if (tiempo_millis - tiempo > tiempo_anti_rebote)
        {
            int presiona = buttonPressed1(SW, *Tecla); // Funcion que libera el boton
            if (presiona == 0)
            {
                const char *str = "Se libera la tecla 1"; // Se guarda en un puntero str el string
                printf("%s\n", str);
                *Tecla = Pulsado; // Cambia de estado al pulsado cuando se libera el boton
                retorno = false;
            }
            tiempo = millis();
        }
        break;
    default:
        break;
    }
    return retorno;
}
// Se repite la funcion fsmAnti_rebote para el boton 2
boolean fsmAnti_rebote2(int16_t SW2, fsmTecla *Tecla)
{
    int presiona;
    unsigned long tiempo_millis = millis();
    static int tiempo = 0;
    unsigned long tiempo_anti_rebote = 200;
    boolean retorno;
    switch (*Tecla)
    {
    case Pulsado:
        if (tiempo_millis - tiempo > tiempo_anti_rebote)
        {
            presiona = buttonReleassed1(SW2, *Tecla);
            if (presiona == 1)
            {
                *Tecla = Liberado;
                retorno = true;
            }
            tiempo = millis();
        }

        break;
    case Liberado:
        if (tiempo_millis - tiempo > tiempo_anti_rebote)
        {
            int presiona = buttonPressed(SW2, *Tecla);
            if (presiona == 0)
            {
                *Tecla = Pulsado;
                retorno = false;
            }
            tiempo = millis();
        }
        break;
    default:
        break;
    }
    return retorno;
}

// Funcion punto 2
// Recibe como argumentos la estructura de la dbn_t en los datos como punteros como punteros y los pulsantes
void actualizarMEF(dbn_t *dataTecla1, dbn_t *dataTecla2, int16_t pulsante1, int16_t pulsante2)
{
    //A cada puntero se le asigna el pulsante, retardo
    dataTecla1->tecla = pulsante1;
    dataTecla2->tecla = pulsante2;

    dataTecla1->retardo = 200;
    dataTecla2->retardo = 500;
    //Se declaran varibles locales y estáticas porque se entra muchas veces a la funcion
    unsigned long tiempo_millis = millis();
    static int tiempo = 0;
    static int bandera1 = 0;
    static int bandera2 = 0;
    //El switch permite crear los estados 
    switch (dataTecla1->estado)
    {
    case Sin_cambio1:
        // Serial.println("Estado Pulsado");
        if (tiempo_millis - tiempo > dataTecla1->retardo)
        {
            // Condicionales si cambia el estado cuando la tecla 1 se pulso
            if (!digitalRead(dataTecla1->tecla) && digitalRead(dataTecla2->tecla) && bandera1 == 0)
            {
                const char *str = "Se pulsa el la tecla 1 1"; // Se guarda en un puntero str el string
                printf("%s\n", str);
                if (tiempo_millis - tiempo > dataTecla1->retardo)
                {
                    dataTecla1->estado = Cambia1;
                    bandera1 = 1;
                }
            }
            // Condicionales si cambia el estado cuando la tecla 2 se pulso y Tambien se pulso tecla 1
            if (digitalRead(dataTecla1->tecla) && !digitalRead(dataTecla2->tecla) && (bandera2 == 0 || bandera2 == 1))
            // if (pulsante1 == 1)
            {
                if (tiempo_millis - tiempo > dataTecla1->retardo)
                {
                    if (bandera2 == 0)
                    {
                        const char *str = "Se pulsa el la tecla 2 1"; // Se guarda en un puntero str el string
                        printf("%s\n", str);
                        dataTecla1->estado = Cambia2;
                        bandera2 = 1;
                    }
                    else if (bandera2 == 1)
                    {
                        const char *str = "Nuevamente 2 1"; // Se guarda en un puntero str el string
                        printf("%s\n", str);
                        dataTecla1->estado = Sin_cambio2;
                        bandera2 = 0;
                    }
                }
            }
            tiempo = millis();
        }
        break;
    case Cambia1:
        if (tiempo_millis - tiempo > dataTecla1->retardo)
        {
            // Condicionales si cambia el estado cuando la tecla 1 se pulso
            if (!digitalRead(dataTecla1->tecla) && digitalRead(dataTecla2->tecla) && bandera1 == 1)
            {
                const char *str = "Nuevamente 1 2"; // Se guarda en un puntero str el string
                printf("%s\n", str);
                if (tiempo_millis - tiempo > dataTecla1->retardo)
                {
                    bandera1 = 0;
                    dataTecla1->estado = Sin_cambio1;
                }
            }
            // Condicionales si cambia el estado cuando la tecla 1 se pulso y se pulsa la tecla 2
            if (digitalRead(dataTecla1->tecla) && !digitalRead(dataTecla2->tecla) && (bandera2 == 0 || bandera2 == 1))
            {
                if (tiempo_millis - tiempo > dataTecla1->retardo)
                {
                    if (bandera2 == 0)
                    {
                        dataTecla1->estado = Cambia2;
                        const char *str = "Se pulsa el la tecla 2 2"; // Se guarda en un puntero str el string
                        printf("%s\n", str);
                        bandera2 = 1;
                    }
                    else if (bandera2 == 1)
                    {
                        dataTecla1->estado = Sin_cambio2;
                        const char *str = "Nuevamente 2 2"; // Se guarda en un puntero str el string
                        printf("%s\n", str);
                        bandera2 = 0;
                    }
                }
            }
            tiempo = millis();
        }
        break;
    case Sin_cambio2:
        if (tiempo_millis - tiempo > dataTecla1->retardo)
        {
            // Condicionales si cambia el estado cuando la tecla 2 se pulso y se pulsa la tecla 1
            if (!digitalRead(dataTecla1->tecla) && digitalRead(dataTecla2->tecla) && (bandera1 == 0 || bandera1 == 1))
            {
                if (bandera1 == 0)
                {
                    const char *str = "Se pulsa el la tecla 1 3"; // Se guarda en un puntero str el string
                    printf("%s\n", str);
                    dataTecla1->estado = Cambia1;
                    bandera1 = 1;
                }
                else if (bandera1 == 1)
                {
                    const char *str = "Nuevamente 1 3"; // Se guarda en un puntero str el string
                    printf("%s\n", str);
                    dataTecla1->estado = Sin_cambio1;
                    bandera1 = 0;
                }
            }
            // Condicionales si cambia el estado cuando la tecla 2 se pulso
            if (digitalRead(dataTecla1->tecla) && !digitalRead(dataTecla2->tecla) && bandera2 == 0)
            {
                const char *str = "Se pulsa el la tecla 2 3"; // Se guarda en un puntero str el string
                printf("%s\n", str);
                if (tiempo_millis - tiempo > dataTecla1->retardo)
                {
                    dataTecla1->estado = Cambia2;
                    bandera2 = 1;
                }
            }
            tiempo = millis();
        }
        break;
    case Cambia2:
        // Condicionales si cambia el estado cuando la tecla 2 se pulso y no la uno
        if (tiempo_millis - tiempo > dataTecla1->retardo)
        {
            if (!digitalRead(dataTecla1->tecla) && digitalRead(dataTecla2->tecla) && (bandera1 == 0 || bandera1 == 1))
            {
                if (bandera1 == 0)
                {
                    dataTecla1->estado = Cambia1;
                    const char *str = "Se pulsa el la tecla 1 4"; // Se guarda en un puntero str el string
                    printf("%s\n", str);
                    bandera1 = 1;
                }
                else if (bandera1 == 1)
                {
                    const char *str = "Nuevamente 1 4"; // Se guarda en un puntero str el string
                    printf("%s\n", str);
                    dataTecla1->estado = Sin_cambio1;
                    bandera1 = 0;
                }
            }
            // Condicionales si cambia el estado cuando la tecla 2 se pulso y se pulsa la tecla 2 nuevamente
            if (digitalRead(dataTecla1->tecla) && !digitalRead(dataTecla2->tecla) && bandera2 == 1)
            {
                const char *str = "Nuevamente 2 4"; // Se guarda en un puntero str el string
                printf("%s\n", str);
                if (tiempo_millis - tiempo > dataTecla1->retardo)
                {
                    bandera2 = 0;
                    dataTecla1->estado = Sin_cambio2;
                }
            }
            tiempo = millis();
        }
        break;
    default:
        break;
    }
}