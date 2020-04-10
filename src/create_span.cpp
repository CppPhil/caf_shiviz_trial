#include <cstdio>

#include <jaegertracing/Tracer.h>

#include "create_span.hpp"
#include "extract.hpp"
#include "test_tracing_data.hpp"

namespace cst {
namespace {
std::unique_ptr<opentracing::Span> create_span_impl(
  const tl::expected<std::unique_ptr<opentracing::SpanContext>, error>& ctx,
  const std::string& operation_name) {
  if (!ctx.has_value() || *ctx == nullptr) {
    fprintf(stderr, "Span with operation name \"%s\" has no parent!\n",
            operation_name.c_str());
    return opentracing::Tracer::Global()->StartSpan(operation_name);
  }

  printf("Span with operation name \"%s\" is a child span!\n",
         operation_name.c_str());
  return opentracing::Tracer::Global()->StartSpan(
    operation_name, {opentracing::ChildOf(ctx->get())});
}
} // namespace

std::unique_ptr<opentracing::Span>
create_span(const caf::tracing_data* tracing_data,
            const std::string& operation_name) {
  if (tracing_data == nullptr)
    return opentracing::Tracer::Global()->StartSpan(operation_name);

  const auto* p = dynamic_cast<const test_tracing_data*>(tracing_data);

  if (p == nullptr)
    return opentracing::Tracer::Global()->StartSpan(operation_name);

  const auto extract_res = extract(p->value);
  return create_span_impl(extract_res, operation_name);
}
} // namespace cst
