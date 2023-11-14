package main

import (
	"fmt"
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/", WP)

	err := http.ListenAndServeTLS(":8888", "cert.pem", "key.pem", nil)
	log.Fatal("ListenAndServeTLS: ", err)
}

func WP(w http.ResponseWriter, r *http.Request) {

	auth := r.Header.Get("Authorization")

	usrname, pw, _ := r.BasicAuth()
	fmt.Println(usrname, pw)

	if usrname != "einstein" && pw != "elsa" {
		return
	}
	if auth == "" {
		w.Header().Set("www-authenticate", "basic realm=\"tp5\"")
		http.Error(w, "Haha!", http.StatusUnauthorized)
		return
	}
	log.Println("Authorization:", auth)

	if r.Method != "HEAD" && r.Method != "GET" {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	data := `<!DOCTYPE html>
<html>
<head>Est-ce qust encrypté ?</head>
<body>
<p> Un peu de texte </p> 
</body>
</html>`

	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprintf(w, data)
}
