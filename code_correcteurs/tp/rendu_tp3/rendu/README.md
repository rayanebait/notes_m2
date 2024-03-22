Le code contient deux classes: 
	-RandRS qui génère un ReedSolomon aléatoire
	-Canal qui représente un canal à poids uniforme

Lors du lancement avec sage ReedSolomon.sage le code génère un RandRS ainsi qu'un (ou des)
message aléatoires, encode le message, le fait passer à travers un canal et lance le déco
-dage avec le décodeur spécifié en arguments. Les arguments sont les suivants:
	-Pour la longueur et la dimension du code: -n 10 -k 3 (par défaut -n 20 -k 5)
	-Pour la caractéristique et la dimension du corps: -p 2 -d 5 (par défaut -p 997 -d 1)
	-Pour l'algorithme de décodage: -a "bw" ou "sudan" ou "gs"
	-Pour la multiplicité dans le cas Guruswami-Sudan: -m (par défaut 2)
	-Pour le nombre de tests: -i 3 (par défaut 1)
	-Pour la verbosité -v

Les algorithmes de décodages sont spécifiés pour décoder à
	(n-k)/2 (Berlekamp-Welch)
	n*(1-sqrt(R)*( sqrt(R+8)-sqrt(R) )/2 (Sudan)
	n*(1-sqrt(R)) (Guruswami-Sudan)

L'algorithme de Sudan simple peut être réalisé en appelant -a "gs" -m 1.  

Exemples:

sage ReedSolomon.sage -v

Generating Reed Solomon RS_5((320, 755, 331, 816, 559, 608, 470, 234, 664, 351, 763, 465, 338, 687, 326, 792, 551, 97, 661, 697)) over Finite Field of size 997 with matrix:
[  1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1]
[320 755 331 816 559 608 470 234 664 351 763 465 338 687 326 792 551  97 661 697]
[706 738 888 857 420 774 563 918 222 570 918 873 586 388 594 151 513 436 235 270]
[598 864 810 415 485   8 405 457 849 670 540 166 662 357 226 949 512 418 800 754]
[933 282 914 657 928 876 920 259 431 875 259 421 428 994 895 867 958 666 390 119]
Decoding with Berlekamp-Welch decoder

Encoding message (319, 29, 320, 676, 194) into (238, 843, 20, 345, 802, 309, 313, 29, 403, 22, 691, 518, 375, 310, 840, 978, 564, 92, 287, 645)

Going through the canal, adding error (61, 0, 0, 0, 735, 0, 0, 0, 0, 0, 392, 0, 995, 0, 0, 0, 859, 0, 0, 491)

Decoding encoded message (299, 843, 20, 345, 540, 309, 313, 29, 403, 22, 86, 518, 373, 310, 840, 978, 426, 92, 287, 139)

Basis of solutions for Q_0(x_i)+u_i*Q_1(x_i)=0: Vector space of degree 21 and dimension 2 over Finite Field of size 997
Basis matrix:
[  1   0 764 917 112 863 697 124 190  15 129 458   0  25 179 922 534 135 112  82 491]
[  0   1 486 671   7 523 806 592 700 413 336 914   0   0  25 365 846 926 524 541 797]

Q_0=458*X^11 + 129*X^10 + 15*X^9 + 190*X^8 + 124*X^7 + 697*X^6 + 863*X^5 + 112*X^4 + 917*X^3 + 764*X^2 + 1, Q_1=491*X^7 + 82*X^6 + 112*X^5 + 135*X^4 + 534*X^3 + 922*X^2 + 179*X + 25

Original message: (319, 29, 320, 676, 194)
Decoded message: (319, 29, 320, 676, 194)






Sur 5 itérations de Sudan En rendement 1/3, 5 réussites:
(appel du décodeur de Guruswami-Sudan pour m=1)

sage ReedSolomon.sage -a "gs" -m 1 -n 15 -k 5 -p 2 -d 6

Original message: (T^5 + T^4 + T^3 + T^2 + T + 1, T^3 + T^2, T^3 + T^2, T^5 + T^4 + T + 1, T^3 + T + 1)
Found the following candidate messages: 

[(T^5 + T^4 + T^3 + T^2 + T + 1, T^3 + T^2, T^3 + T^2, T^5 + T^4 + T + 1, T^3 + T + 1), (T^2 + T + 1, T^5 + T + 1, T^4 + T^2 + 1, T^4 + T^2 + T, T^5 + T^3 + 1)]

Original message: (T^5 + T^4 + T, T^4 + T^3 + T^2, T^5 + T^3 + T^2 + T, T^4 + 1, T^5 + T^2 + T)
Found the following candidate messages: 

[(T^5 + T^2 + 1, T^5 + T^3 + T + 1, T^4 + T^3 + T^2, T^3 + T^2 + T + 1, T^4 + T^3 + T^2 + 1), (T^5 + T^4 + T, T^4 + T^3 + T^2, T^5 + T^3 + T^2 + T, T^4 + 1, T^5 + T^2 + T)]

Original message: (T^5 + T^4 + T^3 + 1, T^5 + T^4 + T + 1, T^4 + T^3 + T^2 + T + 1, T^5 + T^4 + T^2 + 1, T^5 + T^3 + T^2 + T)
Found the following candidate messages: 

[(T^5 + T^4 + T^3 + 1, T^5 + T^4 + T + 1, T^4 + T^3 + T^2 + T + 1, T^5 + T^4 + T^2 + 1, T^5 + T^3 + T^2 + T), (T^4 + 1, T^5 + T^3, T^5, T^2 + T + 1, T^5 + T^4 + T^3 + T^2 + 1)]

Original message: (T^3 + T + 1, T^4, T^3 + 1, T^3 + T^2 + T + 1, T^4 + T + 1)
Found the following candidate messages: 

[(T^5 + T^2 + T + 1, T^3 + T + 1, T^4 + T^2 + T + 1, T^5 + T^3 + T^2 + T + 1, T^3 + T^2 + 1), (T^3 + T + 1, T^4, T^3 + 1, T^3 + T^2 + T + 1, T^4 + T + 1)]

Original message: (T^4 + T^3 + T^2 + T + 1, T^4 + T^2, T^4 + T^2 + T + 1, T^3 + T^2 + 1, T^4 + T^2 + 1)
Found the following candidate messages: 

[(T^5 + T^4 + T + 1, T^3 + T^2 + T + 1, T^5 + T^4 + T^2 + T, T^5 + T^2 + 1, T + 1), (T^4 + T^3 + T^2 + T + 1, T^4 + T^2, T^4 + T^2 + T + 1, T^3 + T^2 + 1, T^4 + T^2 + 1)]










Sur 5 iterations de Guruswami-Sudan en rendement 2/3 et multiplicité 2, 4 réussites.
sage ReedSolomon.sage -a "gs" -m 2 -n 15 -k 10 -i 5 -p 2 -d 6

Original message: (T^5 + T^3 + 1, T^4 + T^3 + 1, T^5 + T + 1, T^2 + T + 1, T^5 + T^4 + T^2 + T + 1, T^2 + 1, T^4 + T^3 + 1, T^4 + T + 1, T^4 + T + 1, T^5)
Found the following candidate messages: 

[(T^5 + T^3 + 1, T^4 + T^3 + 1, T^5 + T + 1, T^2 + T + 1, T^5 + T^4 + T^2 + T + 1, T^2 + 1, T^4 + T^3 + 1, T^4 + T + 1, T^4 + T + 1, T^5)]

Original message: (T^5 + T^4, T^3 + T^2 + T, T^5 + T^2 + 1, T^4 + T^3 + T + 1, T^5 + T^4 + T^3 + T, T, T^5 + T^2, T^5 + T^4, T^2 + T, T)
Found the following candidate messages: 

[]

Original message: (T, T^2 + 1, T^4 + T^3 + T^2, T^5 + T^4 + T^2 + 1, T^5 + T^4 + T^3 + T + 1, T^4 + T, T^4, T^4 + T^3 + T^2 + T, T^4 + T^3 + T^2 + 1, T^5 + T^4 + T^3 + 1)
Found the following candidate messages: 

[(T, T^2 + 1, T^4 + T^3 + T^2, T^5 + T^4 + T^2 + 1, T^5 + T^4 + T^3 + T + 1, T^4 + T, T^4, T^4 + T^3 + T^2 + T, T^4 + T^3 + T^2 + 1, T^5 + T^4 + T^3 + 1)]

Original message: (T^4 + T^2, T^5 + T^4 + T^2 + 1, T^4 + T^3 + T^2 + T + 1, T^5 + T^4 + T^3, T^5 + T^4 + T^3, T^5 + T^4 + T^2 + T + 1, T^5 + T^4 + T^3 + T + 1, T^5 + T^4 + T + 1, T^5 + T^4 + T^3 + T^2 + 1, T^5 + 1)
Found the following candidate messages: 

[(T^4 + T^2, T^5 + T^4 + T^2 + 1, T^4 + T^3 + T^2 + T + 1, T^5 + T^4 + T^3, T^5 + T^4 + T^3, T^5 + T^4 + T^2 + T + 1, T^5 + T^4 + T^3 + T + 1, T^5 + T^4 + T + 1, T^5 + T^4 + T^3 + T^2 + 1, T^5 + 1)]

Original message: (T^5 + T^2, T^4 + T, T^5 + T^4 + 1, T^5 + T^3 + T^2 + 1, T^3 + T^2, T^4 + T + 1, T^5, T^5 + T^4 + T^2 + T, T^5 + T^4 + 1, T^2 + 1)
Found the following candidate messages: 

[(T^5 + T^2, T^4 + T, T^5 + T^4 + 1, T^5 + T^3 + T^2 + 1, T^3 + T^2, T^4 + T + 1, T^5, T^5 + T^4 + T^2 + T, T^5 + T^4 + 1, T^2 + 1)]

