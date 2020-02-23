#pragma once
#include "caf/all.hpp"
#include "caf/tracing_data_factory.hpp"

#include <memory>

#include "test_tracing_data.hpp"

namespace cst {
class test_tracing_data_factory : public caf::tracing_data_factory {
public:
  caf::error
  deserialize(caf::deserializer& source,
              std::unique_ptr<caf::tracing_data>& dst) const override;

  caf::error_code<caf::sec>
  deserialize(caf::binary_deserializer& source,
              std::unique_ptr<caf::tracing_data>& dst) const override;

private:
  template <class Deserializer>
  typename Deserializer::result_type
  deserialize_impl(Deserializer& source,
                   std::unique_ptr<caf::tracing_data>& dst) const {
    std::string value;
    if (auto err = source(value))
      return err;
    dst = std::make_unique<test_tracing_data>(std::move(value));
    return {};
  }
};
} // namespace cst
