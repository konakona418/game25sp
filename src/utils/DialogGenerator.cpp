// Game - NWPU C++ sp25
// Created on 2025/5/24
// by konakona418 (https://github.com/konakona418)

#include "DialogGenerator.hpp"

namespace game {
    DialogGenerator& DialogGenerator::addSpeaker(const sf::String& name, const sf::Color& color, size_t* outId) {
        if (outId != nullptr) {
            *outId = m_currentSpeakerId;
        }

        m_dialogCollection.speakers.insert({m_currentSpeakerId++, DialogSpeaker{name, color}});
        return *this;
    }

    DialogGenerator& DialogGenerator::addLine(size_t speakerId, const sf::String& text) {
        m_dialogCollection.lines.push_back(DialogLine{text, speakerId});
        return *this;
    }

    DialogCollection DialogGenerator::generate() {
        return m_dialogCollection;
    }
} // game