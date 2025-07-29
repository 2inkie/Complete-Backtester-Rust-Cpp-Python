# trader/main.py

import cpp_engine
# import rust_data_pipeline

def run():
    print('--- Starting backtest ---')

    # Rust part
    print('1. [Dummy] Data prepared at: data/processed/AAPL.bin')
    data_path = "data/processed/AAPL.bin"

    # C++ part
    print('2. [Dummy] C++ engine ran successfully.')
    trade_log = cpp_engine.run(data_path)


    print(f'3. Analysis complete. Final log: {trade_log}')


if __name__ == "__main__":
    run()
    print('--- Backtest completed ---')
