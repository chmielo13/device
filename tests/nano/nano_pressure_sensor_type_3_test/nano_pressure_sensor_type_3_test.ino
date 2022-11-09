

const int PRESSURE_SENSOR_PIN = A7;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("Nano Pressure Sensor Type 3 Test");


    pinMode(PRESSURE_SENSOR_PIN, INPUT);

}


void loop() {

    int result = analogRead(PRESSURE_SENSOR_PIN);
    Serial.print("Pressure Sensor Voltage: ");
    Serial.println(result);

}



