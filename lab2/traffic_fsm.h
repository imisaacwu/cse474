/**
 * Richie Doan, Isaac Wu
 * 2169931, 2360957
 * Apr. 30, 2025
 * This file is just to modularize the FSM logic for our
 * traffic light controller.
 */

// The set of possible states for the traffic light system
enum States { Off, Stop, Warn, Go};

/** Updates the traffic light state based on the given inputs
 *
 * @param state a reference to the current state
 * @param pwr the state of the power button input
 * @param ped the state of the pedestrian button input
 * @param red the red LED output
 * @param ylw the yellow LED output
 * @param grn the green LED output
 */
void tick_traffic(State &state, unsigned short pwr, unsigned short ped
                  struct LED red, struct LED ylw, struct LED grn);
