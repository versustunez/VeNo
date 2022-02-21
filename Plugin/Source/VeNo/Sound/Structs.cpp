#include <VeNo/Sound/Structs.h>

namespace VeNo::Audio {
WaveHolder::WaveHolder(size_t id) {
  for (auto &generator : generators) {
    generator = CreateScope<WaveTableGenerator>();
    generator->setInstanceId(id);
    generator->init();
  }
}
} // namespace VeNo::Audio