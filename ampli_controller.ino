#include <avr/wdt.h>

#define power_pin 7
#define relay_pin 8
#define blinking_time 100

unsigned long prev_time = 0;
uint8_t rec, in;
bool flagg = 0;
int count = 0;
uint8_t command[3];


struct state {
  bool power_state;
  bool relay_state;
} stato;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(power_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(power_pin, LOW);
  digitalWrite(relay_pin, LOW);

  blinking();
  delay(1000);
}

void loop() {
  if (millis() - prev_time >= 10) {
    read_commandss();
    prev_time = millis();
  }
}

void resett () {
  //wdt_enable(WDTO_30MS);
  //while (1) ;
  
}

void(* Reset)(void) = 0;

void read_commandss() {
  if (Serial.available() > 0) {
    in = Serial.read();
    if (flagg) {
      command[count++] = in;
      if (in == 0x26) {
        exec_command();
        count = 0;
        flagg = 0;
        return;
      }
    }
    if (in == 0x25) {
      flagg = 1;
    }
    delay(10);
  }
}

void exec_command() {
  switch (command[0]) {
    case 0xaa:
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
    case 0xab:
      //resett();
      Reset();
      break;
    case 0xac:
      digitalWrite(power_pin, HIGH);
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
    case 0xad:
      digitalWrite(power_pin, LOW);
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
    case 0xae:
      digitalWrite(relay_pin, HIGH);
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
    case 0xaf:
      digitalWrite(relay_pin, LOW);
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
    case 0xba:
      digitalWrite(relay_pin, HIGH);
      delay(3000);
      digitalWrite(power_pin, HIGH);
      delay(10);
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
    case 0xbb:
      digitalWrite(power_pin, LOW);
      delay(3000);
      digitalWrite(relay_pin, LOW);
      delay(10);
      read_statePin();
      Serial.write((byte *)&stato, sizeof(stato));
      break;
  }
}

void read_statePin() {
  stato.power_state = digitalRead(power_pin);
  stato.relay_state = digitalRead(relay_pin);

}

void blinking() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(blinking_time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(blinking_time);
}
