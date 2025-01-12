This is a simple multi-client ticketing system for football matches using TCP sockets in C.

## Features
- Simulates a ticketing system for 10 football matches.
- Allows multiple clients to connect simultaneously.
- Each client can:
  - Request tickets for a specific football match.
  - Receive a confirmation message or error response.
- Match data is dynamically generated at server startup.

## How does it work?
First, the server creates a list of football matches where there are:
  -A unique match ID.
  - Team names.
  - Ticket availability and pricing.

Second, the client onnects to the server and sends:
   - A match ID.
   - Number of tickets to purchase.

Third, the server:
   - Validates the request.
   - Processes the purchase if valid.
   - Updates ticket availability.
   - Sends a response back to the client.

##What you need:
(GNU Compiler Collection)
Linux-based system (for sockets and `fork()` usage)

Notes
The server generates match data randomly at startup.
The client must enter valid match IDs (0-9).
