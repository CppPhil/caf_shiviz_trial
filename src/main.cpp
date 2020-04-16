#include <algorithm>
#include <cstdio>
#include <string>

#include <caf/all.hpp>

#include "args.hpp"
#include "create_span.hpp"
#include "setup_tracer.hpp"
#include "test_profiler.hpp"
#include "test_tracing_data_factory.hpp"
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

/* scheduled_actor::peek_at_next_mailbox_element() -> mailbox_element*
 *
 *
 * scheduled_actor::mailbox() -> intrusive::fifo_inbox<mailbox_policy>&
 * mailbox_policy::mapped_type = mailbox_element;
 * intrusive::fifo_inbox<mailbox_policy>::peek() -> mailbox_element*;
 *
 * tracing_data_ptr mailbox_element::tracing_id;
 * using tracing_data_ptr = std::unique_ptr<tracing_data>;
 */

caf::tracing_data* tracing_data(caf::scheduled_actor* actor) {
  if (actor == nullptr)
    return nullptr;

  auto* mailbox_element = actor->peek_at_next_mailbox_element();

  if (mailbox_element == nullptr)
    return nullptr;

  return mailbox_element->tracing_id.get();
}

caf::behavior test_actor_function(caf::event_based_actor* self) {
  return {
    [self](std::string s) {
      //      auto span = cst::create_span(tracing_data(self),
      //                                   "RECV std::string lambda");
      //      span->SetTag("input", s);

      std::transform(s.begin(), s.end(), s.begin(),
                     [](auto c) { return upper(c); });

      //      span->SetTag("output", s);

      return s;
    },
  };
}

void test_actor_buddy_function(caf::event_based_actor* self,
                               const caf::actor& buddy) {
  using namespace std::string_literals;
  self->request(buddy, caf::infinite, "HiTheRe"s)
    .then([self](const std::string& result_string) {
      caf::aout(self) << "test_actor_buddy_function: Got \"" << result_string
                      << "\".\n";
    });
}
} // namespace

void caf_main(caf::actor_system& sys, const config& config) {
  auto test_actor = sys.spawn(&test_actor_function);
  sys.spawn(&test_actor_buddy_function, test_actor);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr,
            "No YAML config file was passed as a command line argument!\n");
    return 1;
  }

  cst::setup_tracer(argv[1]);

  static cst::args args(argc, argv, [](int i) { return i != 1; });

  caf::exec_main_init_meta_objects<>();
  caf::core::init_global_meta_objects();
  return ::caf::exec_main<>(caf_main, args.argc(), args.argv());
}
