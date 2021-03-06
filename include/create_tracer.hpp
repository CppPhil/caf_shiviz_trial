#pragma once
#include <memory>
#include <string>

#include <opentracing/tracer.h>

namespace cst {
std::shared_ptr<opentracing::Tracer>
create_tracer(const std::string& config_filepath,
              const std::string& service_name);
} // namespace cst
