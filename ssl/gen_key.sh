#!/bin/sh

echo
echo '*****'
echo "** When asked for Common Name, enter the server's name"
echo '*****'
echo

openssl req -new -out server.csr -newkey rsa:2048 -keyout server.key -nodes

openssl req -x509 -days 365 -in server.csr -key server.key -out server.crt

echo
openssl x509 -subject -dates -fingerprint -noout -in server.crt

