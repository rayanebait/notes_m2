package main

import (
	"crypto/tls"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"
)

type jsonMessage struct {
	Id   string `json:"id"`
	Time int64  `json:"time"`
	Body string `json:"body"`
}

func main() {
	transport := &*http.DefaultTransport.(*http.Transport)
	transport.TLSClientConfig = &tls.Config{InsecureSkipVerify: true}
	client := &http.Client{
		Transport: transport,
		Timeout:   50 * time.Second,
	}

	/*resp, err := client.Get("https://jch.irif.fr:8082/chat/")
	for err != nil {
		//tester erreur
		time.Sleep(100000000)
		resp, err = client.Get("https://jch.irif.fr:8082/chat/")
	}*/

	// jsonReader, err := io.ReadAll(respj.Body)
	// if err != nil {
	// 	log.Fatal("ReadAll:", err)
	// }

	/*On traduit les octets en string utf8*/
	// jsonStream := string(jsonReader)

	// fmt.Println(jsonStream)

	/*Pour utiliser tout ca faut virer les tokens de la str jsonStream*/

	/*t, err = dec.Token()*/
	// for dec.More() {
	// 	var m []jsonMessage
	// 	// decode an array value (Message)
	// 	err := dec.Decode(&m)
	// 	if err != nil {
	// 		log.Fatal("decode\n", err)
	// 	}

	// 	fmt.Printf("%s: %s: %d\n", m.Id, m.Body, m.Time)
	// }
	req, err := http.NewRequest("GET", "https://jch.irif.fr:8082/chat/messages.json?count=5", nil)
	if err != nil {
		log.Fatal("newrequest", err)
	}

	etag1 := ""
	for true {
		respj, err := client.Do(req)
		for err != nil {
			/*tester erreur*/
			time.Sleep(100000000)
			respj, err = client.Get("https://jch.irif.fr:8082/chat/messages.json?count=4")
		}

		etag := respj.Header.Get("Etag")

		req.Header.Add("If-None-Match", etag)

		fmt.Println(etag)

		if etag != etag1 {
			dec := json.NewDecoder(respj.Body)
			var m []jsonMessage

			_ = dec.Decode(&m)

			for _, jM := range m {
				fmt.Println(jM.Body)
			}
		}

		etag1 = etag

		time.Sleep(5000000000)
		respj.Body.Close()
	}

}
