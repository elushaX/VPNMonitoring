
#include <pqxx/pqxx>
#include <iostream>

class Database {
private:
  std::string connection_str;
  pqxx::connection* connection;

public:
  explicit Database(std::string conn_str) : connection_str(std::move(conn_str)) {
    connection = new pqxx::connection(connection_str);

    if (connection->is_open()) {
      std::cout << "Connected to database successfully." << std::endl;
    } else {
      std::cerr << "Could not connect to the database." << std::endl;
    }
  }

  ~Database() {
    // Close the connection
    delete connection;
  }

  // Method to check if an IP is present in the 'block_ips' table
  bool isIpBlocked(const std::string& ip_address) {
    try {
      // Start a non-transactional object
      pqxx::work txn(*connection);

      // SQL query to check if the IP address exists in the 'block_ips' table
      std::string query = "SELECT EXISTS(SELECT 1 FROM block_ips WHERE ip = " + txn.quote(ip_address) + ");";

      // Execute the query and store the result
      pqxx::result result = txn.exec(query);

      // Check if the IP exists
      bool ip_exists = result[0][0].as<bool>();

      // Commit the transaction
      txn.commit();

      // Return the result
      return ip_exists;

    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return false;
    }
  }
};