#include <VeNo/Sound/Synth/Matrix.h>

namespace VeNo::Audio {

Matrix::Matrix(InstanceID instance_id)
    : m_id(instance_id),
      m_modulatorHandle(instance_id) {
  m_modulatorHandle.init(this);
}
Matrix::~Matrix() {
  m_modulators.clear();
  m_items.clear();
}
void Matrix::update() {
  // Matrix is empty... don't update anything because we would waste cycles
  if (m_items.empty())
    return;

  for (auto &modulator : m_modulators) {
    modulator->update();
  }

  if (!m_deletedItems.empty()) {
    for (auto deletedItem : m_deletedItems)
      m_items.erase(m_items.begin() + (long)deletedItem);
    m_deletedItems.clear();
  }

  // @TODO: Add Modulator -> Modulation
/*  for (auto &item : m_items) {
    if(item.src->isVoiceModulator()) {
      for (int i = 0; i < MAX_VOICES; ++i) {
        item.dst->addValueVoice(i, item.src->value(i));
      }
    } else {
      item.dst->addValue(item.src->value(-1));
    };
  }*/
}
void Matrix::remove(const VString &name, const VString &dst) {
  std::string key = name + dst;
  auto it = m_indexLookup.find(key);
  if (it == m_indexLookup.end())
    return;
  m_deletedItems.push_back(it->second);
  m_indexLookup.erase(it);
}
} // namespace VeNo::Audio