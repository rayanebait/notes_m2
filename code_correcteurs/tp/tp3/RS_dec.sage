n=10
k=5

m=2
p=2
q=p^m

t=floor((n-k)/2)

K.<a>=GF(q)
V=VectorSpace(K,n)

R.<X>=K[]
x=V.random_element()
u=V.random_element()


M=matrix(K,[[u[j]*(x[i]**j) for j in range(t+1)] +[-x[i]**(j) for j in range(n-t)]
		for i in range(n)])

print(M)

KER=M.right_kernel().basis()
#print(type(KER[0][:t+1]))
#Transforme une list en polynome, attention à l'ordre
P=R(list(KER[0]))

print(P)

#Q=R(list(KER[0])[t+1:])
#print(P)
