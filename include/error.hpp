#pragma once
#include <iosfwd>

#include <tl/expected.hpp>

#include "source_location.hpp"

namespace cst {
class error {
public:
  explicit error(std::string error_message);

  [[nodiscard]] const std::string& message() const;

  template <class Exception>
  [[noreturn]] void throw_as() const {
    throw Exception(message());
  }

  friend std::ostream& operator<<(std::ostream& os, const error& e);

private:
  std::string error_message_;
};
} // namespace cst
