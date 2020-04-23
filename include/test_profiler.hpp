#pragma once
#include "caf/all.hpp"

namespace cst {
void set_span_context(const std::string& span_ctx);

void set_span_context(std::string&& span_ctx);

void set_span_context(const char* span_ctx);

class test_profiler : public caf::actor_profiler {
public:
  test_profiler();

  void add_actor(const caf::local_actor& actor,
                 const caf::local_actor* parent) override;

  void remove_actor(const caf::local_actor& actor) override;

  void before_processing(const caf::local_actor& actor,
                         const caf::mailbox_element& element) override;

  void after_processing(const caf::local_actor& actor,
                        caf::invoke_message_result result) override;

  void before_sending(const caf::local_actor& actor,
                      caf::mailbox_element& element) override;

  void before_sending_scheduled(const caf::local_actor& self,
                                caf::actor_clock::time_point timeout,
                                caf::mailbox_element& element) override;
};
} // namespace cst
