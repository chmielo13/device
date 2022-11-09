

#define PRESSURE_SENSOR_ANALOG_PIN A7

// The pressure sensor analog output (0-5V) is scaled using a voltage divider to (0-3.3V)
// Then, the output that corresponds to 10-90% of that voltage range corresponds to [-15 to +15] PSI

void setup() {
    Serial.begin(9600);
    pinMode(PRESSURE_SENSOR_ANALOG_PIN, INPUT);
    // pinMode(PRESSURE_SENSOR_ANALOG_PIN, ANALOG_INPUT);
}

void loop() {
    // Serial.println("Hello from 15 pressure sensor test!");
    int rawAnalogInput = analogRead(PRESSURE_SENSOR_ANALOG_PIN);
    int maxAnalogCount = 1023;

    // [0 to 1023] 


    // An 80% bandwidth corresponds to 30 PSI of value.
    // 100% bandwidth corresponds to 30PSI/0.8 = 37.5 PSI.
    // One analog count = 1 / 1024 * 37.5

    float valueOfOneAnalogCount = 37.5 / 1024;

    int relativeAnalogInput = rawAnalogInput - 512;
    int relativePressurePSI = relativeAnalogInput * valueOfOneAnalogCount;

    Serial.print("Pressure: ");
    Serial.print(relativePressurePSI);
    Serial.print(" (PSI).    ");
    Serial.print("Raw Analog Input: ");
    Serial.print(rawAnalogInput);


    Serial.println();
}

