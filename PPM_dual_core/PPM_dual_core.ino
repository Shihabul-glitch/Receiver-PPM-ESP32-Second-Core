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


TaskHandle_t Task1; // An identity for task, sothat it can be called from any place.
void codeForTask1(void* parameter) {
  // setup section: Code written here runs once.
  pinMode(ppm_pin, INPUT_PULLUP);
  attachInterrupt(ppm_pin, isr, RISING);
  for (;;) { // Infinite loop section, just like the void loop()
  }
}





void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(  
  codeForTask1,
  "led1Task",
  10000,
  NULL,
  tskIDLE_PRIORITY,
  &Task1,
  0);
  delay(500);  // needed to start-up task1
}

void loop() {

  // Print PPM values for each channel
  for (int i = 0; i < NUM_CHANNELS; i++) {
    Serial.print(ppm[i]);
    Serial.print("  ");
  }
  Serial.println();

  //delay(1000);
}
