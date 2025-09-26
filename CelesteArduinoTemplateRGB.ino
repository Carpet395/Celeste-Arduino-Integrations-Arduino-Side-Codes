/*
  Celeste Arduino-side Template.
  in the template, pin 4 is used as an easy ground for my rgb led connection.
  the template was made with the arduino giga r1 wifi, pwm pins vary between models.
*/

// the pin the led in the example is connected on
int R = 5;
int G = 6;
int B = 7;

// an analog LED system i made
const unsigned long LOOP_INTERVAL = 10;  // 10 ms = 100 Hz
unsigned long lastLoopTime = 0;
float SmoothNess = 0.1;
int TargetR = 0;
int TargetG = 0;
int TargetB = 0;
float CurrentR = 0;
float CurrentG = 0;
float CurrentB = 0;

// trigger Data
bool Pinged = false;

// forward declaration for functions
void OnPing(bool First = false, bool Silent = false);
void OnDeath(bool First = false, bool Silent = false);
void OnSpawn(bool First = false, bool Silent = false);
void OnComplete(bool First = false, bool Silent = false);

void setup() {
  Serial.begin(9600);  // must match baud rate in your C# code
  while (!Serial) {
    ;  // wait until USB serial is connected
  }
  Serial.println("init");
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}


// variables for timed functions
unsigned long now = 0;
unsigned long PingedAt = 0;
unsigned long DiedAt = 0;
unsigned long SpawnedAt = 0;
unsigned long CompleteAt = 0;

// the analog LED system i made
void LED_Loop() {
  //Serial.println("r");
  analogWrite(R, int(CurrentR += (TargetR - CurrentR) * SmoothNess));
  //Serial.println("r2");
  //delay(1);
  //Serial.println("g");
  analogWrite(G, int(CurrentG += (TargetG - CurrentG) * SmoothNess));
  //Serial.println("g2");
  //delay(1);
  //Serial.println("b");
  analogWrite(B, int(CurrentB += (TargetB - CurrentB) * SmoothNess));
  //Serial.println("b2");
  //delay(1);
  //Serial.println("yeppe");
}

// The main loop, ideally shouldnt change much since it has all the triggers on updates
void loop() {
  now = millis();
  if (now - lastLoopTime >= LOOP_INTERVAL) {
    lastLoopTime += LOOP_INTERVAL;
    LED_Loop();
  }
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // read until newline
    input.trim();                                 // ensures that itll get the true value sent
    // important for initalizing check.
    if (input.equalsIgnoreCase("ping")) {
      OnPing(true);
    }
    // called when player dies
    if (input.equalsIgnoreCase("death")) { 
      OnDeath(true);
    }
    // called when player respawns
    else if (input.equalsIgnoreCase("spawn")) {
      OnSpawn(true);
    }
    // called when the player finished a chapter ("finishing" a chapter is counted when the speedrun counter stops counting) *mod version 0.2.0+
    else if (input.equalsIgnoreCase("complete")) 
    {
      OnComplete(true);
    }
  } else {
    OnDeath(false);
    OnSpawn(false);
    OnComplete(false);
    OnPing(false);
  }
}
// function that's called when Pinged
void OnPing(bool First, bool Silent) {
  if (First) {
    PingedAt = now;
    TargetB = 255;
    delay(150);
    Serial.println("pong");  // reply with newline, Do NOT remove.
  }
  if (!Silent) {
    if (now - PingedAt >= 500 && now - PingedAt < 510)
      TargetB = 0;
  }
}

void OnDeath(bool First, bool Silent) {
  if (First) {
    DiedAt = now;
    TargetR = 200;
    TargetG = 10;
    TargetB = 10;
  }
  if (!Silent) {
    if (now - DiedAt >= 250 && now - DiedAt < 500)
      TargetR = 30;
    else if (now - DiedAt >= 500 && now - DiedAt < 750)
      TargetR = 200;
    else if (now - DiedAt >= 750 && now - DiedAt < 760) {
      TargetR = 0;
      TargetG = 0;
      TargetB = 0;
    }
  }
}

void OnSpawn(bool First, bool Silent) {
  if (First) {
    SpawnedAt = now;
    TargetG = 128;
    TargetB = 128;
  }
  if (!Silent) {
    if (now - SpawnedAt >= 250 && now - SpawnedAt < 500) {
      TargetG = 100;
        TargetB = 100;
    }
    else if (now - SpawnedAt >= 500 && now - SpawnedAt < 750) {
      TargetG = 145;
      TargetB = 145;
    }
    else if (now - SpawnedAt >= 750 && now - SpawnedAt < 1000) {
      TargetG = 128;
      TargetB = 128;
    }
    else if (now - SpawnedAt >= 1000 && now - SpawnedAt < 1010) {
      TargetG = 0;
      TargetB = 0;
    }
  }
}
void OnComplete(bool First, bool Silent) 
{
  if (First) 
  {
    CompleteAt = now;
    TargetR = 255;
    TargetG = 255;
  }
  if (!Silent) 
  {
    if (now - CompleteAt >= 500 && now - CompleteAt < 1000) {
      TargetB = 255;
      TargetG = 0;
    }
    else if (now - CompleteAt >= 1000 && now - CompleteAt < 1500) {
      TargetG = 255;
      TargetR = 0;
    }
    else if (now - CompleteAt >= 1500 && now - CompleteAt < 2000) {
      TargetR = 255;
      TargetG = 0;
    }
    else if (now - CompleteAt >= 2000 && now - CompleteAt < 2500) {
      TargetG = 255;
      TargetB = 0;
    }
    else if (now - CompleteAt >= 2500 && now - CompleteAt < 3000) {
      TargetG = 0;
      TargetR = 0;
    }
  }
}

