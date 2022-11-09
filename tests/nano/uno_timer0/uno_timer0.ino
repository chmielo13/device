

void setup() {
    Serial.begin(9600);

    while (!Serial);

    Serial.println("Hello world from timer 0 test!!");

    pinMode(2, OUTPUT);
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, OUTPUT);


    // Using the timer interrupt techinque described here:
    // https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function below
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);

}


// Interrupt is called once a millisecond, to update the LEDs
SIGNAL(TIMER0_COMPA_vect) 
{
  
    if (digitalRead(A4) == 0) {
        digitalWrite(A5, HIGH);
    } else {
        digitalWrite(A5, LOW);
    }

} 


void loop() {

    digitalWrite(2, HIGH);
    delay(1000);

    digitalWrite(2, LOW);
    delay(1000);

    // if (digitalRead(A4) == 0) {
    //     digitalWrite(A5, HIGH);
    // } else {
    //     digitalWrite(A5, LOW);
    // }

}



