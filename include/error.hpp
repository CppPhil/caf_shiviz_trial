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

#define CST_MAKE_ERROR(message)                                                \
  ::cst::error(::std::string(message) + "\nerror occurred at:\n"               \
               + CST_SOURCE_LOCATION)

#define CST_UNEXPECTED(message) ::tl::make_unexpected(CST_MAKE_ERROR(message))
