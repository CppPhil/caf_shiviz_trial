#pragma once
#include <memory>
#include <string>

#include <tl/expected.hpp>

#include <opentracing/tracer.h>

#include "error.hpp"

namespace cst {

tl::expected<std::unique_ptr<opentracing::SpanContext>, error>
extract(const std::string& string);
} // namespace cst
