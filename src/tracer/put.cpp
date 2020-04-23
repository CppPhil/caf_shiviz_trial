#include <unordered_map>

#include <caf/all.hpp>

#include <jaegertracing/Tracer.h>

#include <pl/thd/monitor.hpp>

#include "create_tracer.hpp"
#include "tracer/put.hpp"

namespace cst::tracer {
pl::thd::monitor<
  std::unordered_map<caf::actor_id, std::shared_ptr<opentracing::Tracer>>>
  tracers({});

void put(caf::actor_id aid) {
  tracers([aid](auto& map) {
    map.try_emplace(aid, create_tracer("config.yml", "caf_shiviz_trial actor"
                                                       + std::to_string(aid)));
  });
}
} // namespace cst::tracer
