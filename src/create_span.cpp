#include <jaegertracing/Tracer.h>

#include "create_span.hpp"

namespace cst {
std::unique_ptr<opentracing::Span> create_span(
  const tl::expected<std::unique_ptr<opentracing::SpanContext>, error>& ctx,
  const std::string& operation_name) {
  return (!ctx.has_value() || *ctx == nullptr)
           ? opentracing::Tracer::Global()->StartSpan(operation_name)
           : opentracing::Tracer::Global()->StartSpan(
             operation_name, {opentracing::ChildOf(ctx->get())});
}
} // namespace cst
