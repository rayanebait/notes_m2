import argparse
from scapy.all import *
import signal

BEACON=8
PROBE_REQUEST=4
PROBE_RESPONSE=5
DATA=32

def print_infos():
    # function executed at program exit (end of pcap or Ctrl+C)
    # TODO
    global APS, infos
    print(APS, infos)
    print("beacons:", infos[BEACON])
    print("probe requests:", infos[PROBE_REQUEST])
    print("probe responses:", infos[PROBE_RESPONSE])
    print("data:", infos[DATA])
    pass

def signal_handler(signum, frame):
    print_infos()
    if signum == signal.SIGINT:
        sys.exit(0)

signal.signal(signal.SIGUSR1, signal_handler)
signal.signal(signal.SIGINT, signal_handler)


def process_packet(pckt):
    # function executed for each packet
    # TODO
    global APS, infos, relations, ssids
    #concat type and subtype
    dst_to_src=(pckt.addr1, pckt.addr2)
    if dst_to_src in relations:
        relations[dst_to_src]+=1
    else:
        relations[dst_to_src]=1
    try:
    	info=pckt.info
	ssids[info]=info
    except:
    	pass

    try:
        t_st=(pckt.type<<4)+pckt.subtype
    except:
        return
    infos[t_st]+=1

    if pckt.addr4 not in APS:
	#check transmitter address
        APS[pckt.addr4]=pckt.addr4
        print("New AP: ",pckt.addr4)
    pass

def process_ethernet_packet(pkt):
    try:
        #if pkt.type != 0x9999:
            #return
        decapped = RadioTap(pkt.load)
        process_packet(decapped)
    except:
        pass

parser = argparse.ArgumentParser()
parser.add_argument("-i", "--interface", type=str, help="Wireless interface on which to listen")
parser.add_argument("-e", "--ethernet", type=str, help="Ethernet interface on which to listen")
parser.add_argument("-r", "--reload", type=str, help="File from which to reload data")

args = parser.parse_args()

infos={}
APS={}
relations={}
for i in range(64):
    infos[i]=0

# Capture from wireless interface
if args.interface is not None:
    timer=time.time()
    while True:
        pckts = sniff(count=10,
            prn=process_packet, 
            iface=args.interface)

        for pckt in pckts:
            process_packet(pckt)
            if time.time()-timer > 3:
                timer=time.time()
                print_infos()

# read from Ethernet interface
elif args.ethernet is not None:
    pckts = sniff(count=0,
            prn=process_ethernet_packet, 
            iface=args.ethernet)

# load from .pcap file
elif args.reload is not None:

    timer=time.time()
    for pckt in PcapReader(args.reload):
        process_packet(pckt)
        if time.time()-timer > 3:
            timer=time.time()
            print_infos()

else:
    raise Exception("No source specified")
