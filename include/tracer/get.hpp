#pragma once
#include <opentracing/tracer.h>

#include <caf/all.hpp>

namespace cst::tracer {
const opentracing::Tracer* get(caf::actor_id aid);
} // namespace cst::tracer
