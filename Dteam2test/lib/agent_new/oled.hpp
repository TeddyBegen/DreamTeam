#if !defined(oled_HPP_)
#define oled_HPP_

#include <memory>
#include <queue>

#include <information.hpp>


#define OLED_RESET 0 // GPIO0

#define SDA_PIN 32  // Change this to the desired GPIO for SDA
#define SCL_PIN 27  // Change this to the desired GPIO for SCL



namespace dteam
{

    enum class dislpay_states
    {
        
        
    };

 
    class oled
    {

    protected:
        std::shared_ptr<dteam::node> m_node;

    private:
        bool disp_init_done = false;


    public:
        oled(){};
        oled(std::shared_ptr<dteam::node> node): m_node(node){};
        virtual ~oled() = default;
        
    public:
        void initialize();
        void performAction(int string);   
        bool Disp_ok();
        


    };
}
#endif
