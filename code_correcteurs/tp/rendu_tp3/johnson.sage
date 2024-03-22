p=101
d=10

q=p**d

n=100
k=n/2



def rho(R):
	return float((1-1/q)*(1-(sqrt(1-(q)*(1-R)/(q-1)))))
def rho_asymp(R):
	return float(1-sqrt(R))

def bw(R):
	return float(1-sqrt(2*R))

def sudan(R):
	return float(1-sqrt(R)*((sqrt(R+8)-sqrt(R))/2))

def gurusuami_sudan(R):
	return float(1-sqrt(R)*( (sqrt(R+4*d*(d+1))-sqrt(R))/(2*d)))

	
g=plot(rho_asymp, 0, 1, color="purple", legend_label="rho")+plot(bw, 0, 1, color="red", legend_label="Berlekamp-Welch")+plot(sudan, 0, 1, legend_label="Sudan")+plot(gurusuami_sudan, 0, 1, legend_label="Gurusuami-Sudan", color="yellow")


g.save("johnson.png")



