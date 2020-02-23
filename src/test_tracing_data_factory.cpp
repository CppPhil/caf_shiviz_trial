#include "test_tracing_data_factory.hpp"

namespace cst {
test_tracing_data_factory::test_tracing_data_factory()
  : caf::tracing_data_factory() {
}

test_tracing_data_factory::~test_tracing_data_factory() = default;

caf::error test_tracing_data_factory::deserialize(
  caf::deserializer& source, std::unique_ptr<caf::tracing_data>& dst) const {
  return deserialize_impl(source, dst);
}

caf::error_code<caf::sec> test_tracing_data_factory::deserialize(
  caf::binary_deserializer& source,
  std::unique_ptr<caf::tracing_data>& dst) const {
  return deserialize_impl(source, dst);
}
} // namespace cst
