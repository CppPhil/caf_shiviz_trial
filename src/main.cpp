#include <algorithm>
#include <cstdio>
#include <string>

#include <caf/all.hpp>

#include "args.hpp"
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

caf::behavior test_actor_function() {
  return {
    [](std::string s) {
      std::transform(s.begin(), s.end(), s.begin(),
                     [](auto c) { return upper(c); });
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
