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

void Metro::train(int train_id, const std::string& line_name, int route_type) {
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

    {
        std::lock_guard<std::mutex> lock(init_mutex);
        for (const auto& station : stations)
            station_mutexes.try_emplace(station);
    }

    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departed from depot: " + depot);

    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start_time < std::chrono::minutes(1)) {
        if (line_name == "Red") {
            if (route_type == 0) {
                // Маршрут 1: Bakmil -> Icheri Sheher -> Bakmil (просто туда-обратно)
                for (int i = depot_index; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                for (int i = 1; i <= depot_index; ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }
            } else {
                // Маршрут 2:
                // 1. Bakmil -> Icheri Sheher
                for (int i = depot_index; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // 2. Icheri Sheher -> Bakmil
                for (int i = 1; i <= depot_index; ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // 3. Bakmil -> Azi Aslanov
                for (int i = depot_index; i < static_cast<int>(stations.size()); ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // 4. Azi Aslanov -> Icheri Sheher
                for (int i = stations.size()-2; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }
            }
        } else if (line_name == "Green") {
            if (route_type == 0) {
                // Поезд 3: Полный маршрут Bakmil -> Darnagul -> Azi Aslanov -> Bakmil

                // 1. Bakmil -> Darnagul
                for (int i = depot_index; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print(
                            "Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // 2. Darnagul -> Azi Aslanov
                for (int i = 1; i < static_cast<int>(stations.size()); ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print(
                            "Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // 3. Azi Aslanov -> Bakmil
                for (int i = stations.size() - 2; i >= depot_index; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print(
                            "Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }
            } else {
                // Поезд 4: Челночное движение Bakmil <-> Darnagul

                // 1. Bakmil -> Darnagul
                for (int i = depot_index; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print(
                            "Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // 2. Darnagul -> Bakmil
                for (int i = 1; i <= depot_index; ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print(
                            "Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }
            }
        } else if (line_name == "Purple") {
            if (route_type == 0) {
                // Прямое направление: Khojasan -> Avtovagzal -> 8 Noyabr
                for (int i = 0; i < static_cast<int>(stations.size()); ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // Обратное направление: 8 Noyabr -> Avtovagzal -> Khojasan
                for (int i = stations.size()-2; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }
            } else {
                // Обратное направление для второго поезда
                // 8 Noyabr -> Avtovagzal -> Khojasan
                for (int i = stations.size()-1; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }

                // Прямое направление: Khojasan -> Avtovagzal -> 8 Noyabr
                for (int i = 1; i < static_cast<int>(stations.size()); ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + stations[i]);
                }
            }
        } else if(line_name == "Yellow") {
            // Оригинальная логика для других линий
            if (route_type == 0) {
                // Направление: Jafar Jabbarly -> Hatai
                for (int i = 0; i < static_cast<int>(stations.size()); ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name +
                               ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name +
                               ") departing from " + stations[i]);
                }
            } else {
                // Направление: Hatai -> Jafar Jabbarly
                for (int i = stations.size()-1; i >= 0; --i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name +
                               ") arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name +
                               ") departing from " + stations[i]);
            }
        }
    }

    // Возвращение в депо
    if (!is_shuttle) {
        safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") is returning to depot: " + depot);
        if (line_name == "Purple") {
            // Для фиолетовой линии возвращаемся в Khojasan
            int current_pos = (route_type == 0) ? 0 : stations.size()-1;
            int step = (route_type == 0) ? 1 : -1;

            while (stations[current_pos] != depot) {
                current_pos += step;
                std::unique_lock<std::mutex> lock(station_mutexes[stations[current_pos]]);
                safe_print("Train " + std::to_string(train_id) + " (" + line_name +
                           ") returning, arrived at " + stations[current_pos]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        } else if (line_name == "Green")
        {
            int current_pos = (route_type == 0) ? depot_index : 0; // Для маршрута 0 уже в депо, для маршрута 1 на Icheri Sheher

            if (route_type == 1) {
                // Движение от Icheri Sheher до Bakmil
                for (int i = 0; i <= depot_index; ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") returning, arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                }
            }

        }else if (line_name == "Red") {
            int current_pos = (route_type == 0) ? depot_index : 0; // Для маршрута 0 уже в депо, для маршрута 1 на Icheri Sheher

            if (route_type == 1) {
                // Движение от Icheri Sheher до Bakmil
                for (int i = 0; i <= depot_index; ++i) {
                    std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                    safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") returning, arrived at " + stations[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                }
            }
        }

        safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") successfully returned to depot: " + depot);
    }
}
    }

void Metro::run() {
    std::vector<std::thread> threads;

    threads.emplace_back(&Metro::train, this, 1, "Red", 0); // Маршрут 1: Bakmil <-> Icheri Sheher
    threads.emplace_back(&Metro::train, this, 2, "Red", 1); // Маршрут 2: Bakmil -> Azi Aslanov -> Icheri Sheher -> Bakmil
    threads.emplace_back(&Metro::train, this, 3, "Green", 0);
    threads.emplace_back(&Metro::train, this, 4, "Green", 1);
    threads.emplace_back(&Metro::train, this, 5, "Purple", 0);
    threads.emplace_back(&Metro::train, this, 6, "Yellow", 0);
    threads.emplace_back(&Metro::train, this, 7, "Yellow", 1);

    for (auto& t : threads) t.join();
}
