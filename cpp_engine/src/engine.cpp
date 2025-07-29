#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

std::vector<std::map<std::string, pybind11::object>> run(const std::string &data_path) {
    // This print statement confirms that we are running the REAL C++ code.
    std::cout << "[C++] Engine received data path: " << data_path << std::endl;

    // Create a dummy trade log to return to Python
    std::vector<std::map<std::string, pybind11::object>> trade_log;

    std::map<std::string, pybind11::object> trade;
    trade["asset"] = pybind11::str("AAPL");
    trade["type"] = pybind11::str("BUY");
    trade["price"] = pybind11::float_(150.0);
    trade_log.push_back(trade);

    return trade_log;
}

// The first argument, "cpp_engine", MUST match the name of the output file.
PYBIND11_MODULE(cpp_engine, m) {
    m.doc() = "A C++ backtesting engine exposed via pybind11";

    // Exposing the C++ 'run' function to Python as 'run'.
    m.def("run", &run, "Runs the core backtest simulation");
}