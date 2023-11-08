package main

import (
	"crypto/tls"
	"fmt"
	"io"
	"net/http"
	"strings"
	"time"
)

func main() {
	transport := &*http.DefaultTransport.(*http.Transport)
	transport.TLSClientConfig = &tls.Config{InsecureSkipVerify: true}
	client := &http.Client{
		Transport: transport,
		Timeout:   50 * time.Second,
	}

	for {
		time.Sleep(50)
		/*La on récupère comme avec Curl les ids du chat*/
		resp, err := client.Get("https://jch.irif.fr:8082/chat/")
		if err != nil {
			fmt.Println("error: client.Get")
			return
		}

		/*Faut créer une requete ca marche pas du tout ca */

		//On choppe en octets
		idsReaderBytes, err := io.ReadAll(resp.Body)
		if err != nil {
			fmt.Println("error: ReadAll 1")
			return
		}

		/*On traduit les octets en string utf8*/
		idsStrings := string(idsReaderBytes)

		/*On prend une slice qui contient une ligne par entrée*/
		ids := strings.Split(idsStrings, "\n")

		/*for i, s := range ids {
			fmt.Println(i, s)
		}
		*/

		id := ids[len(ids)-2 : len(ids)-1]

		/*On demande le dernier message*/
		// resp1, err := client.Get("https://jch.irif.fr:8082/chat/" + id[0])
		// if err != nil {
		// 	fmt.Println("error: client.Get 2")
		// 	return
		// }

		/*On lit le reader (on a des octets)*/
		// msgReaderBytes, err := io.ReadAll(resp1.Body)
		if err != nil {
			fmt.Println("error: ReadAll 2")
		}

		req, err := http.NewRequest("DELETE", "https://jch.irif.fr:8082/chat/"+id[0], nil)
		if err != nil {
			fmt.Println("error: client.Get 2")
			return
		}

		resp2, err := client.Do(req)

		resp2.Body.Close()
	}

}
