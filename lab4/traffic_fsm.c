/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 30, 2025
 * Implementation of the traffic light FSM
 */

#include "LED.h"
#include "traffic_fsm.h"

void tick_traffic(State *state, unsigned short pwr, unsigned short ped,
                  struct LED red, struct LED ylw, struct LED grn) {
  // Transitions
  switch (*state) {
    case Stop:
      off(red);
      if (pwr) {
        *state = Off;
      } else if (ped) {
        *state = Stop;
      } else {
        *state = Go;
      }
      break;

    case Warn:
      off(ylw);
      if (pwr) {
        *state = Off;
      } else {
        *state = Stop;
      }
      break;

    case Go:
      off(grn);
      if (pwr) {
        *state = Off;
      } else if (ped) {
        *state = Warn;
      } else {
        *state = Stop;
      }   
      break;
    
    case Off:
      if (pwr) {
        *state = Stop;
      }   
      break;
  }
  
  // State actions
  switch (*state) {
    case Stop:
      on(red);
      break;
    
    case Warn:
      on(ylw);
      break;
    
    case Go: 
      on(grn);
      break;
    
    case Off:
      break;
  }
}
