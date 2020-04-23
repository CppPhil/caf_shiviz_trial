#include <cstdio>

#include <memory>

#include <jaegertracing/Tracer.h>

#include "create_span.hpp"
#include "inject.hpp"
#include "test_profiler.hpp"
#include "test_tracing_data.hpp"
#include "tracer/get.hpp"

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
  // nop
}

void test_profiler::remove_actor([
  [maybe_unused]] const caf::local_actor& actor) {
  // nop
}

void test_profiler::before_processing(const caf::local_actor& actor,
                                      const caf::mailbox_element& element) {
  auto* tracer = tracer::get(actor.id());

  if (tracer != nullptr) {
    auto span = create_span(tracer, element.tracing_id.get(),
                            "before_processing");

    span->SetTag("actor", actor.name());
    span->SetTag("element", caf::to_string(element.content()));
    span->SetTag("function", PL_CURRENT_FUNCTION);
  }
}

void test_profiler::after_processing(
  [[maybe_unused]] const caf::local_actor& actor,
  [[maybe_unused]] caf::invoke_message_result result) {
  // nop
}

void test_profiler::before_sending(const caf::local_actor& actor,
                                   caf::mailbox_element& element) {
  auto* tracer = tracer::get(actor.id());

  if (tracer == nullptr)
    return;

  if (element.tracing_id == nullptr)
    element.tracing_id = std::make_unique<test_tracing_data>(span_context);

  auto span = create_span(tracer, element.tracing_id.get(), "before_sending");

  span->SetTag("actor", actor.name());
  span->SetTag("element", caf::to_string(element.content()));
  span->SetTag("function", PL_CURRENT_FUNCTION);

  const auto inject_res = inject(tracer, span->context());

  if (!inject_res.has_value()) {
    fprintf(stderr, "%s: inject failed!\n", PL_CURRENT_FUNCTION);
    return;
  }

  element.tracing_id = std::make_unique<test_tracing_data>(*inject_res);
}

void test_profiler::before_sending_scheduled(
  const caf::local_actor& actor, caf::actor_clock::time_point timeout,
  caf::mailbox_element& element) {
  auto* tracer = tracer::get(actor.id());

  if (tracer == nullptr)
    return;

  if (element.tracing_id == nullptr)
    element.tracing_id = std::make_unique<test_tracing_data>(span_context);

  auto span = create_span(tracer, element.tracing_id.get(),
                          "before_sending_scheduled");

  span->SetTag("actor", actor.name());
  span->SetTag("timeout", timeout.time_since_epoch().count());
  span->SetTag("element", caf::to_string(element.content()));
  span->SetTag("function", PL_CURRENT_FUNCTION);

  const auto inject_res = inject(tracer, span->context());

  if (!inject_res.has_value()) {
    fprintf(stderr, "%s: inject failed!\n", PL_CURRENT_FUNCTION);
    return;
  }

  element.tracing_id = std::make_unique<test_tracing_data>(*inject_res);
}
} // namespace cst
