#include <cstdarg>
#include <cstdio>
#include <memory>

#include "test_profiler.hpp"
#include "test_tracing_data.hpp"

namespace cst {
namespace {
int vasprintf(std::string* strp, const char* fmt, std::va_list ap) noexcept {
  std::va_list args2;
  va_copy(args2, ap);

  const auto err_code = std::vsnprintf(nullptr, 0, fmt, ap);

  if (err_code < 0)
    return err_code;

  const auto byte_count = 1U + err_code;

  strp->resize(byte_count);
  const auto ret_val = std::vsnprintf(strp->data(), byte_count, fmt, args2);

  va_end(args2);
  strp->resize(byte_count - 1);

  return ret_val;
}

int asprintf(std::string* strp, const char* fmt, ...) noexcept {
  std::va_list args1;
  va_start(args1, fmt);

  const auto ret_val = vasprintf(strp, fmt, args1);
  va_end(args1);

  return ret_val;
}
} // namespace

#define P(...)                                                                 \
  do {                                                                         \
    std::string s;                                                             \
    asprintf(&s, "%s: ", __PRETTY_FUNCTION__);                                 \
    std::string s2;                                                            \
    asprintf(&s2, __VA_ARGS__);                                                \
    std::printf("%s\n", (s + s2).data());                                      \
  } while ((void) 0, 0)

void test_profiler::add_actor([[maybe_unused]] const caf::local_actor& self,
                              const caf::local_actor* parent) {
  P("parent: %s", parent);
}

void test_profiler::remove_actor([
  [maybe_unused]] const caf::local_actor& self) {
  P("");
}

void test_profiler::before_processing(
  [[maybe_unused]] const caf::local_actor& self,
  const caf::mailbox_element& element) {
  P("%s", element.content().stringify().data());
}

void test_profiler::after_processing(
  [[maybe_unused]] const caf::local_actor& self,
  caf::invoke_message_result result) {
  P("%s", caf::to_string(result).data());
}

void test_profiler::before_sending(const caf::local_actor& self,
                                   caf::mailbox_element& element) {
  P("%s", element.content().stringify().data());

  element.tracing_id = std::make_unique<test_tracing_data>(self.name());
}

void test_profiler::before_sending_scheduled(
  const caf::local_actor& self,
  [[maybe_unused]] caf::actor_clock::time_point timeout,
  caf::mailbox_element& element) {
  P("%s", element.content().stringify().data());

  element.tracing_id = std::make_unique<test_tracing_data>(self.name());
}
} // namespace cst
