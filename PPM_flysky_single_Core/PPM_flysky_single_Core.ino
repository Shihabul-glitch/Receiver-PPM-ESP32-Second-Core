#define ppm_pin 34
#define NUM_CHANNELS 6
#define PPM_FRAME_LENGTH 22500 // Length of PPM frame in microseconds

volatile uint16_t ppm[NUM_CHANNELS] = {0};
volatile uint8_t currentChannel = 0;
volatile unsigned long lastInterruptTime = 0;
volatile unsigned long now = 0;
volatile unsigned long pulseLength = 0;


portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR isr(){
  portENTER_CRITICAL(&synch);
  now = micros();
  pulseLength = now - lastInterruptTime;
  lastInterruptTime = now;

  if (pulseLength > 3000) { // Sync pulse, start of frame
    currentChannel = 0;
  }
  else if (currentChannel < NUM_CHANNELS) {
    ppm[currentChannel] = pulseLength;
    currentChannel++;
  }
  portEXIT_CRITICAL(&synch);
}


void setup() {
    Serial.begin(115200);
    pinMode(ppm_pin, INPUT_PULLUP);
    attachInterrupt(ppm_pin, isr, RISING);
}

void loop() {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    Serial.print(ppm[i]);
    Serial.print("  ");
  }
  Serial.println();

}
