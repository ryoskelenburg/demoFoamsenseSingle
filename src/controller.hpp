#ifndef controller_hpp
#define controller_hpp

#include <stdio.h>

class controller {
    
public:
    controller(int firstValve, int secondValve);
    void supply();
    void vacume();
    void open();
    void shut();
    
private:
    int m_firstValve;
    int m_secondValve;
    int m_arduino;
    
    //ofArduino com; //arduino
};

#endif
