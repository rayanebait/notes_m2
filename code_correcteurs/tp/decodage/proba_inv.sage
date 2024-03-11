q=2
n=100000
k=n

a=[1-(float(1/q**(n-i))) for i in range(k)]

p=product(a)

print(float(p))
