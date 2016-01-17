void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial Initialized");
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available()){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }
}
