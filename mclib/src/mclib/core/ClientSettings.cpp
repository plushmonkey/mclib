#include <mclib/core/ClientSettings.h>

namespace mc {
namespace core {

constexpr auto FullSkin =
    (u8)((u8)SkinPart::Cape | (u8)SkinPart::Jacket | (u8)SkinPart::LeftSleeve |
         (u8)SkinPart::RightSleeve | (u8)SkinPart::LeftPants |
         (u8)SkinPart::RightPants | (u8)SkinPart::Hat);

ClientSettings::ClientSettings()
    : m_Locale(L"en_GB"),
      m_ChatMode(ChatMode::Enabled),
      m_MainHand(MainHand::Right),
      m_ViewDistance(16),
      m_SkinParts(FullSkin),
      m_ChatColors(true) {}

ClientSettings& ClientSettings::SetLocale(const std::wstring& locale) {
    m_Locale = locale;
    return *this;
}

ClientSettings& ClientSettings::SetChatMode(ChatMode mode) noexcept {
    m_ChatMode = mode;
    return *this;
}

ClientSettings& ClientSettings::SetMainHand(MainHand main) noexcept {
    m_MainHand = main;
    return *this;
}

ClientSettings& ClientSettings::SetViewDistance(u8 distance) noexcept {
    m_ViewDistance = distance;
    return *this;
}

ClientSettings& ClientSettings::SetViewDistance(s32 distance) noexcept {
    this->SetViewDistance((u8)distance);
    return *this;
}

ClientSettings& ClientSettings::SetSkinParts(u8 parts) noexcept {
    m_SkinParts = parts;
    return *this;
}
ClientSettings& ClientSettings::SetChatColors(bool chatColors) noexcept {
    m_ChatColors = chatColors;
    return *this;
}

ClientSettings& ClientSettings::SetSkinPart(SkinPart part,
                                            bool display) noexcept {
    if (display) {
        m_SkinParts |= (u8)part;
    } else {
        m_SkinParts &= ~(u8)part;
    }
    return *this;
}

}  // namespace core
}  // namespace mc
