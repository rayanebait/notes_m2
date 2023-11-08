package main

import "fmt"

func main() {
	primes := make([]bool, 1000)

	for i := range primes {
		primes[i] = true
	}

	for i := 2; i*i < 1000; i++ {
		if primes[i] {
			for j := i * 2; j < 1000; j = j + i {
				primes[j] = false
			}
		}
	}

	for i := 2; i < 1000; i = i + 1 {
		if primes[i] {
			fmt.Printf("%d ", i)
		}
	}

	fmt.Printf("\n")

}
