// Game - NWPU C++ sp25
// Created on 2025/5/24
// by konakona418 (https://github.com/konakona418)

#ifndef DIALOGGENERATOR_HPP
#define DIALOGGENERATOR_HPP
#include "ResourceManager.hpp"

namespace game {

class DialogGenerator {
public:
    DialogGenerator() = default;
    ~DialogGenerator() = default;

    DialogGenerator& addSpeaker(const sf::String& name, const sf::Color& color, size_t* outId = nullptr);
    DialogGenerator& addLine(size_t speakerId, const sf::String& text);

    DialogCollection generate();
private:
    DialogCollection m_dialogCollection {};
    size_t m_currentSpeakerId = 0;
};

} // game

#endif //DIALOGGENERATOR_HPP
