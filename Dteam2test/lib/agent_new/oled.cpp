#include <Wire.h>
#include <iostream>
#include <Adafruit_SSD1306.h>
#include <LOLIN_I2C_BUTTON.h>
#include <information.hpp>
#include <oled.hpp>
#include <node.hpp>
#include <state.hpp>

Adafruit_SSD1306 display(OLED_RESET);



//displayens pixelkoordinarer är 32,8 -> 90,8 alla karaktärer tar 5px
//                              32,24 -> 90,24 

namespace dteam {

  I2C_BUTTON button(DEFAULT_I2C_BUTTON_ADDRESS); 
/*
auto display::initialize(std::shared_ptr<dteam::node> node_ptr) -> void{

}

*/



void oled::initialize(){


  if (!disp_init_done)
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
  display.setCursor(35,10);
  display.println("init done!");
  display.display();

  disp_init_done = true;

}
bool oled::Disp_ok(){
  return disp_init_done;
}



void oled::performAction(int state){

  

    switch (state)
    {
    case 1: // Init


      break;

    case 2: // Idle

      display.setCursor(32,8);
      display.println("in idle");

      break;

    case 3: // main mission



      break;


    case 4: //transport enter

      display.clearDisplay();
      display.setCursor(32,8);
      display.println("Transport");

      display.setCursor(32,16);
      display.println("CUR:");
      display.setCursor(80,16);
      display.println(m_node->get_information().get_position());

      display.setCursor(32,24);
      display.println("DEST:");
      display.setCursor(80,24);
      display.println(m_node->get_information().get_destination());

      break;

    case 5: //charging

      display.clearDisplay();
      display.setCursor(32,8);
      display.println("-> Charging");

      display.setCursor(32,16);
      display.println("CB:"+ int(m_node->get_information().get_battery_level())); //current battery
      display.println(" RB: " + int(m_node->get_calculator().get_battry_range())); // required battery

      display.setCursor(32,24);
      display.println("DEST:");
      display.setCursor(80,24);
      display.println(m_node->get_information().get_destination());
      
      break;

    case 6: // arrival
      display.clearDisplay();
      display.setCursor(32,8);
      display.println("At station");

      display.setCursor(32,24);
      display.println("Broadcasting");
          
      break;

    case 7: //queue
      display.clearDisplay();
      display.setCursor(32,8);
      display.println("Queue: FIX" );

      display.setCursor(32,24);
      display.println("prio: " + int (m_node->get_calculator().get_priority_score()));    
      break;

    case 8: // charging
      display.clearDisplay();
      display.setCursor(32,8);
      display.println("Charging");

      display.setCursor(32,16);
      display.println("REQ: " ); // required battery
      display.setCursor(80,16);
      display.println(int(m_node->get_calculator().get_battry_range()));

      display.setCursor(32,24);
      display.println("CUR:");
      display.setCursor(80,24);
      display.println(m_node->get_information().get_battery_level());  

      break;




  }

}







}

/*
    private:
  State currentState;
  State prevState;
  State nextState;

  public:
    display::StateMachine(): currentState(State::ID ){}
    
    
    

        
    


  }


  

};
*/





/*
void loop() 
{
  static StateMachine stateMachine;
  static dteam::information info(name);  // Replace '1' with your desired initial ID


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
}
*/