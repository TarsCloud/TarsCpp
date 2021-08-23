
#create ca key
openssl genrsa -out ca.key 2048

#create ca crt
openssl req -x509 -new -nodes -key ca.key -subj "/CN=*.demo.com" -days 5000 -out ca.crt

#########################################################################################
#create server key
openssl genrsa -out server.key 2048

#create server csr
openssl req -new -key server.key -subj "/CN=*.demo.com" -out server.csr

#ca sign server, create server crt
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 5000

#########################################################################################
#create client key
openssl genrsa -out client.key 2048

#create clilent csr
openssl req -new -key client.key -subj "/CN=*.demo.com" -out client.csr

#ca sign client, create client crt
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 5000

#########################################################################################
#create server1 key
openssl genrsa -out server1.key 2048

#create server1 csr
openssl req -new -key server1.key -subj "/CN=*.demo.com" -out server1.csr

#ca sign server1, create server crt
openssl x509 -req -in server1.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server1.crt -days 5000

#########################################################################################

#create client1 key
openssl genrsa -out client1.key 2048

#create clilent1 csr
openssl req -new -key client1.key -subj "/CN=*.demo.com" -out client1.csr

#ca sign client1, create client1 crt
openssl x509 -req -in client1.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client1.crt -days 5000