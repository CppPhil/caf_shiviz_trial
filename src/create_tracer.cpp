#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

#include "create_tracer.hpp"

namespace cst {
std::shared_ptr<opentracing::Tracer>
create_tracer(const std::string& config_filepath,
              const std::string& service_name) {
  auto config_yaml = YAML::LoadFile(config_filepath);
  auto config = jaegertracing::Config::parse(config_yaml);
  return jaegertracing::Tracer::make(service_name, config,
                                     jaegertracing::logging::consoleLogger());
}
} // namespace cst
