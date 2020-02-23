#pragma once
#include "caf/all.hpp"

namespace cst {
class test_profiler : public caf::actor_profiler {
public:
  void add_actor(const caf::local_actor& self,
                 const caf::local_actor* parent) override;

  void remove_actor(const caf::local_actor& self) override;

  void before_processing(const caf::local_actor& self,
                         const caf::mailbox_element& element) override;

  void after_processing(const caf::local_actor& self,
                        caf::invoke_message_result result) override;

  void before_sending(const caf::local_actor& self,
                      caf::mailbox_element& element) override;

  void before_sending_scheduled(const caf::local_actor& self,
                                caf::actor_clock::time_point timeout,
                                caf::mailbox_element& element) override;
};
} // namespace cst
