package main

import (
	"fmt"
	"log"
	"math/big"
	"net"
	"time"
)

var p, g, p1, zero, one big.Int

func init() {
	p.SetString("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A63A36210000000000090563", 16)
	g.SetInt64(2)
	zero.SetInt64(0)
	one.SetInt64(1)
	p1.Sub(&p, &one)
}

func main() {
	conn, err := net.Dial("tcp", "localhost:8081")

	if err != nil {
		log.Fatal(err)
	}
	str := make([]byte, 2000)

	_, err = conn.Read(str)
	fmt.Println(str)

	fmt.Println(conn.LocalAddr(), conn.RemoteAddr())

	for {
		time.Sleep(1000000000)
	}

}
