# GTP-encapsulator
add GTPv1U header to the incoming packets.
## Build it
Use make to generate gtp_generator and udp_packet_sender.
```bash
make
```
## Run gtp_generator
Please use superuser privilege to run server program.
```bash
sudo ./gtp_generator
```
## Run udp_packet_sender
type the command below to run client program.
```bash
./udp_packet_sender <server IP address>
```
