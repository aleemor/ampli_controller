#include <avr/wdt.h>
#define enable_pin 2
#define relay_pin 7
#define pausa 1000

bool printing=false;

int DC_OK_pin=A1;
int V24_OK=A2;

char char_in = ' ';
unsigned long prev_time_info = 0;


struct state {
  uint8_t id;
  uint8_t enable_state;
  uint8_t relay_state;
  uint8_t DC_OK;
  
  uint8_t V24;
  uint8_t temp;
  uint8_t tempp;
  uint8_t endd;
} stato;

void setup() {
  Serial.begin(9600);
  if (printing){
    Serial.println(" CONTROLLER "); 
  }
  stato.id = 0x40;
  pinMode(enable_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(enable_pin, HIGH);     // 24V OFF
  digitalWrite(relay_pin, LOW);
  printState();
  
  delay(2000);
}

void loop() { 
  read_commandss();
  displayInfo();

  delay(10);
}

void(* Reset)(void) = 0;

void read_commandss() {
  if (Serial.available() > 0) {
    char_in = Serial.read();
    exec_command();
  }
}

void exec_command() {
  switch (char_in) {
    case 0x61:    // sequenza di accensione
      digitalWrite(relay_pin, HIGH);
      beacon();
      printState();
      delay(pausa);
      digitalWrite(enable_pin, LOW);
      break;
    case 0x62:      // sequenza di spegnimento
      digitalWrite(enable_pin, HIGH); 
      beacon();
      printState();
      delay(pausa);
      digitalWrite(relay_pin, LOW);
      break;  
    case 0x63:     // relay pin HIGH    
      digitalWrite(relay_pin, HIGH);
      break;
    case 0x64:     // relay pin LOW
      digitalWrite(relay_pin, LOW);
      break;
    case 0x65:     // enable pin HIGH
      digitalWrite(enable_pin, HIGH);
      break;
    case 0x66:     // enable pin LOW
      digitalWrite(enable_pin, LOW);
      break;  
    case 0x67:                            // reset
      Reset();
      break;
  }
}

void read_statePin() {
  stato.enable_state = digitalRead(enable_pin);
  stato.relay_state  = digitalRead(relay_pin);
  stato.DC_OK        = analogRead(DC_OK_pin)/4;
  stato.V24          = analogRead(V24_OK)/4;
}

void displayInfo(){
  if (millis() > prev_time_info + 1000 ){
    printState();
    beacon();
    prev_time_info = millis();
  }
}

void beacon(){
  if (!printing){
    read_statePin();
    Serial.write((uint8_t *)&stato,sizeof(state));
  }
}

void printState(){
  if (printing){
    read_statePin();
    Serial.print(millis());
    Serial.print(" - Enable state: ");
    Serial.print(stato.enable_state);
    Serial.print(" Relay state: ");
    Serial.print(stato.relay_state);
    Serial.print(" DC_OK: ");
    Serial.print(stato.DC_OK);
    Serial.print(" 24V: ");
    Serial.print(stato.V24);
    Serial.println(" ");
  }
}
