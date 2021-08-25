/*
  Controlador da tampa da lixeira
  Autor: Henrique Vilela
  Data: 23/08/2021
  Versão: 1.0
*/
#include <Arduino.h>
#include <Servo.h>
#include <avr/sleep.h>

//definições de parâmetros

// PINOS
#define BUTTON 2
#define SERVO 12

// LIMITES
#define POS_MAX 105
#define POS_MIN 10

// MOVIMENTO
#define INCREMENT 2
#define VEL 30

// ESTADOS
#define FECHADO false
#define ABERTO true

Servo myservo;     // cria objeto para controlar o servo
int pos = POS_MIN; // variável pra armazenar posição do servo já com a posição inicial central
bool estado = FECHADO;

// protótipos das funções
void sobe();
void desce();
void sleepMode();

//------------------------------------------------------
void setup()
{
  myservo.attach(SERVO);         // define o pino do servo
  pinMode(BUTTON, INPUT_PULLUP); // define o pino do botão como entrada
  myservo.write(pos);            // escreve a posição inicial no servo
}
//------------------------------------------------------
void loop()
{
  sleepMode();
  while (!digitalRead(BUTTON))
  {
    if (estado == FECHADO)
      sobe();
    else // estado == ABERTO
      desce();
  }
}

// FUNÇÕES ----------------------------------------------

void sobe()
{
  while (pos < POS_MAX)
  {
    pos += INCREMENT;
    delay(VEL);
    myservo.write(pos);
  }
  estado = ABERTO;
}
//-------------------------------------------------------
void desce()
{
  while (pos > POS_MIN)
  {
    pos -= INCREMENT;
    delay(VEL);
    myservo.write(pos);
  }
  estado = FECHADO;
}

//-------------------------------------------------------
void sleepMode()
{
  attachInterrupt(
      digitalPinToInterrupt(BUTTON),
      []()
      {
        //função lâmbda para acordar o Arduino
        sleep_disable();
        detachInterrupt(digitalPinToInterrupt(BUTTON));
      },
      LOW);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  delay(1000);
  sleep_cpu();
}