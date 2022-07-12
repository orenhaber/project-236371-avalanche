#include "worker.h"
#include "utils/utils.h"
#include "network/network.h"
#include <thread>
#include <chrono>
#include <random>

#define PORT 8080
#define PORT_2 8081
#define PORT_3 8083
#define PORT_4 8084
#define PORT_5 8085
#define PORT_6 8086
#define PORT_7 8087
#define PORT_8 8088

int KILL = std::stoi(std::string(getenv("KILL")));
int REPLICAS = std::stoi(std::string(getenv("REPLICAS")));
int K_SAMPLE_SIZE = std::stoi(std::string(getenv("K_SAMPLE_SIZE")));
double ALPHA = std::stod(std::string(getenv("ALPHA")));
int BETA = std::stoi(std::string(getenv("BETA")));

void QueryAnswerThread(worker& a, int local_port) {
    a.queryAnswer(local_port);
}



int main() {
    worker my_worker;
    for (int i = 0; i < KILL; i++) {
        std::string kill_worker = "worker-envars-fieldref-statefulset-" + std::to_string(i);
        if (std::string(getenv("MY_POD_NAME")) == kill_worker) {
            std::this_thread::sleep_for(std::chrono::milliseconds(3000000));
        }
    }

    std::thread query_answer_thread(QueryAnswerThread, std::ref(my_worker), PORT);
    std::thread query_answer_thread_2(QueryAnswerThread, std::ref(my_worker), PORT_2);
    std::thread query_answer_thread_3(QueryAnswerThread, std::ref(my_worker), PORT_3);
    std::thread query_answer_thread_4(QueryAnswerThread, std::ref(my_worker), PORT_4);
    std::thread query_answer_thread_5(QueryAnswerThread, std::ref(my_worker), PORT_5);
    std::thread query_answer_thread_6(QueryAnswerThread, std::ref(my_worker), PORT_6);
    std::thread query_answer_thread_7(QueryAnswerThread, std::ref(my_worker), PORT_7);
    std::thread query_answer_thread_8(QueryAnswerThread, std::ref(my_worker), PORT_8);


    std::this_thread::sleep_for(std::chrono::milliseconds(25000));

    for (int i = 0; i < std::stoi(std::string(getenv("REPLICAS"))); ++i) {
        std::string new_host = "worker-envars-fieldref-statefulset-" + std::to_string(i) + ".worker.default.svc.cluster.local";

        host_list.push_back(new_host);
        auto addr = lookup_host(new_host.c_str());
        if (addr != std::string(getenv("MY_POD_IP"))) {
            ip_list.push_back(addr);
            for (int j = 0; j < BATCH_SIZE; j++) {
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<double> dist(0, 81.0);
                int local_port = PORT;
                int mod = (int) dist(mt) % 8;
                switch (mod) {
                    case 0: {
                        local_port = PORT;
                        break;
                    }
                    case 1: {
                        local_port = PORT_2;
                        break;
                    }
                    case 2: {
                        local_port = PORT_3;
                        break;
                    }
                    case 3: {
                        local_port = PORT_4;
                        break;
                    }
                    case 4: {
                        local_port = PORT_5;
                        break;
                    }
                    case 5: {
                        local_port = PORT_6;
                        break;
                    }
                    case 6: {
                        local_port = PORT_7;
                        break;
                    }
                    case 7: {
                        local_port = PORT_8;
                        break;
                    }
                }
                auto new_addr = addr + std::to_string(j);
                int fd = get_socket(addr, local_port);
                socket_list[new_addr] = fd;
                print_log("adding new host " + new_host + " with ip " + addr + "and port " + std::to_string(local_port));
                print_log("added " + addr);

            }
        } else {
            print_log("removed my ip " + addr);
        }
    }

    print_log("started running pod: " + std::string(getenv("MY_POD_NAME")));


    my_worker.run_snowflake();
    query_answer_thread.join();
    query_answer_thread_2.join();
    query_answer_thread_3.join();
    query_answer_thread_4.join();
    query_answer_thread_5.join();
    query_answer_thread_6.join();
    query_answer_thread_7.join();
    query_answer_thread_8.join();
    return 0;
}