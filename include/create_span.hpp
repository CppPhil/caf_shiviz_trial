#pragma once
#include <memory>
#include <string>

#include <opentracing/span.h>

#include "error.hpp"

namespace cst {
std::unique_ptr<opentracing::Span> create_span(
  const tl::expected<std::unique_ptr<opentracing::SpanContext>, error>& ctx,
  const std::string& operation_name);
} // namespace cst
