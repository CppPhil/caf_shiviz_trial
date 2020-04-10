#pragma once
#include <memory>
#include <string>

#include <caf/fwd.hpp>

#include <opentracing/span.h>

#include "error.hpp"

namespace cst {
std::unique_ptr<opentracing::Span>
create_span(const caf::tracing_data* tracing_data,
            const std::string& operation_name);
} // namespace cst
