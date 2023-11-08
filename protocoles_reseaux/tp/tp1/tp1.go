package main

import (
	"fmt"
	"log"
	"net/http"
	"strconv"
)

func main() {
	http.HandleFunc("/hello.text", helloText)
	http.HandleFunc("/hello.html", helloHtml)
	http.HandleFunc("/name-get", nameGet)
	http.HandleFunc("/name-post", namePost)
	http.HandleFunc("/name-request", nameRequest)
	http.HandleFunc("/num-get", numGet)
	http.HandleFunc("/primes-request", primesRequest)

	err := http.ListenAndServe(":8080", nil)
	log.Fatal("ListenAndServe: ", err)
}

func helloText(w http.ResponseWriter, r *http.Request) {
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method no allowed", http.StatusMethodNotAllowed)
		return
	}
	w.Header().Set("Content-Type", "text/plain; charset=utf-8")
	fmt.Fprintln(w, "Hello!")
}

func helloHtml(w http.ResponseWriter, r *http.Request) {
	const data = `<!DOCTYPE html>
<html>
<head></head>
<body>
<p>Hello2!</p>
</body>
</html>`
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)
}

func nameGet(w http.ResponseWriter, r *http.Request) {
	const data = `<!DOCTYPE html>
<html>
<head></head>
<body>
<form action="/name-request" method="get">
Votre nom: <input type="text" name="name"/> <input type="submit"/></form>
</body>
</html>`
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)
}

func namePost(w http.ResponseWriter, r *http.Request) {
	const data = `<!DOCTYPE html>
<html>
<head></head>
<body>
<form action="/request-name-post" method="post">
Votre nom: <input type="text" name="name"/> <input type="submit"/></form>
</body>
</html>`
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)
}

func nameRequest(w http.ResponseWriter, r *http.Request) {
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}

	r.ParseForm()

	data := `<!DOCTYPE html>
<html>
<head></head>
<body>
<p> Votre nom est dkzzdk:` + r.Form.Get("name") + `</p> 
</body>
</html>`

	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)

}

func numGet(w http.ResponseWriter, r *http.Request) {
	const data = `<!DOCTYPE html>
<html>
<head></head>
<body>
<form action="/num-request" method="get">
Entrez un nombre: <input type="number" name="num"/> <input type="submit"/></form>
</body>
</html>`
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)

}
func primesRequest(w http.ResponseWriter, r *http.Request) {
	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}

	r.ParseForm()

	data := `<!DOCTYPE html>
<html>
<head></head>
<body>
<p> Liste des premiers plus petits que:` + r.Form.Get("num")

	a, _ := strconv.ParseInt(r.Form.Get("num"), 10, 32)
	primes := make([]bool, a)

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

	for i, n := range primes {
		if n {
			if i%30 == 0 {
				data += `\n`
			}
			data += strconv.FormatInt(int64(i), 10)
		}
	}

	data += `</p>
	</body>
	</hmtl>`

	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)
}
