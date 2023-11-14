package main

import (
	"crypto/tls"
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"strconv"
	"time"
)

type jsonAddressUDP struct {
	Host string `json:"host"`
	Port uint16 `json:"port"`
}

type PacketUDP struct {
	Id     uint32
	Type   uint8
	Length uint16
	Body   string
}

/*la fonction est degeu+fausse*/
func initPacketUDP(p *PacketUDP, packet []byte) {
	p.Id = uint32(packet[0]) + uint32(packet[1]) + uint32(packet[2]) + uint32((packet[3] & 1))
	p.Type = packet[4] - packet[3]&1
	p.Length = uint16(packet[5]) + uint16(packet[6])
	p.Body = string(packet[7:])
}

func main() {
	transport := &*http.DefaultTransport.(*http.Transport)
	transport.TLSClientConfig = &tls.Config{InsecureSkipVerify: true}
	client := &http.Client{
		Transport: transport,
		Timeout:   50 * time.Second,
	}

	r, err := client.Get("https://jch.irif.fr:8443/udp-addresses.json")

	if err != nil {
		log.Fatal("Get", err)
	}

	var m []jsonAddressUDP

	dec := json.NewDecoder(r.Body)

	_ = dec.Decode(&m)
	for _, jM := range m {
		fmt.Println(jM.Host + ":" + strconv.FormatInt(int64(jM.Port), 10))
	}
	net.ParseIP(m[0].Host)

	udpaddress := net.UDPAddr{
		IP:   net.ParseIP(m[0].Host),
		Port: int(m[0].Port),
	}

	/*A partir de la on a la connection*/
	// c, err := net.DialUDP("udp", nil, &udpaddress)

	// buf := make([]byte, 7)

	// c.Write(buf)

	c, err := net.ListenPacket("udp", "")
	if err != nil {
		log.Fatal("ListenPacket", err)
	}

	buf := make([]byte, 7)
	buf[0] = 0

	for i := 2000000000; i < 64*2000000000; {
		c.SetDeadline(time.Now().Add(time.Duration(i)))

		_, err = c.WriteTo(buf, &udpaddress)

		if errors.Is(err, os.ErrDeadlineExceeded) {
			fmt.Println(err)
			i = i * 2
			continue
		}

		buf2 := make([]byte, 2000)
		_, addr, err := c.ReadFrom(buf2)

		if errors.Is(err, os.ErrDeadlineExceeded) {
			i = i * 2
			fmt.Println(err)
			continue
		}

		var p PacketUDP
		initPacketUDP(&p, buf2)

		time.Sleep(1000000000)
		fmt.Println("Read/Write SUCCESS")
		fmt.Println(addr, p)
	}

	// fmt.Println(addr, p)

}
