#pragma once

#include "InfluxDBFactory.h"
#include "InfluxDBBuilder.h"

#include <iostream>

class InfluxDataBase {
public:
  InfluxDataBase();

  void insertPoint(const std::string &user_id, double in, double out);

private:
  std::unique_ptr<influxdb::InfluxDB> db;
};