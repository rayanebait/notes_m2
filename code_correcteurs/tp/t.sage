k=3
n=10
m=2
p=11

q=p^m


K.<t>=GF(q)


V=VectorSpace(K, n)
M=MatrixSpace(K,n)

H=M.random_element()

print(H)

H1=matrix([vector(K,[c_i**p-c_i for c_i in c]) for c in H.columns()])

print(H1)
Ker=H1.left_kernel()
Ker.change_ring(GF(p))
print(Ker)
print(Ker.dimension())


