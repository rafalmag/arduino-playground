#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

int ledPin = 5;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

  Serial.print("Hall sensor measurement: ");
  Serial.println(hallRead());

  Serial.print("Internal temp measurement: ");
  Serial.println((temprature_sens_read() - 32) / 1.8);

  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
