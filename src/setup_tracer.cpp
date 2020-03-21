#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

#include "setup_tracer.hpp"

namespace cst {
void setup_tracer(const std::string& config_filepath) {
  auto config_yaml = YAML::LoadFile(config_filepath);
  auto config = jaegertracing::Config::parse(config_yaml);
  auto tracer = jaegertracing::Tracer::make(
    "caf_shiviz_trial", config, jaegertracing::logging::consoleLogger());

  opentracing::Tracer::InitGlobal(
    std::static_pointer_cast<opentracing::Tracer>(tracer));
}
} // namespace cst
