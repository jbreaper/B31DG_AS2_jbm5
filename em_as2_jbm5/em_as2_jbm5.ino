#include <Ticker.h>

// pin assignments
#define LED         14    // Pin G15, Used for the output of Signal A
#define WD          19    // Pin G21, Used for the output of Signal B
#define PB1         12    // Pin G22, Used for the input of Button 1
#define A_IN        0    // Pin G23, Used for the input of Button 2
#define PULSE_IN    19

// Rate of task (ms)
#define R_T2        200
#define R_T3        1000
// while hz to ms gives 41.666666... for multiple
// reasons this has been rounded to 42
#define R_T4        42
#define R_T5        42
#define R_T6        100
// while hz to ms gives 333.33333... for multiple
// reasons this has been rounded to 333
#define R_T7        333
#define R_T8        333
#define R_T9        5000

Ticker ticker;

volatile int tick;
volatile bool ticked;

bool button_1 = false;

float frequency_in = 0;

float analogue_in;
float average_analogue_in = 0;
float analogues[4];

int error_code;


void setup() {
  Serial.begin(57600);
  tick = 0;
  analogues[0] = 0;
  analogues[1] = 0;
  analogues[2] = 0;
  analogues[3] = 0;
  pinMode(LED, OUTPUT);
  pinMode(WD, OUTPUT);
  pinMode(PB1, INPUT);
  pinMode(A_IN, INPUT);
  pinMode(PULSE_IN, INPUT);
  ticker.attach_ms(1, tick_up);
  Serial.print("\nSwitch, \tFrequency, \tInput \n");
}

void tick_up() {
  tick++;

  task_1();
  if((tick%R_T2)==0)    task_2();
  if((tick%R_T3) == 0)  task_3();
  if((tick%R_T4)==0)    task_4();
  if((tick%R_T5)==21)   task_5();
  if((tick%R_T6)==0)    task_6();
  if((tick%R_T7) == 0)  task_7();
  if((tick%R_T8)==167)  task_8();
  if((tick%R_T9) == 0)  task_9();
}

void task_1() {
  digitalWrite(WD, HIGH);
  delayMicroseconds(50);
  digitalWrite(WD, LOW);
}

void task_2() {
  button_1 = digitalRead(PB1);
}

void task_3() {
  float high;
  high = pulseIn(PULSE_IN, LOW);
  frequency_in = 1000000.0 / (high * 2);
}

void task_4() {
  for (int i = 1; i < 4; i++) {
    analogues[i - 1] = analogues[i];
    average_analogue_in += analogues[i];
  }

  Serial.print(analogRead(A_IN));
  analogues[3] =  analogRead(A_IN);
}

void task_5() {
  average_analogue_in = 0;

  for (int i = 0; i < 4; i++) {
    average_analogue_in += analogues[i];
  }

  average_analogue_in = average_analogue_in / 4;
}

void task_6() {
  for (int i = 0; i < 1000; i++) {
    __asm__ __volatile__ ("nop");
  }
}

void task_7() {
  if (average_analogue_in > (3.3 / 2)) {
    error_code = 1;
  } else {
    error_code = 0;
  }
}

void task_8() {
  digitalWrite(LED, error_code);
}

void task_9() {
  Serial.print(button_1);
  Serial.print(", \t");
  Serial.print(frequency_in);
  Serial.print(", \t");
  Serial.print(average_analogue_in);
  Serial.print("\n");
}

void loop() {}
