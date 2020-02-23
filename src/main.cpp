#include <algorithm>
#include <iterator>
#include <string>

#include "caf/all.hpp"

#include "upper.hpp"

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

void caf_main(caf::actor_system& sys) {
  auto test_actor = sys.spawn(&test_actor_function);
  sys.spawn(&test_actor_buddy_function, test_actor);
}

CAF_MAIN()
