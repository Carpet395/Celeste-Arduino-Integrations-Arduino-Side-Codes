/*
  Celeste Arduino-side Template.
  in the template, pin11 is connected to an led.
  the template was made with the arduino uno r3, pwm pins vary between models.
*/

// the pin the led in the example is connected on
int Pin = 11;

// an analog LED system i made
const unsigned long LOOP_INTERVAL = 10;  // 10 ms = 100 Hz
unsigned long lastLoopTime = 0;
float SmoothNess = 0.1;
// remember original smoothness
float const OrigSmoothNess = 0.1;
int TargetBrightness = 0;
float CurrentBrightness = 0;

// trigger Data
bool Pinged = false;

// forward declaration for functions
void OnPing(bool First = false, bool Silent = false);
void OnDeath(bool First = false, bool Silent = false);
void OnSpawn(bool First = false, bool Silent = false);
void OnComplete(bool First = false, bool Silent = false);
void OnCut(bool First = false, bool Silent = false);
void OnCollect(bool First = false, bool Silent = false);

void setup() {
  Serial.begin(9600);  // must match baud rate in your C# code
  while (!Serial) {
    ;  // wait until USB serial is connected
  }
  Serial.println("init");
  pinMode(Pin, OUTPUT);
}


// variables for timed functions
unsigned long now = 0;
unsigned long PingedAt = 0;
unsigned long DiedAt = 0;
unsigned long SpawnedAt = 0;
unsigned long CompleteAt = 0;
unsigned long CutAt = 0;
unsigned long CollectAt = 0;

// the analog LED system i made
void LED_Loop() {
  analogWrite(Pin, int(CurrentBrightness += (TargetBrightness - CurrentBrightness) * SmoothNess));
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
    else if (input.equalsIgnoreCase("complete")) {
      OnComplete(true);
    }
    // called when the player collects a strawberry 
    else if (input.equalsIgnoreCase("collected")) {
      OnCollect(true);
    } 
    // called when a cutscene starts playing
    else if (input.equalsIgnoreCase("cutscene")) {
      OnCut(true);
    }
  } else {
    OnDeath(false);
    OnComplete(false);
    OnCollect(false);
    OnCut(false);
    OnSpawn(false);
    OnPing(false);
  }
}
// function that's called when Pinged
void OnPing(bool First, bool Silent) {
  if (First) {
    PingedAt = now;
    TargetBrightness = 255;
    Serial.println("pong");  // reply with newline, Do NOT remove.
  }
  if (!Silent) {
    if (now - PingedAt >= 500 && now - PingedAt < 510)
      TargetBrightness = 0;
  }
}

void OnDeath(bool First, bool Silent) {
  if (First) {
    DiedAt = now;
    TargetBrightness = 30;
  }
  if (!Silent) {
    if (now - DiedAt >= 250 && now - DiedAt < 500)
      TargetBrightness = 255;
    else if (now - DiedAt >= 500 && now - DiedAt < 750)
      TargetBrightness = 30;
    else if (now - DiedAt >= 750 && now - DiedAt < 1000)
      TargetBrightness = 255;
    else if (now - DiedAt >= 1000 && now - DiedAt < 1010)
      TargetBrightness = 0;
  }
}

void OnSpawn(bool First, bool Silent) {
  if (First) {
    SpawnedAt = now;
    TargetBrightness = 128;
  }
  if (!Silent) {
    if (now - SpawnedAt >= 255 && now - SpawnedAt < 500)
      TargetBrightness = 100;
    else if (now - SpawnedAt >= 500 && now - SpawnedAt < 750)
      TargetBrightness = 145;
    else if (now - SpawnedAt >= 750 && now - SpawnedAt < 1000)
      TargetBrightness = 128;
    else if (now - SpawnedAt >= 1000 && now - SpawnedAt < 1010)
      TargetBrightness = 0;
  }
}
void OnComplete(bool First, bool Silent) {
  if (First) {
    CompleteAt = now;
    TargetBrightness = 128;
  }
  if (!Silent) {
    if (now - CompleteAt >= 255 && now - CompleteAt < 500)
      TargetBrightness = 168;
    else if (now - CompleteAt >= 500 && now - CompleteAt < 750)
      TargetBrightness = 208;
    else if (now - CompleteAt >= 750 && now - CompleteAt < 1000)
      TargetBrightness = 228;
    else if (now - CompleteAt >= 1000 && now - CompleteAt < 1010)
      TargetBrightness = 255;
    else if (now - CompleteAt >= 1250 && now - CompleteAt < 1260)
      TargetBrightness = 0;
  }
}
void OnCut(bool First, bool Silent) {
  if (First) {
    CutAt = now;
    SmoothNess = 1;
    TargetBrightness = 250;
  }
  if (!Silent) {
    if (now - CutAt >= 125 && now - CutAt < 250)
      TargetBrightness = 0;
    else if (now - CutAt >= 250 && now - CutAt < 375)
      TargetBrightness = 250;
    else if (now - CutAt >= 375 && now - CutAt < 385) {
      TargetBrightness = 0;
    }
    else if (now - CutAt >= 405 && now - CutAt < 420) {
      SmoothNess = OrigSmoothNess;
    }
  }
}
void OnCollect(bool First, bool Silent) {
  if (First) {
    CollectAt = now;
    SmoothNess = 1;
    TargetBrightness = 100;
  }
  if (!Silent) {
    if (now - CollectAt >= 250 && now - CollectAt < 500)
      TargetBrightness = 250;
    else if (now - CollectAt >= 500 && now - CollectAt < 510) {
      SmoothNess = OrigSmoothNess;
      TargetBrightness = 0;
    }
  }
}
