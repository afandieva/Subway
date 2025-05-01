#ifndef METRO_H
#define METRO_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <map>

class Metro {
public:
    Metro();
    void run();

private:
    struct Line {
        std::vector<std::string> stations;
        std::string depot;
        bool is_shuttle = false;
    };

    std::map<std::string, Line> lines;
    std::mutex cout_mutex;

    void train(int train_id, const std::string& line_name, int route_type);
    void safe_print(const std::string& message);
};

#endif // METRO_H
