int photodiode;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

void loop() {
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //digitalWrite(8, !digitalRead(8));

  photodiode = analogRead(A0);
  
  Serial.println(photodiode);
  //delay(500);
  if (photodiode <= 46)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(8, HIGH);
    //delay(500);
  } else
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(8, LOW);
  }
  delay(50);
}
