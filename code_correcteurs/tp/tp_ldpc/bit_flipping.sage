import random
from scipy.stats import binom

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

class randLDPC:
	def __init__(self, n,k,w, q=2, Verbose=True):
		self.field.<T>=GF(q)
		self.n=n
		self.k=k
		self.w=w
		self.verbose=Verbose
		self.MatSpace=MatrixSpace(self.field, self.n-self.ki, self.n, sparse=True)

		def genLDPC():
			line=[self.field(1)]*self.w +[self.field(0)]*(self.n-self.w)
			H=[]
			for i in range(self.k):
				random.shuffle(line)
				H.append(line)
			return matrix(self.field, H)

		if self.verbose:
			print(f"Generating LDPC with matrix:\n {H}\n of weight {w}\n")
		self.H=genLDPC()


	def DecodeBitFlipping():
		a=[]

class Canal:
	def __init__(self, q, n, distribution="uniform", Verbose=True):
		self.distribution=distribution
		self.field=GF(q, "T")
		self.n=n
		self.verbose=Verbose

	def go_through(self, c, t):
		nb_err=random.randrange(0, t)
		if self.distribution="uniform":
			e=[self.field.random_element() for i in range(nb_err)]+[0]*(self.n-nb_err)
			random.shuffle(e)
		elif self.distribution="binomial":
			e=binom.rvs(1, 0.5, size=self.n)
		else:
			e=[self.field.random_element() for i in range(nb_err)]+[0]*(self.n-nb_err)
			random.shuffle(e)

		e=vector(self.field, e)

		if self.verbose:
			print(f"Going through the canal, adding error {e}\n")

		return c+e

w=3
CODE=randLDPC(n,k,w)
m=vector(CODE.field, [CODE.field.random_element() for i in range(CODE.k)])

c=CODE.Encode(m)

if args.algo == "bitflipping":









