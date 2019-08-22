# UDSOverJ1939
This is demo application which read ECU fault.
Stack is written to read UDS frame over J1939 protocol.

## Build instruction
pre-requisites
SocketCAN lib
    
    Clone repository
    cd <root-dir>
    mkdir build
    cd build
    cmake ..
    cmake --build .

Things TBD

    Broadcast message 0xDB00 PGN
    Multiple ECU handling
    USDT frame

