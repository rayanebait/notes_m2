q=2
n=20
k=10

t=distance de decodage<= n*entropy.inverse(1-k/n, q)


Pour 1 instance:
	H=MatrixSpace(GF(2), n-k, n).random_element()
	e_tab=[randrange(1,q) for i in range(t)].append([0]*(n-t))
	random.shuffle(e_tab)
	s=H*vector(GF(q), e_tab)

Complexité de Prange=Nb_iter(pivot de gauss negl)
		    =1/P_succ(iter)
		    =binom(n-t, k)/binom(n, k)


Retenu: En gros la proba d'itération doit prendre en compte la proba 
d'inverser+ la proba d'etre de poids != t ? oui, i.e. P_succes

la c'est dans le calc d'itération, la complexité: 
	Cprange=binom(n, k)/binom(n-t, k)=2^(nh_2(k/n)-(n-t)h_2(k/n-t))
					 =2^(n( h_2(R)-(1-t)h_2(R/(1-t) )))

dépend grandement de t et de gibert varshamov, nb exponentiel de sols au dessus.
au dessus de gibert varshamov. Nb de sols= 1+binom(n, t)*1/q^n-k (proba d'etre de poids t* proba
d'avoir le bon syndrome, = 2^(n*max(0, h_q(t/n)-1+R)))

En fait si on ajoute le nb de sols: P_succ= min(1, Nsol*Psucc)


