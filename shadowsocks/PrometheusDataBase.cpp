#include "PrometheusDataBase.hpp"

using namespace prometheus;

#include <string>

PrometheusDataBase::PrometheusDataBase() : exposer{"127.0.0.1:8080"} {

  registry = std::make_shared<prometheus::Registry>();

  gauge = &prometheus::BuildGauge().Name("own_metrics").Help("own metrics").Register(*registry);

  exposer.RegisterCollectable(registry);
}

void PrometheusDataBase::insertPoint(const std::string &user_id, double in, double out) {
  gauge->Add({{"ip_address", user_id}, {"direction", "in"}}).Set(in);
  gauge->Add({{"ip_address", user_id}, {"direction", "out"}}).Set(out);
}

