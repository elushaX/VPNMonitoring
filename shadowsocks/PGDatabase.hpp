#pragma once

#include "PrometheusDataBase.hpp"

#include <pqxx/pqxx>
#include <iostream>

class PGDatabase {

private:
  std::string connection_str;
  pqxx::connection *connection;

public:
  explicit PGDatabase();
  ~PGDatabase();

  void recordTotal(double speedBytesIn, double speedBytesOut);
  void recordConnection(const std::string &ip_address, double speedBytesIn, double speedBytesOut);

private:
  void checkDataBase();

private:
  PrometheusDataBase timeSeriesDataBase;
};