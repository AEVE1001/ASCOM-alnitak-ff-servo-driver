//Código Experimental. No usar en producción. 


#include <Servo.h>

Servo servoA;

int pos = 0;

void setup(){
    servoA.attach(9);
}

void loop() {
    for (pos = 0; pos <= 180; pos +=1)

    servoA.write(pos);
    delay(15);

    for (pos = 180; pos >= 0; pos -= 1){
         servoA.write(pos);
        delay(15); 
    }
}
