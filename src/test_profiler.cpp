#include <cstdarg>
#include <cstdio>

#include <memory>

#include <jaegertracing/Tracer.h>

#include "test_profiler.hpp"
#include "test_tracing_data.hpp"

namespace cst {
test_profiler::test_profiler() {
  setbuf(stdout, nullptr);
}

void test_profiler::add_actor(const caf::local_actor& self,
                              const caf::local_actor* parent) {
  printf("add_actor: actor = \"%s\", parent = \"%s\"\n", self.name(),
         parent != nullptr ? parent->name() : "null");

  auto span = opentracing::Tracer::Global()->StartSpan("add_actor");

  span->SetTag("actor", self.name());
  span->SetTag("parent", parent == nullptr ? "null" : parent->name());
}

void test_profiler::remove_actor(const caf::local_actor& actor) {
  printf("remove_actor: actor = \"%s\"\n", actor.name());

  auto span = opentracing::Tracer::Global()->StartSpan("remove_actor");

  span->SetTag("actor", actor.name());
}

void test_profiler::before_processing(const caf::local_actor& actor,
                                      const caf::mailbox_element& element) {
  printf("before_processing: actor = \"%s\", element: \"%s\"\n", actor.name(),
         element.content().stringify().c_str());

  auto span = opentracing::Tracer::Global()->StartSpan("before_precessing");

  span->SetTag("actor", actor.name());
  span->SetTag("element", element.content().stringify());
}

void test_profiler::after_processing(const caf::local_actor& actor,
                                     caf::invoke_message_result result) {
  printf("after_processing: actor = \"%s\", result: \"%s\"\n", actor.name(),
         caf::to_string(result).c_str());

  auto span = opentracing::Tracer::Global()->StartSpan("after_processing");

  span->SetTag("actor", actor.name());
  span->SetTag("result", caf::to_string(result));
}

void test_profiler::before_sending(const caf::local_actor& actor,
                                   caf::mailbox_element& element) {
  printf("before_sending: actor = \"%s\", element: \"%s\"\n", actor.name(),
         element.content().stringify().c_str());

  element.tracing_id = std::make_unique<test_tracing_data>(actor.name());

  auto span = opentracing::Tracer::Global()->StartSpan("before_sending");

  span->SetTag("actor", actor.name());
  span->SetTag("element", element.content().stringify());
}

void test_profiler::before_sending_scheduled(
  const caf::local_actor& actor, caf::actor_clock::time_point timeout,
  caf::mailbox_element& element) {
  printf("before_sending_scheduled: actor = \"%s\", element: \"%s\"\n",
         actor.name(), element.content().stringify().c_str());

  element.tracing_id = std::make_unique<test_tracing_data>(actor.name());

  auto span
    = opentracing::Tracer::Global()->StartSpan("before_sending_scheduled");

  span->SetTag("actor", actor.name());
  span->SetTag("timeout", timeout.time_since_epoch().count());
  span->SetTag("element", element.content().stringify());
}
} // namespace cst
