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


