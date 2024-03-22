import random
from argparse import ArgumentParser
parser=ArgumentParser()

parser.add_argument("-v", "--verbose",action="store_true")
parser.add_argument("-n", "--length", default="20")
parser.add_argument("-k", "--dimension", default="5")
parser.add_argument("-p", "--characteristic", default="997")
parser.add_argument("-d", "--fielddimension", default="1")
parser.add_argument("-a", "--algo", default="bw")

args=parser.parse_args()

p=Integer(args.characteristic)
d=Integer(args.fielddimension)

n=Integer(args.length)
k=Integer(args.dimension)




class RandRS:
	def __init__(self, p, d, n, k, Verbose=True):
		self.p=p
		self.d=d
		self.n=n
		self.k=k
		self.field=GF((p,d), "T")
		self.x=vector(self.field, [self.field.random_element() for i in range(n)])

		self.M=matrix(self.field, [[self.x[i]**j for i in range(n)] for j in range(k)])
		self.verbose=Verbose

		if self.verbose:
			print(f"Generating Reed Solomon RS_{k}({self.x}) over {self.field} with matrix:")
			print(f"{self.M}")
	
	def Encode(self, m):
		c=vector(self.field, m * self.M)
		if self.verbose:
			print(f"Encoding message {m} into {c}\n")
		return c

		

	def DecodeBerlekampWelch(self, u):
		if self.verbose:
			print(f"Decoding encoded message {u}\n")
		#n+1=n-t+1+t
		variables=PolynomialRing(self.field, self.n+1, "a")
		R.<X>=self.field[]

		#les coeffs sont des polynomes en les variables
		L.<X,Y>=variables[]

		Q=sum([L(variables.gen(i)*X^i) for i in range(n-t)])
		Q+=L(Y)*sum([L(variables.gen(n-t+i)*X^i) for i in range(t+1)])
	
		#matrice dont les lignes sont les Q(x_i, u_i)
		M=matrix(self.field, [[Q(self.x[i], u[i]).coefficient(variables.gen(j)) for j in range(n+1)] for i in range(n)])

		SOLS=M.right_kernel()

		#contient les coeffs d'un polynome de la base
		#Ordonné de haut en bas comme coeffs a_i de Q_0 puis a_i-(n-t) de u_i*Q_1 
		if self.verbose:
			print(f"Basis of solutions for Q_0(x_i)+u_i*Q_1(x_i)=0: {SOLS}\n")
		for coeffs in SOLS.basis():
			Q_0=R(list(coeffs[:n-t]))
			Q_1=R(list(coeffs[n-t:]))
			if self.verbose:
				print(f"Q_0={Q_0}, Q_1={Q_1}\n")
			L.<Y>=R[]
			Q=Q_0+Y*Q_1

			#R(-Q_0/Q_1)
			#return vector(self.field, Q.roots()[0].coefficients())
			try:
				return vector(self.field, list(R(-Q_0/Q_1)))
			except:
				continue
	def DecodeSudan(self, u, r, l):
		if self.verbose:
			print(f"Decoding encoded message {u}\n")
		nb_vars=(l+1)*(self.n-r)-(self.k-1)*((l*(l+1))//2)

		variables=PolynomialRing(self.field, nb_vars, "a")

		#les coeffs sont des polynomes en les variables
		L.<T,T_>=variables[]

		Q=L(0)
		last_ind=0
		for j in range(l+1):
			degQj=self.n-r-j*(self.k-1)-1
			Q+=L(T_^j)*sum([L(variables.gen(last_ind+i)*T^i) for i in range(degQj+1)])
			last_ind+=degQj

	
		#matrice dont les lignes sont les Q(x_i, u_i)
		M=matrix(self.field, [[Q(self.x[i], u[i]).coefficient(variables.gen(j)) for j in range(nb_vars)] for i in range(n)])
		
		SOLS=M.right_kernel()

		R.<X>=self.field[]
		O.<Y>=R[]

		if self.verbose:
			print(f"Basis of solutions for Q(x_i,u_i)=0: {SOLS}\n")

		for coeffs in SOLS.basis():
			Pjs=[]
			lowind=0
			highind=0
			for j in range(l+1):
				highind+=self.n-r-j*(self.k-1)
				Pjs.append(R(list(coeffs[lowind:highind])))
				lowind=highind
			P=sum([Pjs[j]*Y^j for j in range(l+1)])
			print(P(X^(k-1)).degree(), self.n-r)

			if self.verbose:
				print(f"Searching for roots of {P} in {P.parent()}")
			try:
				roots=P.roots()
				return roots
			except:
				continue
		return 

	def DecodeSudanCorr(self, u, r, l):
		ncols=(l+1)*(self.n-1)-((self.k-1)*(l+1)*l)//2
		M_tab=[[0]*ncols for i in range(self.n)]

		for i in range(n):
			index=0
			for j in range(l+1):
				for s in range(self.n-r-j*(self.k-1)):
					M_tab[i][index]=u[i]^j*self.x[i]^s
					index+=1
		R.<X>=self.field[]
		L.<Y>=R[]

		M=matrix(self.field, M_tab)
		S=M.right_kernel()
		coeffs = S.basis()[0]

		Q_j=[0]*(l+1)
		Q=0*X*Y
		index_min=0
		index_max=self.n-r
		for j in range(l+1):
			Q_j[j]=R(list(coeffs[index_min:index_max]))
			index_min=index_max
			index_max+=self.n-r-j*(self.k-1)
			Q+=Q_j[j]*Y^j
		if self.verbose:
			print(f"\nSearching for roots of {Q} in {Q.parent()}\n")

		return Q.roots()
		
	def DecodeGuruswamiSudan(self, y, r, l, m):
		"""
		inspiré de 

https://github.com/sagemath/sage/blob/develop/src/sage/coding/guruswami_sudan/interpolation.py

		et 

https://en.wikipedia.org/wiki/Guruswami%E2%80%93Sudan_list_decoding_algorithm

		Tout polynome Q(X,Y) de degré m lesté par (1,k-1) s'écrit :

			Q(X,Y)=\sum_{i+(k-1)*j}<=m a_i,j X^iY^j

		On peut ensuite écrire Q(X+a,Y+b)=\sum X^i*Y^jQ^(i,j)(a, b)
		avec 

			Q^(i,j)(X,Y)=\sum_k,l  a_i,j*binom(i,k)*binom(j,l)*X^(i-k)*Y^(j-l)

		On cherche des Q_(i,j) tel que Q a multiplicité m>= en (a, b)
		On traduit que Q a multiplicité m>= en (a,b) par Q_(i,j)(a,b)=0
		pour tout 0 <= i+j <= m-1. 
		"""
		#ncols=(l+1)*(m*self.n-1)-((self.k-1)*(l+1)*l)//2
		#neqs=self.n*(m*(m+1))/2
		def monomials(l, m, k, n, r):
			for j in range(m*(n-r)+1):
				for i in range(m*(n-r)-j*(k-1)):
					yield (i, j)

		lst_monomials=list(monomials(l,m,self.k,self.n,r))

		M_tab=[]
		for (a,b) in zip(self.x, y):
			line=[]
			for u in range(m):
				for v in range(m-u):
					part_line={}
					for monomial in lst_monomials:
						(i, j)=monomial
						if i>=u and j>=v:
							if i>u:
								coeff_i=binomial(i, u) * a**(i-u)
							else:
								coeff_i=1
							if j>v:
								coeff_j=binomial(j, v) * b**(j-v)
							else:
								coeff_j=1
						part_line[monomial]=coeff_i*coeff_j
					for monomial in lst_monomials:
						line.append(part_line.get(monomial, 0))
			M_tab.append(line)


		R.<X>=self.field[]
		L.<Y>=R[]

		M=matrix(self.field, M_tab)
		print(f"Equations to solve: {M}\n")
		S=M.right_kernel()
		print(f"Kernel of the system: {S}\n")
		coeffs = S.basis()[0]


		Q=sum([X^(lst_monomials[i][0]) *Y^(lst_monomials[i][1]) * coeffs[i] for i in range(0, len(lst_monomials))])


		if self.verbose:
			print(f"\nSearching for roots of {Q} in {Q.parent()}\n")
		return Q.roots()
		


class Canal:
	def __init__(self, q, n, distribution="uniform", Verbose=True):
		self.distribution=distribution
		self.field=GF(q, "T")
		self.n=n
		self.verbose=Verbose
	def go_through(self, c, t):
		nb_err=random.randrange(0, t)
		e=[self.field.random_element() for i in range(nb_err)]+[0]*(self.n-nb_err)
		random.shuffle(e)
		e=vector(self.field, e)

		if self.verbose:
			print(f"Going through the canal, adding error {e}\n")

		return c+e

		

RS=RandRS(p,d,n,k, Verbose=args.verbose)
C=Canal(p**d, n, Verbose=args.verbose)

m=vector(RS.field, [RS.field.random_element() for i in range(RS.k)])
c=RS.Encode(m)

if args.algo == "bw":
	t=floor((RS.n-RS.k)/2)
	decoded=RS.DecodeBerlekampWelch(C.go_through(c, t))

	print(f"Original message: {m}")
	print(f"Decoded message: {decoded}")
elif args.algo == "sudan":
	r=floor(RS.n-(RS.n)*sqrt(RS.k/RS.n)*( sqrt(k/n + 8) - sqrt(k/n) )/2)
	l=ceil((n-r)/(k-1)-1)

	m_=RS.DecodeSudan(C.go_through(c, r), r, l)

	print(f"Original message: {m}")
	print(f"Found the following candidate messages: \n\n{m_}")
elif args.algo == "sudancorr":
	r=floor(RS.n-(RS.n)*sqrt(RS.k/RS.n)*( sqrt(k/n + 8) - sqrt(k/n) )/2)
	l=ceil((n-r)/(k-1)-1)

	m_=RS.DecodeSudanCorr(C.go_through(c, r), r, l)

	print(f"Original message: {m}")
	print(f"Found the following candidate messages: \n\n{m_}")
elif args.algo == "gs":
	r=floor( RS.n*(1-sqrt(k/n)) )

	mult=1
	l=floor(mult*(n-r)/(k-1)-1)

	m_=[tuple(root[0]) for root in RS.DecodeGuruswamiSudan(C.go_through(c,r),r,l,mult)]
	print(f"Original message: {m}")

	print(f"Found the following candidate messages: \n\n{m_}")








