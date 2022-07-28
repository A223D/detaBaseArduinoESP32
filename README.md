# Library For Interacting with a Deta.sh Base Instance Using an Arduino-Based ESP32

This library is meant to abstract away the networking aspect of interacting with a Deta.sh base Instance, a free online NoSQL data base. This library uses Deta's HTTP API, and exposes all functions listed here. 

Required items to use this library:

* Deta Project ID

* Deta Project key

* Deta Base name

Optional items:

* Root CA certificate

(This is provided in the library, but it will change eventually, at which point you will manually have to supply
