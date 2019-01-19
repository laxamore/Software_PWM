int freq = 338;

void setup() {
  Serial.begin(9600);

  pinMode(10, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);

  digitalWrite(50, HIGH);
  digitalWrite(51, LOW);
}

String data = "";
int pwm = 0;

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      pwm = data.toInt();
      data = "";
    }
    else {
      data += c;
    }
  }

  //  for ( int i = 0; i <= 255; i++) {
  //    unsigned long delayMillis = millis();
  //    while ((millis() - delayMillis) < 10) {
  //      softAnalogWrite(53, i);
  //      analogWrite(10, i);
  //    }
  //  }
  //
  //  for (int i = 255; i >= 0; i--) {
  //    unsigned long delayMillis = millis();
  //    while ((millis() - delayMillis) < 10) {
  //      softAnalogWrite(53, i);
  //      analogWrite(10, i);
  //    }
  //  }

  softAnalogWrite(53, pwm);
  analogWrite(10, pwm);
}

unsigned long us0 = micros();
unsigned long us1 = micros();

int state = 0;

void softAnalogWrite(int a, double b) {
  double dutyC = (b / 255) * (1000000 / freq);

  if ((b / 255) == 0) {
    digitalWrite(a, LOW);
    us0 = micros();
    us1 = micros();
  }
  else if ((b / 255) == 1) {
    digitalWrite(a, HIGH);
    us0 = micros();
    us1 = micros();
  }
  else {
    if (state == 0) {
      us1 = micros();
      if ((micros() - us0) >= ((1000000 / freq) - dutyC)) {
        us0 = micros();
        digitalWrite(a, HIGH);
        state = 1;
      }
    }
    else if (state == 1) {
      us0 = micros();
      if ((micros() - us1) >= dutyC) {
        us1 = micros();
        digitalWrite(a, LOW);
        state = 0;
      }
    }
  }
}

