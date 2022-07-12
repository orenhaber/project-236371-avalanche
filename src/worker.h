#ifndef TECH_PROJECT_WORKER_H
#define TECH_PROJECT_WORKER_H

#include "src/utils/utils.h"
#include <atomic>
#include <vector>

class worker {
public:
    std::map<int, int> m_count;
    std::map<int, color_t> m_colors;
    std::atomic<int> m_wasted_rounds;

    worker();
    void accept_round(int round_number);
    void run_snowflake();
    void queryAnswer(int local_port);
};

void run_snowflake_loop(worker *our_worker, int round_number, std::vector<std::string> local_ip_list);
extern int K_SAMPLE_SIZE;
extern double ALPHA;
extern int BETA;
extern int REPLICAS;
#endif //TECH_PROJECT_WORKER_H
