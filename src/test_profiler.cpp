#include <cstdio>

#include <memory>

#include <jaegertracing/Tracer.h>

#include "create_span.hpp"
#include "inject.hpp"
#include "test_profiler.hpp"
#include "test_tracing_data.hpp"

namespace cst {
namespace {
thread_local std::string span_context;
} // namespace

void set_span_context(const std::string& span_ctx) {
  span_context = span_ctx;
}

void set_span_context(std::string&& span_ctx) {
  span_context = std::move(span_ctx);
}

void set_span_context(const char* span_ctx) {
  span_context = span_ctx;
}

test_profiler::test_profiler() = default;

void test_profiler::add_actor([[maybe_unused]] const caf::local_actor& self,
                              [[maybe_unused]] const caf::local_actor* parent) {
#ifdef VERBOSE_SPANS
  auto span = opentracing::Tracer::Global()->StartSpan("add_actor");

  span->SetTag("actor", self.name());
  span->SetTag("parent", parent == nullptr ? "null" : parent->name());
  span->SetTag("function", PL_CURRENT_FUNCTION);
#endif // VERBOSE_SPANS
}

void test_profiler::remove_actor([
  [maybe_unused]] const caf::local_actor& actor) {
#ifdef VERBOSE_SPANS
  auto span = opentracing::Tracer::Global()->StartSpan("remove_actor");

  span->SetTag("actor", actor.name());
  span->SetTag("function", PL_CURRENT_FUNCTION);
#endif // VERBOSE_SPANS
}

void test_profiler::before_processing(const caf::local_actor& actor,
                                      const caf::mailbox_element& element) {
  auto span = create_span(element.tracing_id.get(), "before_processing");

  span->SetTag("actor", actor.name());
  span->SetTag("element", caf::to_string(element.content()));
  span->SetTag("function", PL_CURRENT_FUNCTION);
}

void test_profiler::after_processing(
  [[maybe_unused]] const caf::local_actor& actor,
  [[maybe_unused]] caf::invoke_message_result result) {
#ifdef VERBOSE_SPANS
  auto span = opentracing::Tracer::Global()->StartSpan("after_processing");

  span->SetTag("actor", actor.name());
  span->SetTag("result", caf::to_string(result));
  span->SetTag("function", PL_CURRENT_FUNCTION);
#endif // VERBOSE_SPANS
}

void test_profiler::before_sending(const caf::local_actor& actor,
                                   caf::mailbox_element& element) {
  if (element.tracing_id == nullptr)
    element.tracing_id = std::make_unique<test_tracing_data>(span_context);

  auto span = create_span(element.tracing_id.get(), "before_sending");

  span->SetTag("actor", actor.name());
  span->SetTag("element", caf::to_string(element.content()));
  span->SetTag("function", PL_CURRENT_FUNCTION);

  const auto inject_res = inject(span->context());

  if (!inject_res.has_value()) {
    fprintf(stderr, "%s: inject failed!\n", PL_CURRENT_FUNCTION);
    return;
  }

  element.tracing_id = std::make_unique<test_tracing_data>(*inject_res);
}

void test_profiler::before_sending_scheduled(
  const caf::local_actor& actor, caf::actor_clock::time_point timeout,
  caf::mailbox_element& element) {
  if (element.tracing_id == nullptr)
    element.tracing_id = std::make_unique<test_tracing_data>(span_context);

  auto span = create_span(element.tracing_id.get(), "before_sending_scheduled");

  span->SetTag("actor", actor.name());
  span->SetTag("timeout", timeout.time_since_epoch().count());
  span->SetTag("element", caf::to_string(element.content()));
  span->SetTag("function", PL_CURRENT_FUNCTION);

  const auto inject_res = inject(span->context());

  if (!inject_res.has_value()) {
    fprintf(stderr, "%s: inject failed!\n", PL_CURRENT_FUNCTION);
    return;
  }

  element.tracing_id = std::make_unique<test_tracing_data>(*inject_res);
}
} // namespace cst
