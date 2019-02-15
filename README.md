# GTP-encapsulator
add GTPv1U header to the incoming packets.
## Build it
Use make to generate gtp_generator and udp_packet_sender.
```bash
make
```
## Run gtp_generator
Please use superuser privilege to run this program.
```bash
sudo ./gtp_generator
```
## Run udp_packet_sender
type the command below to run this program.
```bash
./udp_paccket_generator <server IP address>
```
