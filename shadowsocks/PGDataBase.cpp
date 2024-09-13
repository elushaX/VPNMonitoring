
#include "PGDatabase.hpp"

PGDatabase::PGDatabase() : connection_str("host=localhost port=5432 dbname=vpn_db user=auser password=1234") {
  connection = new pqxx::connection(connection_str);

  if (connection->is_open()) {
    std::cout << "Connected to PG database successfully." << std::endl;
  } else {
    std::cerr << "Could not connect to the PG database." << std::endl;
  }

  checkDataBase();
}

PGDatabase::~PGDatabase() {
  delete connection;
}

void PGDatabase::checkDataBase() {
  pqxx::work txn(*connection);
  try {
    txn.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "ip_address VARCHAR(255) UNIQUE, "
        "first_connection TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");"
    );
    txn.commit();
  } catch (const std::exception &e) {
    std::cerr << "Error in checkDataBase: " << e.what() << std::endl;
    txn.abort();
  }
}

void PGDatabase::recordTotal(double speedBytesIn, double speedBytesOut) {
  timeSeriesDataBase.insertPoint("total", speedBytesIn, speedBytesOut);
}

void PGDatabase::recordConnection(const std::string &ip_address, double speedBytesIn, double speedBytesOut) {
  pqxx::work txn(*connection);

  try {
    txn.exec(
        "INSERT INTO users (ip_address) VALUES ($1) "
        "ON CONFLICT (ip_address) DO NOTHING;",
        pqxx::params{ip_address}
    );

    pqxx::result res = txn.exec(
        "SELECT id FROM users WHERE ip_address = $1;",
        pqxx::params{ip_address}
    );

    if (!res.empty()) {
      int user_id = res[0][0].as<int>();
      timeSeriesDataBase.insertPoint(std::to_string(user_id), speedBytesIn, speedBytesOut);
    }

    txn.commit();

  } catch (const std::exception &e) {
    std::cerr << "Error in recordConnection: " << e.what() << std::endl;
    txn.abort();
  }
}
