#pragma once
#include <string>

#include <opentracing/tracer.h>

#include "error.hpp"

namespace cst {
tl::expected<std::string, error> inject(const opentracing::Tracer* tracer,
                                        const opentracing::SpanContext& sc);
} // namespace cst
