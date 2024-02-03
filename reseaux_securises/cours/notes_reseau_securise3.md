ctf: rootme.org (burp/proxy)

commandes nc : 
	nc -l -p 1234 (l pour listen , sur le port (p) 1234
	nc 192.168.1.1 1234 envoie un paquet

modifier la policy en accept: iptables -F; iptables -P FORWARD ACCEPT
	iptables -L -n -v(verbose) pour avoir les regles par defaut.
	iptables -L -n -t nat pour avoir les regles du nat
	iptables -F -t nat (pour flush la table nat)

(dans man, /pattern pour chercher un pattern puis N/n pour se deplacer)
modifier addresses sources/dest: (man iptables, man iptables-extensions)
	source : iptables -t nat -A POSTROUTING -s <addresse a modif> -j SNAT 
		--to-source <nouvelle addr>
	dest: iptables -t nat -A POSTROUTING -i <iface> -d <addr dest> --dport oldport -j DNAT 
		--to-destination <nouvelle addr>:newport



dhclient -iface pour obtenir une daresse ip

ajout d'états:(dit au routeur d'accepter les paquets avec l'état décrit)
	iptables -A ACCEPT -m(match) state --state RELATED(udp par ex),ESTABLISHED(tcp par ex)


resolution dns, quand on utilise ping www.google.fr faut pouvoir resoudre l'addresse:
	dans /etc/resolv.conf : nameserver 10.0.4.3 (la resolution est demandée a 10.0.4.3)
	host google.com pour resoudre l'ip 
	ping google.com


liste blanche : 
	iptables -P FORWARD DROP.
	iptables -A FORWARD .... -j ACCEPT
	iptables -A FORWARD -j LOG (pour ajouter des logs)

logs accesibles avec journalctl -f

pour delete, -D au lieu de -A(meme ligne)

Proxy: export http_proxy=https://addr:port

dans l'ordre sur le routeur: 
	forward le proxy vers internet avec l'addresse source du routeur.
	autoriser les flux entrants du proxy
	autoriser le client a acceder au prxy sur le port 8080 en tcp
	gestion d'état ESTABLISHED,RELATED

nmap -n -p 0-65535 ip 
