void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  Serial3.println("TEST");
  delay(500);
}
