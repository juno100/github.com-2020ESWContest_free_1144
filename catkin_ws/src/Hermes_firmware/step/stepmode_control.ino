#include <SoftwareSerial.h>

//
// L298N motor driver
//
#define ENA 5 
#define ENB 6
#define ENC 7 
#define END 8

#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25
#define IN5 26
#define IN6 27
#define IN7 28
#define IN8 29


// drive the car with the given pwm values.
void drive(int pwmA, int pwmB, int pwmC, int pwmD);

//
// encoders
//
#define ENCODER_A1 2
#define ENCODER_A2 A0

#define ENCODER_B1 3
#define ENCODER_B2 A1

#define ENCODER_C1 21
#define ENCODER_C2 A2

#define ENCODER_D1 20
#define ENCODER_D2 A3

#define RESOLUTION 256.0

volatile int encoderA = 0;
volatile int encoderB = 0;
volatile int encoderC = 0;
volatile int encoderD = 0;

//
// wheel specification
//
#define DIAMETER      20.0
#define CIRCUMFERENCE (DIAMETER * PI)

byte encoderALast;
bool encoderADir = true;
int encoderAcount = 0;

byte encoderBLast;
bool encoderBDir = true;
int encoderBcount = 0;

byte encoderCLast;
bool encoderCDir = true;
int encoderCcount = 0;

byte encoderDLast;
bool encoderDDir = true;
int encoderDcount = 0;


//
// control routine
//

// move the car for the given distance with the specified pwm.
void move(int distance1, int pwm1, int distance2, int pwm2, int distance3, int pwm3, int distance4, int pwm4);




void setup() {
  // setup serial ports
  Serial.begin(57600);
  
  
  // L298N motor driver
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(END, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);

  // encoders
  pinMode(ENCODER_A1, INPUT);
  pinMode(ENCODER_B1, INPUT);
  pinMode(ENCODER_C1, INPUT);
  pinMode(ENCODER_D1, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A1), ISR_encoderA, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B1), ISR_encoderB, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_C1), ISR_encoderC, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_D1), ISR_encoderD, FALLING);
}

void loop() {
   if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'f') {
      int distance1 = 4000;
      int distance2 = 4000;
      int distance3 = 4000;
      int distance4 = 4000;
      
      int pwm1 = 250;
      int pwm2 = 250;
      int pwm3 = 250;
      int pwm4 = 250;
      
      move( distance1,  pwm1, distance2,  pwm2, distance3, pwm3, distance4, pwm4);

      Serial.print("encoderA = ");
      Serial.print(encoderAcount);
      Serial.println();
      Serial.print("encoderB = ");
      Serial.print(encoderBcount);
      Serial.println();
      Serial.print("encoderC = ");
      Serial.print(encoderCcount);
      Serial.println();
      Serial.print("encoderD = ");
      Serial.print(encoderDcount);
      Serial.println();
    }
  }
}

//
// ISR for encoders
//

void ISR_encoderA() {
  byte encoderAA = digitalRead(ENCODER_A1);
  if (encoderALast == LOW && encoderAA == HIGH) {
    byte encoderAB = digitalRead(ENCODER_A2);
    if (encoderAB == LOW) {
      encoderADir = true;
    } else if (encoderAB == HIGH) {
      encoderADir = false;
    }
  }
  encoderALast = encoderAA;
  if (encoderADir) {
    encoderAcount++;
  } else {
    encoderAcount--;
  }
}

void ISR_encoderB() {
  byte encoderBA = digitalRead(ENCODER_B1);
  if (encoderBLast == LOW && encoderBA == HIGH) {
    byte encoderBB = digitalRead(ENCODER_B2);
    if (encoderBB == LOW) {
      encoderBDir = true;
    } else if (encoderBB == HIGH) {
      encoderBDir = false;
    }
  }
  encoderBLast = encoderBA;
  if (encoderBDir) {
    encoderBcount++;
  } else {
    encoderBcount--;
  }
}

void ISR_encoderC() {
  byte encoderCA = digitalRead(ENCODER_C1);
  if (encoderCLast == LOW && encoderCA == HIGH) {
    byte encoderCB = digitalRead(ENCODER_C2);
    if (encoderCB == LOW) {
      encoderCDir = true;
    } else if (encoderCB == HIGH) {
      encoderCDir = false;
    }
  }
  encoderCLast = encoderCA;
  if (encoderCDir) {
    encoderCcount++;
  } else {
    encoderCcount--;
  }
}

void ISR_encoderD() {
  byte encoderDA = digitalRead(ENCODER_D1);
  if (encoderDLast == LOW && encoderDA == HIGH) {
    byte encoderDB = digitalRead(ENCODER_D2);
    if (encoderDB == LOW) {
      encoderDDir = true;
    } else if (encoderDB == HIGH) {
      encoderDDir = false;
    }
  }
  encoderDLast = encoderDA;
  if (encoderDDir) {
    encoderDcount++;
  } else {
    encoderDcount--;
  }
}

//
// motor drive routine
//

void drive(int pwmA, int pwmB, int pwmC, int pwmD) {
  // MOTOR A direction
  if (pwmA > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (pwmA < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // MOTOR B direction
  if (pwmB > 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (pwmB < 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

    if (pwmC > 0) {
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
  } else if (pwmC < 0) {
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
  } else {
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, LOW);
  }

  // MOTOR B direction
  if (pwmD > 0) {
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);
  } else if (pwmD < 0) {
    digitalWrite(IN7, HIGH);
    digitalWrite(IN8, LOW);
  } else {
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, LOW);
  }

  // speed of motors
  analogWrite(ENA, abs(pwmA));
  analogWrite(ENB, abs(pwmB));
  analogWrite(ENC, abs(pwmC));
  analogWrite(END, abs(pwmD));
}

//
// distance control routines
//

// returns number of ticks to go the specified distance.
int ticksForDistance(int distance) {
  return (int)(distance * RESOLUTION / CIRCUMFERENCE);
}

// move the car for the given distance with the specified pwm.
void move(int distance1, int pwm1, int distance2, int pwm2, int distance3, int pwm3, int distance4, int pwm4) {
  int ticksToMove1 = ticksForDistance(distance1);
  int ticksToMove2 = ticksForDistance(distance2);
  int ticksToMove3 = ticksForDistance(distance3);
  int ticksToMove4 = ticksForDistance(distance4);
  noInterrupts();
  encoderAcount = 0;
  encoderBcount = 0;
  encoderCcount = 0;
  encoderDcount = 0;
  
  interrupts();
  
  while (encoderAcount <= ticksToMove1 && encoderBcount <= ticksToMove2 &&encoderCcount <= ticksToMove3 && encoderDcount <= ticksToMove4 ) {
    drive(pwm1, pwm2, pwm3, pwm4);
  }
  drive(0, 0, 0, 0);
}
