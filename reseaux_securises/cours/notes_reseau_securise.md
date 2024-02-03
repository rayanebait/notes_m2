tcpdump -enli veth0 not ip6 and icmp (not ip6 pour les sollicitations du routeur, icmp pour les arp)
ou tshark -i veth0 -T text (ip a et ether)

ip neigh show pour voir les voisins
ip netns show

ip neigh flush all 

quand on envoie un paquet faut faire une resolution ARP avant (checker avec un ping et tcpdump)

Pour que l'os passe en mode routeur: sysctl net.ipv4.ip_forward=0 ou 1

switch trunk: Pour envoyer d'un vlan a un autre, faire parler ou attendre qu'une adresse parle, récuper l'id de vlan et ajouter Dot1Q(vlan=id)


dans scapy: 
	-e=Ether(src="mac_addr", dst="mac_addr"), ff:ff:ff:ff:ff:ff for broadcast
	-sendp(e, iface="ether interface"), iface="veth0" in network
	-srp(e, timeout=1)



COURS 2:

ip r sort
default via 172.20.10.1 dev wlp3s0 proto dhcp metric 600 -> 172.20.10.1 est l'adresse du routeur
et dev .. désigne l'interface.

169.254.0.0/16 dev wlp3s0 scope link metric 1000 -> pour les addresses sur ce masque en /16 on parle en couche lien via wlp3s0 (wi fi?)

172.20.10.0/28 dev wlp3s0 proto kernel scope link src 172.20.10.7 metric 600 -> pour les addresses en /28  pareil mais cette fois on doit utiliser l'addresse 172.20.10.7

traceroute -nI

sysctl -a |grep truc (pour afficher les params)
l'os entame des actions indépendamment de ce qu'on fait avec la carte réseau
ex: redirects->sysctl

avantages du dhcp mitm: discret (visible qu'au moment de donner l'ip) (Pas tant d'avantages)
(en fait on peut faire l'attaque a tout moment en disant qu'on est un serveur v6)

