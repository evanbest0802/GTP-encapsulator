all: gtp_generator udp_packet_sender
gtp_generator: gtp_generator.o
	gcc -g -O2 -D_REENTRANT -Wall -o gtp_generator gtp_generator.o
gtp_generator.o: gtp_generator.c
	gcc -g -O2 -D_REENTRANT -Wall -c -o gtp_generator.o gtp_generator.c
udp_packet_sender:udp_packet_sender.o
	gcc -g -O2 -D_REENTRANT -Wall -o udp_packet_sender udp_packet_sender.o
udp_packet_sender.o: udp_packet_sender.c
	gcc -g -O2 -D_REENTRANT -Wall -c -o udp_packet_sender.o udp_packet_sender.c
clean:
	rm *.o gtp_generator udp_packet_sender
