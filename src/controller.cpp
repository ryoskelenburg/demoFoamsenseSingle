#include "controller.hpp"
#include "ofArduino.h"

controller::controller(int firstValve, int secondValve ) {
    m_firstValve = firstValve;
    m_secondValve = secondValve;
    //m_arduino = arduino;
    
    //com.sendDigitalPinMode(m_firstValve, ARD_OUTPUT);
    //com.sendDigitalPinMode(m_secondValve, ARD_OUTPUT);
}

void controller::supply(void) {
    //com.sendDigital(m_firstValve, ARD_HIGH);
    //com.sendDigital(m_secondValve, ARD_HIGH);
    //  return On;
}

//void controller::vacume(void) {
//    digitalWrite(m_firstValve, LOW);
//    digitalWrite(m_secondValve, LOW);
//    digitalWrite(m_thirdValve, HIGH);
//}
//
//void controller::shut(void) {
//    digitalWrite(m_firstValve, LOW);
//    digitalWrite(m_secondValve, HIGH);
//    digitalWrite(m_thirdValve, LOW);
//}
//
//void controller::openup(void) {
//    digitalWrite(m_firstValve, LOW);
//    digitalWrite(m_secondValve, LOW);
//    digitalWrite(m_thirdValve, LOW);
//}
