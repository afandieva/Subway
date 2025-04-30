#include "metro.h"
#include <chrono>
#include <thread>

Metro::Metro() {
    lines["Red"] = {
            {"Icheri Sheher", "Sahil", "28 May", "Ganjlik", "Nariman Narimanov",
             "Bakmil", "Ulduz", "Koroglu", "Qara Qaraev", "Neftchilar",
             "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
            "Bakmil"
    };

    lines["Green"] = {
            {"Darnagul", "Azadlig Prospekti", "Nasimi", "Memar Ajami", "20 January",
             "Inshaatchilar", "Elmlar Akademiyasy", "Nizami", "28 May",
             "Ganjlik", "Nariman Narimanov", "Bakmil", "Ulduz", "Koroglu",
             "Qara Qaraev", "Neftchilar", "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
            "Bakmil"
    };

    lines["Purple"] = {
            {"Khojasan", "Avtovagzal", "8 Noyabr"},
            "Khojasan"
    };

    lines["Yellow"] = {
            {"Jafar Jabbarly", "Hatai"},
            "N/A",
            true
    };
}

void Metro::safe_print(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << message << std::endl;
}

void Metro::train(int train_id, const std::string& line_name, int direction) {
    static std::map<std::string, std::mutex> station_mutexes;
    static std::mutex init_mutex;

    auto& line = lines[line_name];
    const auto& stations = line.stations;
    const std::string& depot = (line.depot != "N/A") ? line.depot : stations[0];
    bool is_shuttle = line.is_shuttle;

    int depot_index = 0;
    for (int i = 0; i < stations.size(); ++i) {
        if (stations[i] == depot) {
            depot_index = i;
            break;
        }
    }

    int current = depot_index;
    int step = (direction == 0) ? 1 : -1;

    {
        std::lock_guard<std::mutex> lock(init_mutex);
        for (const auto& station : stations)
            station_mutexes.try_emplace(station);
    }

    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departed from depot: " + stations[current]);

    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start_time < std::chrono::minutes(1)) {
        {
            std::unique_lock<std::mutex> lock(station_mutexes[stations[current]]);
            safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[current]);

            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[current]);
        }

        current += step;

        if (current < 0 || current >= static_cast<int>(stations.size())) {
            step = -step;
            current += step * 2;
        }
    }

    if (is_shuttle) {
        safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") finished at: " + stations[current]);
    } else {
        // Движение до депо
        while (stations[current] != depot) {
            if (current < depot_index) current++;
            else if (current > depot_index) current--;

            {
                std::unique_lock<std::mutex> lock(station_mutexes[stations[current]]);
                safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") returning, arrived at " + stations[current]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }

        safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") successfully returned to depot: " + depot);
    }
}

void Metro::run() {
    std::vector<std::thread> threads;

    threads.emplace_back(&Metro::train, this, 1, "Red", 0);
    threads.emplace_back(&Metro::train, this, 2, "Red", 1);
    threads.emplace_back(&Metro::train, this, 3, "Green", 0);
    threads.emplace_back(&Metro::train, this, 4, "Green", 1);
    threads.emplace_back(&Metro::train, this, 5, "Purple", 0);
    threads.emplace_back(&Metro::train, this, 6, "Yellow", 0);
    threads.emplace_back(&Metro::train, this, 7, "Yellow", 1);

    for (auto& t : threads) t.join();
}
