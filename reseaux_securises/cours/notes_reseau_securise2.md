POur le tp: 
setxkbmap fr
service NetworkManager stop (sinon ca deco tout le temps)
ifconfig eth0 192.168.0.10 (configure une route liée au cable ethernet)
ifconfig eth0 192.168.0.1 (configure une route liée au cable ethernet)
(on peut ping la)


	dans virtual box, et sur la vm dans reseau, on peut configurer ce qu'y faut
	
coté client: ajouter ip du routeur (eth0 192.168.0.1/29)
coté routeur: ajouter ip des autres routeurs (forward aussi)

en fait y'a une interface locale (192.168.0.n'/29)et une interface de la classe (10.0.0...)

client ifconfig eth0 192.168.0.n/29
routeur ifconfig eth0 192.168.0.n/29
	ifconfig eth1 10.0.0.n'/24
	(je sais pas si ca c'est utile) route add -net 192.168.0.n/29 gw 10.0.0.n'
	route add default gw 192.168.0.158 (ou 158 est le routeur)

le n est calculé par le /29, le n' est donné dans la classe. (On prend n comme la premiere addr du sous reseau + 1)

iptables: 
	-F pour flush toute les regles
	-P (policy, ex:) FORWARD DROP -> drop tout les paquets 
	-P FORWARD -s 192.168.0.153 -d 192.168.1.153 -j ACCEPT (autorise .0 vers .1)
	-P FORWARD -s 192.168.1.153 -d 192.168.0.153 -j ACCEPT (autorise les comms de .1 vers .0)

	-m pour ajouter un protocole et -p tcp --port 1234 pour ajouter un port

(nmap pour cartographier)

ressources:
	attack.mitre.org
	snort et zeek(ressemble a wireshark a la source, capture des paquets entiers) = outils de detection
	netflow (garde que des ips ports)
