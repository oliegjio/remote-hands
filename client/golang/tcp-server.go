package main

import (
	"bufio"
	"fmt"
	"net"
)

func handleConnection(conn net.Conn) {
	fmt.Printf("Serving %s\n", conn.RemoteAddr().String())
	for {
		data, err := bufio.NewReader(conn).ReadString('\n')
		checkError(err)
		fmt.Println(string(data))
		//temp := strings.TrimSpace(string(data)
	}
	conn.Close()
}

func startTCPServer() {
	port := ":8080"
	listener, err := net.Listen("tcp4", port)
	checkError(err)
	defer listener.Close()

	for {
		conn, err := listener.Accept()
		checkError(err)
		go handleConnection(conn)
	}
}
