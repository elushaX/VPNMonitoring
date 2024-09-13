#include "InfluxDataBase.hpp"


#include <string>
#include <chrono>

InfluxDataBase::InfluxDataBase() {
  db = influxdb::InfluxDBBuilder::http("http://root:15319@localhost:8086?db=test").connect();

  // std::vector<influxdb::Point> points = db->query("SELECT * FROM test");
  const auto response = db->execute("SHOW DATABASES");

  if (db == nullptr) {
    std::cerr << "Failed to connect to the influx database";
  }
}

void InfluxDataBase::insertPoint(const std::string &user_id, double in, double out) {
  using namespace std::chrono;
  // int64_t timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

  db->write(influxdb::Point{user_id}
                .addField("in", in)
                .addField("out", out)
  );
}

