#pragma once

// Creates UDP socket and connects it to specific address and port
int udp_client_create_socket(const char* addr, unsigned short port);

void udp_client_send_total(int sockfd, const double percent);
void udp_client_send_core(int sockfd, const unsigned short core_num, const double percent);
