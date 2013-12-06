#!/bin/bash
openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout mycert.pem -out mycert.pem -subj /CN=www.example.org/O=Example/C=US/ST=Oregon/L=Portland
openssl s_server -cert mycert.pem
rm mycert.pem
