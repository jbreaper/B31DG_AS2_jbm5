#include <Ticker.h>

// pin assignments
#define LED         15    // Pin G15, Used for the output of Signal A
#define WATCHDOG    21    // Pin G21, Used for the output of Signal B
#define PB1         22    // Pin G22, Used for the input of Button 1
#define A_IN        17    // Pin G23, Used for the input of Button 2
#define PULSE_IN    36

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

bool button_1;

float frequency_in;

float analogue_in;
float average_analogue_in;

int error_code;

void setup(){
    Serial.begin(57600);
    tick = 0;
    pinMode(WATCHDOG, OUTPUT);
    pinMode(WATCHDOG, OUTPUT);
    pinMode(PB1, INPUT);
    pinMode(A_IN, INPUT);
    ticker.attach_ms(1, tick_up);
    Serial.print("Switch, \tFrequency, \tInput");
}

void tick_up() {
    tick++;
    ticked = true;
}

void task_1(){
    digitalWrite(WATCHDOG, HIGH);
    delayMicroseconds(50);
    digitalWrite(WATCHDOG, LOW);
}

void task_2(){
    button_1 = digitalRead(PB1);
}

void task_3(){
    int high;
    high = pulseIn(PULSE_IN, HIGH);

    frequency_in = 1000000.0/(high*2);
}

void task_4(){
    analogue_in = analogRead(A_IN);
}

void task_5(){
    average_analogue_in;
}

void task_6(){
    for (int i = 0; i < 1000; i++){
        __asm__ __volatile__ ("nop");
    }
}

void task_7(){
    if (average_analogue_in > (3.3/2)){
        error_code = 1;
    }else{
        error_code = 0;
    }
}

void task_8(){
    digitalWrite(LED, error_code);
}

void task_9(){
    Serial.print(button_1);
    Serial.print(", \t");
    Serial.print(frequency_in);
    Serial.print(", \t");
    Serial.print(average_analogue_in);
    Serial.print("\n");
}

void loop(){
    task_1();
    if(ticked){
        if((tick%R_T2)==0) task_2;
        if((tick%R_T3)==0) task_3;
        if((tick%R_T4)==0) task_4;
        if((tick%R_T5)==0) task_5;
        if((tick%R_T6)==0) task_6;
        if((tick%R_T7)==0) task_7;
        if((tick%R_T8)==0) task_8;
        if((tick%R_T9)==0) task_9;
        ticked = false;
    }
}
