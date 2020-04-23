#include <cstdio>

#include <sstream>

#include "inject.hpp"

namespace cst {
tl::expected<std::string, error> inject(const opentracing::Tracer* tracer,
                                        const opentracing::SpanContext& sc) {
  std::ostringstream oss;

  const opentracing::expected<void> exp(tracer->Inject(sc, oss));

  if (!exp.has_value()) {
    oss.clear();
    oss.str("");
    oss << exp.error();
    const auto string = oss.str();
    fprintf(stderr, "inject failure: \"%s\"\n", string.c_str());
    return CST_UNEXPECTED(string);
  }

  return oss.str();
}
} // namespace cst
