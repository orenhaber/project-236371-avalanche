#ifndef TECH_PROJECT_NETWORK_H
#define TECH_PROJECT_NETWORK_H

#include "src/utils/utils.h"
#include <map>
#include <vector>
#include <string>

extern int BATCH_SIZE;

extern std::vector<std::string> host_list;
extern std::vector<std::string> ip_list;
extern std::map<std::string, int> socket_list;

int getQuerySocket(int max_clients, int local_port);
std::vector<std::string> Sample(int k_sample_size, std::vector<std::string> & local_ip_list);
std::map<std::string, color_t> QueryAll(std::vector<std::string> &sample_list, int round_number);
std::string lookup_host (const char *host);
int get_socket (std::string addr, int local_port);

#endif //TECH_PROJECT_NETWORK_H
