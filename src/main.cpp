#include <algorithm>
#include <cstdio>
#include <string>

#include <caf/all.hpp>

#include "create_span.hpp"
#include "inject.hpp"
#include "test_profiler.hpp"
#include "test_tracing_data_factory.hpp"
#include "tracer/get.hpp"
#include "tracer/put.hpp"
#include "upper.hpp"

namespace {
struct config : caf::actor_system_config {
  cst::test_profiler test_profiler;
  cst::test_tracing_data_factory test_tracing_context;

  config() {
    profiler = &test_profiler;
    tracing_context = &test_tracing_context;
  }
};

caf::tracing_data* tracing_data(caf::scheduled_actor* actor) {
  if (actor == nullptr)
    return nullptr;

  auto* mailbox_element = actor->current_mailbox_element();

  if (mailbox_element == nullptr)
    return nullptr;

  return mailbox_element->tracing_id.get();
}

caf::behavior test_actor_function(caf::event_based_actor* self) {
  cst::tracer::put(self->id());

  return {
    [self](std::string s) {
      auto span = cst::create_span(cst::tracer::get(self->id()),
                                   tracing_data(self),
                                   "test_actor RECV std::string lambda");
      span->SetTag("input", s);
      std::transform(s.begin(), s.end(), s.begin(),
                     [](auto c) { return upper(c); });
      span->SetTag("output", s);
      cst::set_span_context(
        cst::inject(cst::tracer::get(self->id()), span->context())
          .value_or(""));
      return s;
    },
  };
}

void test_actor_buddy_function(caf::event_based_actor* self,
                               const caf::actor& buddy) {
  using namespace std::string_literals;

  cst::tracer::put(self->id());

  auto span = cst::create_span(cst::tracer::get(self->id()), tracing_data(self),
                               "test_actor_buddy_function");
  span->SetTag("info", "about to send HiTheRe to the other actor");

  cst::set_span_context(
    cst::inject(cst::tracer::get(self->id()), span->context()).value_or(""));

  self->request(buddy, caf::infinite, "HiTheRe"s)
    .then([self](const std::string& result_string) {
      auto span = cst::create_span(cst::tracer::get(self->id()),
                                   tracing_data(self), "buddy RECV lambda");

      span->SetTag("result_string", result_string);

      caf::aout(self) << "test_actor_buddy_function: Got \"" << result_string
                      << "\".\n";
    });
}
} // namespace

void caf_main(caf::actor_system& sys, const config& config) {
  auto test_actor = sys.spawn(&test_actor_function);
  sys.spawn(&test_actor_buddy_function, test_actor);
}

CAF_MAIN()
