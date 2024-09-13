#pragma once

#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/gauge.h>

#include <iostream>

class PrometheusDataBase {
public:
  PrometheusDataBase();

  void insertPoint(const std::string &user_id, double in, double out);

private:
  prometheus::Exposer exposer;
  std::shared_ptr<prometheus::Registry> registry;
  prometheus::Family<prometheus::Gauge>* gauge;
};