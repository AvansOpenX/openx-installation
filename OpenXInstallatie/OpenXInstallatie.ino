#define BTN_PIN_1 2
#define BTN_PIN_2 13
#define BTN_PIN_3 12
#define BTN_PIN_4 11
#define BTN_PIN_5 10
#define BTN_PIN_6 9
#define LED_PIN_1 6
#define LED_PIN_2 5
#define LED_PIN_3 7
#define LED_PIN_4 8
#define LED_PIN_5 4
#define LED_PIN_6 3

long gameLength = 30000; // 30 seconds
long idleLength = 500;
int currentScore = 0;

class Led {
  private:
    byte pin;
  
  public:
    Led(byte pin) {
      this->pin = pin;
      init();
    }
  
    void init() {
      pinMode(pin, OUTPUT);
      off();
    }
  
    void on() {
      digitalWrite(pin, HIGH);
    }
  
    void off() {
      digitalWrite(pin, LOW);
    }
};

class Button {
  private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
  
  public:
  	Led *led;
  
    Button(byte pin, byte ledPin) {
      this->pin = pin;
      this->led = new Led(ledPin);
      lastReading = LOW;
      init();
    }
  
    void init() {
      pinMode(pin, INPUT_PULLUP);
      update();
    }
  
    void update() {
      byte newReading = digitalRead(pin);
      if (newReading != lastReading) {
        lastDebounceTime = millis();
      }
      if (millis() - lastDebounceTime > debounceDelay) {
        state = newReading;
      }
      lastReading = newReading;
    }
  
    byte getState() {
      update();
      return state;
    }
  
    bool isPressed() {
      return (getState() != HIGH);
    }
};

Button buttons[] = {
  Button(BTN_PIN_1, LED_PIN_1),
  Button(BTN_PIN_2, LED_PIN_2),
  Button(BTN_PIN_3, LED_PIN_3),
  Button(BTN_PIN_4, LED_PIN_4),
  Button(BTN_PIN_5, LED_PIN_5),
  Button(BTN_PIN_6, LED_PIN_6),
};
  
void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  idleAnimation();
  Serial.println(buttons[0].isPressed());
  if (buttons[0].isPressed()) runGame();
}

void idleAnimation() {
  static bool ledsOn;
  unsigned static long previousBlink = millis();
  if (millis() - previousBlink >= idleLength) {
    previousBlink = millis();
  	for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
      if (ledsOn) buttons[i].led->off();
      else buttons[i].led->on();
  	}
    ledsOn = !ledsOn;
  }
}

void runGame() {
  for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
    buttons[i].led->off();
  }
  delay(1000);
  gameLoop();
  for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
    buttons[i].led->on();
  }
  Serial.print("Game ended, score:");
  Serial.println(currentScore);
  currentScore = 0;
  delay(2000);
}

void gameLoop() {
  int activeButton = 0;
  buttons[activeButton].led->on();
  unsigned long startTime = millis();
  // Start looping for as long as a game should last
  while(millis() - startTime < gameLength) {
    // Detect whether the correct button has been pressed
    if (buttons[activeButton].isPressed()) {
      currentScore++;
      buttons[activeButton].led->off();
      // Select a new button to press
      while(true) {
      	int nextButton = random(sizeof buttons/sizeof buttons[0] - 1);
        if (nextButton == activeButton) continue;
        activeButton = nextButton;
      	break;
      }
      buttons[activeButton].led->on();
    }
  	delay(15);
  }
}
