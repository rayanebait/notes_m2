#Etant donné une erreur e tq |e|<=w,
#He=s (syndrome). On connait s on veut e
#tq |e|=w. On regarde sur des ens d'infos


#H est de taille n*(n-k)?(ca a l'air)

from sage.coding.code_bounds import entropy, entropy_inverse
import random

n=20
k=10
p=2
m=1
q=p^m

t_max=floor(n*entropy_inverse(1-k/n,q))

ssets=Subsets([i for i in range(0,n)], n-k)

def Prange(H,s,n,k,w):
	if s==[0]*(n-k):
		print(s)
		return [0]*(n-k)

	count=0
	while True:
		count=count+1
		J=ssets.random_element()
		H_J=H.matrix_from_columns(J)
		#on peut aussi solve_right(s)
		try:
			H_J_inv=H_J^(-1)
		except:
			#if count%1000==0:
				#print(J," n'est pas un ensemble d'info")
				#print("\n", count, "itérations")
			continue
		e_=H_J_inv*s
		if count%1111==0:
			e=[0]*n	
			i=0
			for j in J:
				e[j]=e_[i]
			i=i+1
			print("H^-1*s=",e)
		if e_.hamming_weight() == w:
			break
	e=[0]*n	
	i=0
	for j in J:
		e[j]=e_[i]
		i=i+1
	print(count)
	return e

K.<t>=GF(q)
M=MatrixSpace(K,n-k,n)
H=M.random_element()
print(H,"t_max=", t_max)

w=random.randint(1,t_max)

e=vector(K, [1 for i in range(t_max)]+[0]*(n-t_max))
random.shuffle(e)

e_=Prange(H,H*e,n,k,t_max)

print(e,e_)
