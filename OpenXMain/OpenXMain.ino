
class Plant {
  private:

  public:
    void init() {

    }

    bool needsWater() {
      return true;
    }
}

class MoistureSensor {
  private:
    byte pin;

  public:
    void init() {

    }

    int getLevel() {
      return 0;
    }
}

class Lamp {
  private:
    byte pin;
    bool state;

  public:
    void init() {

    }

    void on() {

    }

    void off() {

    }
}

class WaterValve {
  private:
    byte pin;
    int state;

  public:
    void init() {

    }

    void open(int percentage) {

    }

    void close() {

    }
}

class WaterReservoir {
  private:

  public:
    void init() {

    }

    int getLevel() {
      return 0;
    }

    void add(int milliseconds) {

    }

    void remove(int milliseconds) {

    }
}

class Battery {
  private:
    byte pin;
    int percentage;

  public:
    void init() {

    }

    int getLevel() {
      return 0;
    }

    void add() {
      
    }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  
}