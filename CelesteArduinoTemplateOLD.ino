/*
  Celeste Arduino-side Template.
  in the template, pin11 is connected to an led.
  the template was made with the arduino uno r3, pwm pins vary between models.
*/

bool FlashOnRecconect = true;

void setup() {
  Serial.begin(9600); // must match baud rate in your C# code
    while (!Serial) {
    ; // wait until USB serial is connected
  }
  Serial.println("init");
  pinMode(11, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // read until newline
    input.trim(); // ensures that itll get the true value sent
    // important for initalizing check. 
    if (input.equalsIgnoreCase("ping"))
    {
      Serial.println("pong"); // reply with newline, Do NOT remove.
      if (FlashOnRecconect) 
      {
        digitalWrite(11, HIGH);
        delay(500);
        digitalWrite(11, LOW); 
      }
    }
    // called when player dies
    else if (input.equalsIgnoreCase("death"))
    {
      digitalWrite(11, HIGH);
      delay(250);
      analogWrite(11, 80);
      delay(250);
      digitalWrite(11, HIGH);
      delay(250);
      digitalWrite(11, LOW); 
    }
    // called when player respawns
    else if (input.equalsIgnoreCase("spawn")) 
    {
      analogWrite(11, 128);
      delay(1000);
      digitalWrite(11, LOW); 
    }
  }
}
