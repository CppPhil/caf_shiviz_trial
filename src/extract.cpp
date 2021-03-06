#include <sstream>

#include "extract.hpp"

namespace cst {
namespace {
opentracing::expected<std::unique_ptr<opentracing::SpanContext>>
extract_impl(const opentracing::Tracer* tracer, const std::string& string) {
  std::istringstream iss(string);
  return tracer->Extract(iss);
}
} // namespace

tl::expected<std::unique_ptr<opentracing::SpanContext>, error>
extract(const opentracing::Tracer* tracer, const std::string& string) {
  opentracing::expected<std::unique_ptr<opentracing::SpanContext>> exp(
    extract_impl(tracer, string));

  if (!exp.has_value()) {
    std::ostringstream oss;
    oss << exp.error();
    return CST_UNEXPECTED(oss.str());
  }

  return *std::move(exp);
}
} // namespace cst
