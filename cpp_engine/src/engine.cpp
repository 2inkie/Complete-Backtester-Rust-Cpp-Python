#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stdexcept>

namespace py = pybind11;

#pragma pack(push, 1)
struct PriceBar {
    char date[11]; // Date in YYYY-MM-DD format
    double open;
    double high;
    double low;
    double close;
    double volume;
};
#pragma pack(pop)

std::vector<PriceBar> load_price_bars(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Vector to hold the data
    std::vector<PriceBar> bars(size / sizeof(PriceBar));
    if (!file.read(reinterpret_cast<char*>(bars.data()), size)) {
        throw std::runtime_error("Error reading file: " + filepath);
    }

    return bars;
}

std::map<std::string, py::object>
run_backtest(
    const std::string& filepath,
    py::function strategy,
    double starting_cash = 10000.0
) {
    auto bars = load_price_bars(filepath);

    // Portfolio State
    double cash = starting_cash;
    long shares = 0;

    std::vector<std::map<std::string, py::object>> trade_log;
    std::vector<std::map<std::string, py::object>> portfolio_log;

    for (const auto& bar : bars) {
        // Pass full portfolio state to the Python strategy
        py::dict current_state;
        current_state["date"] = std::string(bar.date);
        current_state["open"] = bar.open;
        current_state["high"] = bar.high;
        current_state["low"] = bar.low;
        current_state["close"] = bar.close;
        current_state["volume"] = bar.volume;
        current_state["cash"] = cash;
        current_state["shares"] = shares;

        // Call Python strategy and get its decision
        // Expects a tuple: (string_action, long_trade_size)
        py::tuple decision = strategy(current_state);

        // Handle flexible trade sizes
        if (py::len(decision) == 2) {
            std::string action = decision[0].cast<std::string>();
            long trade_size = decision[1].cast<long>();

            if (action == "BUY" && cash >= bar.close * trade_size) {
                shares += trade_size;
                cash -= bar.close * trade_size;
                trade_log.push_back({
                    {"date", py::str(bar.date)},
                    {"action", py::str("BUY")},
                    {"price", py::float_(bar.close)},
                    {"shares", py::int_(trade_size)}
                });
            } else if (action == "SELL" && shares >= trade_size) {
                shares -= trade_size;
                cash += bar.close * trade_size;
                trade_log.push_back({
                    {"date", py::str(bar.date)},
                    {"action", py::str("SELL")},
                    {"price", py::float_(bar.close)},
                    {"shares", py::int_(trade_size)}
                });
            }
        }

        // Log portfolio value at the end of each day
        double portfolio_value = cash + (shares * bar.close);
        portfolio_log.push_back({
            {"date", py::str(bar.date)},
            {"value", py::float_(portfolio_value)}
        });
    }

    // Return both logs to Python for analysis
    std::map<std::string, py::object> results;
    results["trades"] = py::cast(trade_log);
    results["portfolio"] = py::cast(portfolio_log);
    return results;
}

PYBIND11_MODULE(cpp_engine, m) {
    m.doc() = "A high-performance, realistic backtesting engine";
    m.def("run_backtest", &run_backtest, "Runs the backtest",
          py::arg("filepath"),
          py::arg("strategy"),
          py::arg("starting_cash") = 10000.0
    );
}