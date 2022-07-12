#include "worker.h"
#include "src/network/network.h"
#include "src/utils/utils.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define NUMBER_OF_SOCKETS 1000

worker::worker() {
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    for (int i = 0; i < number_of_rounds; i++) {
        m_count[i] = 0;
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::srand(seed);
        m_colors[i] = (color_t)(rand() % 2);
    }
    m_wasted_rounds = 0;
}

void worker::accept_round(int round_number) {
    print_log("Accepted color: " + std::to_string((int) m_colors.at(round_number)) + " in round number:" + std::to_string(round_number));
    print_csv(std::to_string(round_number) + "," + std::to_string((int) m_colors.at(round_number)));
}

void worker::queryAnswer(int local_port) {
    char buffer[10] = {0};
    int max_clients = NUMBER_OF_SOCKETS;
    int client_socket[NUMBER_OF_SOCKETS] = {0};
    fd_set readfds;
    int master_socket = getQuerySocket(max_clients, local_port);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);

        int max_sd = master_socket;
        for (int i = 0; i < max_clients; i++) {
            int sd = client_socket[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }
        if (FD_ISSET(master_socket, &readfds)) {
            int new_socket = accept(master_socket, NULL, NULL);
            if (new_socket < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }
        for (int i = 0; i < max_clients; i++) {
            int sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                int valread = read(sd, buffer, 10);
                if (valread == 0) {
                    continue;
                } else {
                    buffer[valread] = '\0';
                    int requested_round = atoi(buffer);
                    std::string msg = std::to_string(m_colors.at(requested_round));
                    send(sd, msg.c_str(), strlen(msg.c_str()), 0);
                }
            }
        }
    }
    return;
}



void run_snowflake_loop(worker *our_worker, int round_number, std::vector<std::string> local_ip_list) {

    while (true) {
        auto k_sample_list = Sample(K_SAMPLE_SIZE, local_ip_list);
        auto sample_results = QueryAll(k_sample_list, round_number);
        if (sample_results.empty()) {
            continue;
        }
        for (auto color: colors) {
            int count = CountSampleResults(sample_results, color);
            if (count >= ALPHA * K_SAMPLE_SIZE) {
                if (color != our_worker->m_colors.at(round_number)) {
                    our_worker->m_colors[round_number] = color;
                    our_worker->m_count[round_number] = 0;
                } else {
                    our_worker->m_count[round_number]++;
                    if (our_worker->m_count[round_number] > BETA) {
                        return;
                    }
                }
            }
        }
    }
}


void worker::run_snowflake() {
    std::vector <std::thread> snowflake_threads;
    auto start = std::chrono::steady_clock::now();
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    for (int j = 0; j < number_of_rounds / BATCH_SIZE; j++) {
        if (j == 300) {
            start = std::chrono::steady_clock::now();
        }

        auto start_batch = std::chrono::steady_clock::now();

        for (int i = 0; i < BATCH_SIZE; ++i) {
            snowflake_threads.emplace_back(run_snowflake_loop, this, j * BATCH_SIZE + i, ip_list);
        }
        for (auto &loop_thread: snowflake_threads) {
            loop_thread.join();
        }

        auto end_batch = std::chrono::steady_clock::now();
        std::cout << "Elapsed time - batch: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end_batch - start_batch).count()
                  << " ms" << std::endl;
        snowflake_threads.clear();
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    print_csv(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()));

    for (int j = 0; j < number_of_rounds; j++) {
        accept_round(j);
    }

}
