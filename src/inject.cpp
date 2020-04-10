#include <sstream>

#include "inject.hpp"

namespace cst {
tl::expected<std::string, error> inject(const opentracing::SpanContext& sc) {
  std::ostringstream oss;

  const opentracing::expected<void> exp(
    opentracing::Tracer::Global()->Inject(sc, oss));

  if (!exp.has_value()) {
    oss.clear();
    oss.str("");

    oss << exp.error();

    return CST_UNEXPECTED(oss.str());
  }

  return oss.str();
}
} // namespace cst
