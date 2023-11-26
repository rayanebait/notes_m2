#!/usr/bin/env python3
import sys

"""

       1
     ┌────┐
     │    ▼
    ┌┴─────────┐             ┌─────────┐  ┌─────────┐       ┌─────────┐
    │          │      0      │         │d0│         │d1   d6│         │
    │   idle   ├────────────►│  data0  ├─►│  data1  ├─►...─►│  data7  │
    │          │             │         │  │         │       │         │
    └──────────┘             └─────────┘  └─────────┘       └────┬────┘
         ▲                                                       │
         │                                                       │
         │                                                       │
         │                                                       │
         │                    ┌──────────┐                       │
         │   1                │          │              d7       │
         └────────────────────┤   stop   │◄──────────────────────┘
                              │          │
                              └─────┬────┘
                                    │
                                    ▼
                                  Error
"""

def handle_idle(status, c):
    if c == '0':
        # This is a start bit
        # Set bit_count to 0 (we don't have read data bits yet)
        status["bit_count"] = 0
        # Reset data
        status["data"] = 0
        status["state"] = "data"
    else:
        status["state"] = "idle"

def handle_data(status, c):
    # data bit <bit_count> has been received
    # Update data and shifting the received bit in the right position
    status["data"] += int(c) << status["bit_count"]
    # Increment the number of data bits received
    status["bit_count"] += 1
    if status["bit_count"] == 8:
        # We have read 8 data bits, we are now waiting for a stop bit
        status["state"] = "stop"
    else:
        status["state"] = "data"

def handle_stop(status, c):
    if c != '1':
        raise ValueError("Stop bit is invalid")
    # We read a stop bit, everything is OK, let's go back to idle
    status["state"] = "idle"


# Defines the available states of the state machine
# Each state has a name, and a "transition function" that will be called
# with the current status, the next character read, and will edit
# the current status.
STATES = {
    "idle": handle_idle,
    "data": handle_data,
    "stop": handle_stop
}


# Decodes the UART frame and prints its data
def parse(string):
    # Contains the current status of the decoder
    # "state" contains the name of the current state (in the state machine)
    # bit_count contains the number of data bits we have already read
    #   in the current UART frame
    # data contains the current value read from the UART frame.
    status = {
        "state": "idle",
        "bit_count": 0,
        "data": 0
    }

    for char in string:
        start_state = status["state"]
        # STATES[start_state] contains the "transition function" of the
        # current state
        transition_function = STATES[start_state]
        transition_function(status, char)
        # if we started from "stop" and no exception happenned, it means
        # that we succesfully read a byte
        if start_state == "stop":
            print(chr(status["data"]), end="")


parse(sys.argv[1])
