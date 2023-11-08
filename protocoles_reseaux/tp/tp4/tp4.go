package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os"
	"time"

	"github.com/gorilla/websocket"
)

type chatMessage struct {
	Id   string `json:"id,omitempty"`
	Time int64  `json:"time,omitempty"`
	Body string `json:"body"`
}

type jsonMessage struct {
	Type     string        `json:"type"`
	Message  string        `json:"message,omitempty"`
	Messages []chatMessage `json:"messages,omitempty"`
	Count    int           `json:"count,omitempty"`
	Error    string        `json:"error,omitempty"`
}

func update(c *websocket.Conn) {
	var m jsonMessage

	err := c.ReadJSON(&m)
	if err != nil {
		log.Fatal(err)
	}
	if m.Messages == nil {
		fmt.Println(m)
	} else {
		fmt.Println(m.Messages[0].Body)
	}

}

func process_file(filename string) string {
	f, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}

	len_file, _ := f.Seek(0, 2)
	f.Seek(0, 0)

	text := make([]byte, len_file)

	_, err = f.Read(text)
	if err != nil {
		log.Fatal(err)
	}
	f.Close()

	return bytes.NewBuffer(text).String()
}

func readloop2(c *websocket.Conn) {
	for {
		var m jsonMessage

		err := c.ReadJSON(&m)
		if err != nil {
			c.Close()
			log.Fatal(err)
		}

		fmt.Println(m.Messages)

		time.Sleep(500000000)
	}
}
func readloop1(c *websocket.Conn) {
	for {
		_, r, err := c.NextReader()
		if err != nil {
			c.Close()
			break
		}

		ms, _ := io.ReadAll(r)
		fmt.Println(string(ms))
		dec := json.NewDecoder(r)
		var m jsonMessage

		_ = dec.Decode(&m)
		if m.Type == "error" {

		}
		msglist := make([]string, len(m.Messages))

		for i, msg := range m.Messages {
			msglist[i] = msg.Body
		}

	}
}

func main() {
	d := websocket.Dialer{}
	c, _, err := d.Dial("wss://jch.irif.fr:8082/chat/ws", nil)
	defer c.Close()

	if err != nil {
		log.Fatal("Dial", err)
	}

	err = c.WriteJSON(jsonMessage{Type: "get", Count: 1})
	if err != nil {
		log.Fatal(err)
	}

	var m jsonMessage

	err = c.ReadJSON(&m)
	if err != nil {
		log.Fatal(err)
	}

	for _, ms := range m.Messages {
		fmt.Println(ms.Body)
	}

	_ = c.WriteJSON(jsonMessage{Type: "subscribe"})
	update(c)

	// text := process_file("msg.txt")
	/*Ecrit des msgs dans le chat et update*/
	for {
		var msg jsonMessage
		msg.Type = "post"
		// msg.Message = text
		fmt.Scanf("%s", &msg.Message)
		err = c.WriteJSON(msg)
		if err != nil {
			log.Fatal(err)
		}
		time.Sleep(1000000000)
		update(c)
	}
	// wc, err := c.NextWriter(websocket.TextMessage)
	// if err != nil {
	// 	log.Fatal("NextWriter", err)
	// }
	// message := []byte(`{"type": "get", "count": 20}`)

	// _, err = wc.Write(message)
	// if err != nil {
	// 	log.Fatal("Write", err)
	// }
	// err = wc.Close()
	// if err != nil {
	// 	log.Fatal("Close", err)
	// }

	// readloop2(c)

}
