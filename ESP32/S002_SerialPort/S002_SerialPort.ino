void setup() {
  Serial.setTimeout(100);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    Serial.print("Echo: ");
    Serial.println(msg);
  }
}
