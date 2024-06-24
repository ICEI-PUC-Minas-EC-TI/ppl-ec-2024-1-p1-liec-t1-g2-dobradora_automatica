#include <ESP32Servo.h>
#include "BluetoothSerial.h"

// Configurações de Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define pinServo1 14
#define pinServo2 27
#define pinServo3 26
#define pinServo4 25

BluetoothSerial SerialBT;
int tipo_roupa; // var recebido do bluetooth indicando a roupa escolhida

Servo servo1, servo2, servo3, servo4;

// Ordem de operação dos servos para dobragem
int camisa[4] = {4, 3, 2, 1}, 
cam_longa[5] = {4, 3, 2, 3, 1}, 
bermuda[2] = {3, 1},
regata[4] = {4, 3, 2, 1};

// Função que movimenta o servo para mover a plataforma
void ServoOn(Servo &servo) {
  servo.write(180);
  delay(1300); // Aguardo de 1,3s para mover o servo
  servo.write(0);
}

void ServoOn(Servo &servo);
void dobrarRoupa(int servo);

// Função que define qual servo será movimentado
void dobrarRoupa(int servo) {
  switch (servo) {
    case 1:
      ServoOn(servo1);
      break;
    case 2:
      ServoOn(servo2);
      break;
    case 3:
      ServoOn(servo3);
      break;
    case 4:
      ServoOn(servo4);
      break;
  }
  delay(1300); // Delay de 1,3s para esperar a plataforma se mover
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Dobradora");
  Serial.println("O dispositivo ja pode ser pareado!");

  // Associa o motor no pino correspondente ao objeto servo
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo4.attach(pinServo4);

  // Definir o angulo de 0º para o giro do servo
  servo1.write(0); 
  servo2.write(0); 
  servo3.write(0); 
  servo4.write(0); 
  delay(2000);// Aguarda 2 segundos para inicializar os servos
}

void loop() {
  if (SerialBT.available() > 0) { // Se bluetooth estiver ativo
    Serial.println("Conectado!");

    tipo_roupa = (int)SerialBT.read(); // Lê valor do tipo de roupa
    
    switch (tipo_roupa) {
      case 1: // Se for camisa
        Serial.println("a");
        for(int i=0; i<4; i++) {
          dobrarRoupa(camisa[i]);
        } break;

      case 2: // Se for camisa de manga longa
        for(int i=0; i<5; i++) {
          dobrarRoupa(cam_longa[i]);
        } break;

      case 3: // Se for bermuda/shorts
        for(int i=0; i<2; i++) {
          dobrarRoupa(bermuda[i]);
        } break;

      case 4: // Se for regata
        for(int i=0; i<4; i++) {
          dobrarRoupa(regata[i]);
        } break;
    }
  }
}