# HTTP Server & TCP/IP Notes

```md
# HTTP Server & TCP/IP - My Notes

## 1. TCP/IP Model

The TCP/IP model consists of four layers:

1. Application Layer
2. Transport Layer (TCP/UDP)
3. Internet Layer (IP)
4. Link/Physical Layer

My HTTP server is **NOT** implementing the TCP/IP stack.

Instead, it is an **Application Layer program** that **uses** the TCP/IP stack provided by the operating system.

---

## 2. Who Implements TCP/IP?

Windows/Linux/macOS already implement:

- TCP
- IP
- Packet routing
- Retransmission
- Error checking
- Connection management

My application simply uses these services through the Socket API.

---

## 3. What Am I Building?

I am building:

- An HTTP Server
- An Application Layer application
- An implementation of the HTTP protocol

I am NOT implementing:

- TCP
- IP
- The Internet
- The TCP/IP stack

---

## 4. What is a Socket?

A socket is a communication endpoint.

Think of it like a telephone.

Creating a socket does NOT connect to anyone.

It only asks the operating system:

"I want to communicate over the network."

The OS returns a socket object.

Initially it has:

✔ Exists

✖ No address

✖ No port

✖ No connection

✖ Not listening

---

## 5. Socket Life Cycle

1. Initialize Winsock (Windows only)
2. Create Socket
3. Bind
4. Listen
5. Accept
6. Receive
7. Send
8. Close

---

## 6. IP Address

An IP address identifies a computer on a network.

Think of it as a house address.

Example:

192.168.1.20

Every computer has an IP.

---

## 7. Port

A port identifies an application running on a computer.

Think of it as an apartment number inside a building.

Building = IP

Apartment = Port

Example:

192.168.1.20:8080

means

Computer:
192.168.1.20

Application:
Port 8080

---

## 8. Why Server Needs Both IP and Port

IP tells:

Which computer?

Port tells:

Which application?

Without the port, Windows wouldn't know which application should receive the data.

---

## 9. localhost (127.0.0.1)

127.0.0.1 always means:

"This computer."

It never leaves the machine.

If another person types:

127.0.0.1

they connect to THEIR own computer, not mine.

---

## 10. Why Two Applications Can't Listen on Same Port

Only one application can own:

IP + Port

Otherwise Windows wouldn't know where to deliver incoming data.

This is why:

127.0.0.1:8080

can only belong to one application.

---

## 11. Client vs Server Ports

Servers use fixed ports.

Examples:

HTTP -> 80

HTTPS -> 443

Clients use temporary (ephemeral) ports.

Example:

Chrome

192.168.1.5:53124

↓

Google

142.xx.xx.xx:443

Next time Chrome starts it may use:

53201

54122

60215

etc.

Windows assigns them automatically.

---

## 12. Why Client Ports Can Change

Clients initiate connections.

Windows only needs a free local port.

Once the TCP connection is created,

the local port remains fixed until the connection closes.

After the connection ends,

Windows can reuse that port for another application.

---

## 13. TCP Connection Identity

A TCP connection is identified by four values:

(Local IP,
 Local Port,
 Remote IP,
 Remote Port)

Example:

192.168.1.5:53124

↓

31.xx.xx.xx:443

Changing any one of these creates a different connection.

---

## 14. Can Server Send Data Anytime?

Yes,

but only while the TCP connection exists.

If the client closes the application,

the connection disappears.

The server usually stores pending data until the client reconnects.

Example:

WhatsApp

If offline

↓

Server stores messages

↓

Client reconnects

↓

Messages delivered

---

## 15. What if Port Changes Mid Conversation?

It doesn't.

Ports remain fixed for the lifetime of a TCP connection.

If Wi-Fi disconnects,

the connection dies.

A completely new connection is created later.

---

## 16. Does Client Trust Server?

Yes.

HTTPS exists largely so that the client can verify the server.

The browser checks:

- Certificate
- Certificate Authority (CA)
- Domain name
- Expiry
- Validity

Only then does it trust the server.

---

## 17. Does Server Trust Client?

No.

Servers assume every client could be malicious.

Clients prove identity using:

- Password
- Session
- Token
- Authentication
- Cookies
- API Keys

---

## 18. Is Server IP Secret?

Usually no.

DNS converts:

google.com

↓

IP Address

Anyone can know the IP.

Knowing the IP does NOT mean access is granted.

Authentication still happens.

---

## 19. Can I Make My Own WhatsApp Client?

Technically:

Yes, IF

- You know the protocol.
- You authenticate correctly.
- The server allows it.

Practically:

Very difficult because companies use:

- Proprietary protocols
- Encryption
- Authentication
- Device registration

---

## 20. Why Use Port 8080?

Port 80 is the standard HTTP port.

8080 is simply a convention for development.

Reasons:

- Doesn't require special privileges on many systems.
- Usually free.
- Easy to remember.

There is nothing magical about 8080.

Any free port can be used.

---

## 21. Multiple Browsers Don't Conflict

Chrome

↓

53124

Edge

↓

53125

Firefox

↓

53126

All connect to:

Google:443

Each browser has its own temporary local port.

---

## 22. Can One Computer Have Multiple HTTP Servers?

Yes.

Example:

localhost:3000

localhost:5000

localhost:8080

All can run simultaneously.

Only one server can listen on a particular:

IP + Port

---

## 23. Why Don't Websites Show Ports?

Browser defaults:

HTTP -> 80

HTTPS -> 443

Therefore:

https://google.com

actually means

google.com:443

---

## 24. How Can One Machine Host Many Websites?

Using a Reverse Proxy.

Example:

Internet

↓

Nginx

↓

Shop Server

↓

Chat Server

↓

Admin Server

Only Nginx listens on:

80 / 443

Internal applications use different ports.

---

## 25. Reverse Proxy

A Reverse Proxy:

- Accepts incoming requests.
- Examines the request.
- Chooses the correct backend server.
- Forwards the request.
- Returns the response.

Client never knows about internal ports.

---

## 26. Host Header

Browser sends:

Host: ecom.com

or

Host: chat.com

Reverse proxy reads the Host header and routes to the correct backend.

Example:

Host: ecom.com

↓

localhost:3000

Host: chat.com

↓

localhost:5000

---

## 27. Overall Architecture

                Browser
                    │
                    ▼
            HTTP / HTTPS
                    │
                    ▼
         Reverse Proxy (Nginx)
                    │
          ┌─────────┴─────────┐
          ▼                   ▼
    E-commerce          Chat Server
     Port 3000           Port 5000
                    │
                    ▼
            Windows TCP/IP Stack
                    │
                    ▼
             Internet Hardware

---

## Key Takeaways

- My HTTP server is an Application Layer application.
- Windows implements the TCP/IP stack.
- A socket is a communication endpoint.
- IP identifies a computer.
- Port identifies an application.
- Server ports are fixed.
- Client ports are temporary.
- TCP connections are identified by 4 values.
- HTTP runs over TCP.
- HTTPS adds TLS for secure communication.
- Only one application can listen on one IP + Port.
- Multiple applications can run on different ports.
- Reverse proxies allow many websites and services to share ports 80 and 443.
- The Host header tells the reverse proxy which website the client wants.
```

---

## One addition (for your future self)

As you continue building the server, keep this mental model in mind:

```text
My C++ Code
      │
      ▼
Socket API (socket, bind, listen, accept, recv, send)
      │
      ▼
Windows TCP/IP Stack
      │
      ▼
Network
      │
      ▼
Another Computer
```

If you always remember **which layer you're working at**, you'll rarely get confused. Right now you're learning the boundary between your application and the operating system—and that's one of the most valuable concepts in systems programming.
