#!/usr/bin/env python3

import argparse
import sys

COMMANDS = {
    "DDR": 0,
    "PORT": 1
}

PORTS = {
    "B": 0,
    "C": 1,
    "D": 2
}

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("command", choices=["DDR", "PORT"])
    parser.add_argument("port", choices=["B", "C", "D"])
    parser.add_argument("bit", choices=list(range(8)), type=int)
    parser.add_argument("value", choices=[0,1], type=int)
    args = parser.parse_args()

    command = (COMMANDS[args.command] << 6) | (PORTS[args.port] << 4) | (args.bit << 1) | args.value;
    print("%02x" % command)
    print("%c" % chr(command))
    return 0

if __name__ == "__main__":
    sys.exit(main())
