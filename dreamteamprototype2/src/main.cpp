#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <LOLIN_I2C_BUTTON.h>


#include <information.hpp>

#define OLED_RESET 0 // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define SDA_PIN 32  // Change this to the desired GPIO for SDA
#define SCL_PIN 27  // Change this to the desired GPIO for SCL


//displayens pixelkoordinarer är 32,8 -> 90,8 alla karaktärer tar 5px
//                              32,24 -> 90,24 


I2C_BUTTON button(DEFAULT_I2C_BUTTON_ADDRESS); 

//String keyString[] = {"None", "Press", "Long", "Double", "Hold"};


enum class State{
  ID,
  Info,
  Plats
};

#define name random(16)



class StateMachine{
private:
  State currentState;
  State prevState;
  State nextState;

  public:
    StateMachine(): currentState(State::ID ){}
    void transition(u_char pressedButton){

      if( pressedButton == button.BUTTON_A)
      {
          currentState = prevState;
      }

      if (pressedButton == button.BUTTON_B)
      {
        currentState = nextState;
      }

  }


  void performAction(march::information &info){

    switch (currentState)
    {
    case State::ID:

      display.clearDisplay();
      display.setCursor(50,8);
      display.println("ID: " + String(info.get_ID()));

      display.setCursor(32,16);
      display.println("QUEUE:");
      display.setCursor(90,16);
      display.println("2");

      display.setCursor(32,24);
      display.println("REACH:");
      display.setCursor(70,24);
      display.println("50KM");

      display.display();
      nextState = State::Info;
      prevState = State::Plats;

      break;

    case State::Info:

      display.clearDisplay();
      display.setCursor(53,8);
      display.println("INFO");

      display.setCursor(32,16);
      display.println("BAT:");
      display.setCursor(79,16);
      display.println(info.get_battery_level());

      display.setCursor(32,24);
      display.println("LOAD:");
      display.setCursor(77,24);
      display.println("20T");


      nextState = State::Plats;
      prevState = State::ID;

      break;

    case State::Plats:
      display.clearDisplay();
      display.setCursor(50,8);
      display.println("PLATS");

      display.setCursor(32,16);
      display.println("CUR:");
      display.setCursor(80,16);
      display.println(info.get_position());

      display.setCursor(32,24);
      display.println("DEST:");
      display.setCursor(80,24);
      display.println(info.get_destination());

      nextState = State::ID;
      prevState = State::Info;

      break;


  }



}



};

void setup() {
  Serial.begin(115200);

  // Set custom SDA and SCL pins for Wire library
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // init done
  display.display();
  // Clear the buffer.
  display.clearDisplay();

  // Text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();



}



void loop() 
{
  static StateMachine stateMachine;
  static march::information info(name);  // Replace '1' with your desired initial ID


  stateMachine.performAction(info);

    if (button.get() == 0) // Button press
    {
        
        if (button.BUTTON_A)
        {
            
            Serial.println("A");

            stateMachine.transition(button.BUTTON_A);
            

        }

        if (button.BUTTON_B)
        {
            Serial.println("B");

            stateMachine.transition(button.BUTTON_B);
            

        }
        
    }
    else
    {
        display.println("Test...");
    }

    display.display();
    

}