#include <VUtils/Logging.h>
#include <VeNo/Core/Instance.h>

namespace VeNo::Core {
std::vector<Instance *> Instance::m_instances = {};
Instance::~Instance() { handler = nullptr; }
Instance *Instance::get(size_t m_id) {
  // This should never be a problem tbh!
  return m_instances[m_id];
}
Instance *Instance::create() {
  auto *instance = new Instance();
  // if m_instances is not empty we search for an empty slot... this happens on
  // start-time so no problem here :D
  if (!m_instances.empty()) {
    for (size_t i = 0; i < m_instances.size(); i++) {
      if (m_instances[i] == nullptr) {
        DBGN("Reuse Slot: %d", i);
        instance->id = i;
        instance->init();
        m_instances[i] = instance;
        return instance;
      }
    }
  }
  m_instances.push_back(instance);
  instance->id = m_instances.size() - 1;
  instance->init();
  DBGN("New Spot: %d", instance->id);
  return instance;
}

void Instance::remove(size_t m_id) {
  if (m_instances[m_id] != nullptr) {
    DBGN("Instance: %d", m_id);
    delete m_instances[m_id];
    m_instances[m_id] = nullptr;
  }
  bool isEmpty = true;
  for (auto &m_instance : m_instances) {
    if (m_instance != nullptr) {
      isEmpty = false;
      break;
    }
  }
  DBGN("Clear Instances? %s", isEmpty ? "Yes" : "No");
  if (isEmpty)
    m_instances.clear();
}
void Instance::init() {
  handler = CreateScope<ParameterHandler>(id);
  mainInterpreter = CreateRef<GUI::Interpreter>(id);
  state.m_id = id;
}

} // namespace VeNo::Core