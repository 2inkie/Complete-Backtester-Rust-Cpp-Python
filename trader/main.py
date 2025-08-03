# trader/main.py

import cpp_engine
import rust_data_pipeline
import json

API_PATH = "trader/config.json"
SYMBOL='AAPL'
OUTPUT_PATH = f"data/{SYMBOL}_data.bin"
with open(API_PATH, 'r') as f:
    API_KEY = json.load(f)['api_key']

def run():
    print('--- Starting backtest ---')

    # Rust part
    print('1. Calling Rust data pipeline...')
    
    try:
        rust_data_pipeline.fetch_and_clean(
        symbol=SYMBOL,
        api_key=API_KEY,
        output_path=OUTPUT_PATH
        )
        print(f"Successfully fetched and saved data to {OUTPUT_PATH}")
    except Exception as e:
        print(f"An error occurred: {e}")

    # C++ part
    print('2. [Dummy] C++ engine ran successfully.')
    trade_log = cpp_engine.run(OUTPUT_PATH)


    print(f'3. Analysis complete. Final log: {trade_log}')


if __name__ == "__main__":
    run()
    print('--- Backtest completed ---')
