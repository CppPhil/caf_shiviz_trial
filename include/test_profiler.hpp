#pragma once
#include "caf/all.hpp"

namespace cst {
class test_profiler : public caf::actor_profiler {
public:
  virtual void add_actor(const caf::local_actor& self,
                         const caf::local_actor* parent) override;

  virtual void remove_actor(const caf::local_actor& self) override;

  virtual void before_processing(const caf::local_actor& self,
                                 const caf::mailbox_element& element) override;

  virtual void after_processing(const caf::local_actor& self,
                                caf::invoke_message_result result) override;

  virtual void before_sending(const caf::local_actor& self,
                              caf::mailbox_element& element) override;

  virtual void before_sending_scheduled(const caf::local_actor& self,
                                        caf::actor_clock::time_point timeout,
                                        caf::mailbox_element& element) override;
};
} // namespace cst
