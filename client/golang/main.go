package main

import (
	"fmt"
	// "./gonum/quat"
	"net"
	"time"
	"os"
)

func check(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func main() {
	service := ":8080"
	tcpAddr, err := net.ResolveTCPAddr("tcp4", service)
	check(err)
	listener, err := net.ListenTCP("tcp", tcpAddr)
	check(err)

	buffer := [] byte {}

	for {
		conn, err := listener.Accept()
		if err != nil {
			continue
		}
		conn.SetDeadline(time.Now())
		for {
			if _, err := conn.Read(buffer); err != nil {
				conn.Close()
				conn = nil
				fmt.Println(err)
				continue
			} else {
				conn.SetDeadline(time.Now().Add(10 * time.Millisecond))
			}
			if len(buffer) == 0 {
				continue
			}
			fmt.Println(buffer)
		}
	}
}
