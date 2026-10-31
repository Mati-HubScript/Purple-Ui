#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <cmath>
#include <algorithm>

using namespace geode::prelude;

namespace {
    // Draw a genuinely rounded translucent panel using a smooth polygon.
    // It is visual-only: it does not register touch handlers or cover controls.
    class RoundedGlassPanel : public CCNode {
    protected:
        float m_width = 0.f;
        float m_height = 0.f;
        float m_radius = 16.f;
        ccColor4F m_fill = {0.12f, 0.075f, 0.22f, 0.68f};
        ccColor4F m_edge = {0.86f, 0.72f, 1.0f, 0.28f};

        void draw() override {
            CCNode::draw();
            constexpr int segments = 10;
            CCPoint points[segments * 4];
            const float r = std::min(m_radius, std::min(m_width, m_height) * 0.5f);
            const float pi = 3.14159265358979323846f;
            const float centersX[4] = {m_width-r, r, r, m_width-r};
            const float centersY[4] = {m_height-r, m_height-r, r, r};
            const float starts[4] = {0.f, 90.f, 180.f, 270.f};
            int index = 0;
            for (int corner = 0; corner < 4; ++corner) {
                for (int i = 0; i < segments; ++i) {
                    const float angle = (starts[corner] + (90.f * i / (segments - 1))) * pi / 180.f;
                    points[index++] = ccp(centersX[corner] + std::cos(angle) * r,
                                          centersY[corner] + std::sin(angle) * r);
                }
            }
            ccDrawSolidPoly(points, index, m_fill);

            // A restrained inset highlight gives the glass a fine iOS-like edge.
            ccDrawPoly(points, index, true);
        }

    public:
        static RoundedGlassPanel* create(float width, float height, float radius,
                                          ccColor4F fill, ccColor4F edge) {
            auto panel = new RoundedGlassPanel();
            if (panel && panel->init()) {
                panel->m_width = width;
                panel->m_height = height;
                panel->m_radius = radius;
                panel->m_fill = fill;
                panel->m_edge = edge;
                panel->setContentSize({width, height});
                panel->autorelease();
                return panel;
            }
            CC_SAFE_DELETE(panel);
            return nullptr;
        }
    };

    void addLabel(CCNode* parent, const char* text, const char* font,
                  float scale, CCPoint position, ccColor3B color = {245, 239, 255}) {
        auto label = CCLabelBMFont::create(text, font);
        if (!label) return;
        label->setScale(scale);
        label->setPosition(position);
        label->setColor(color);
        label->setOpacity(240);
        parent->addChild(label, 5);
    }
}

class $modify(PurpleUIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        const auto size = CCDirector::get()->getWinSize();
        const float w = size.width;
        const float h = size.height;

        // Soft purple veil; low opacity keeps the original GD background visible.
        auto tint = CCLayerColor::create({20, 10, 42, 42}, w, h);
        tint->setAnchorPoint({0.f, 0.f});
        tint->setPosition({0.f, 0.f});
        tint->setID("purpleui-background-tint");
        this->addChild(tint, 1);

        // Minimal floating title capsule.
        const float headerW = std::min(w - 34.f, 310.f);
        auto header = RoundedGlassPanel::create(headerW, 54.f, 22.f,
            {0.16f, 0.10f, 0.28f, 0.78f}, {0.90f, 0.79f, 1.0f, 0.42f});
        if (header) {
            header->setPosition({(w - headerW) * 0.5f, h - 76.f});
            header->setID("purpleui-header-glass");
            this->addChild(header, 30);
            addLabel(header, "PURPLE UI", "goldFont.fnt", 0.38f,
                     {headerW * 0.5f, 34.f}, {250, 246, 255});
            addLabel(header, "LIQUID GLASS", "bigFont.fnt", 0.19f,
                     {headerW * 0.5f, 17.f}, {202, 180, 235});
        }

        // A subtle rounded glass surface behind the original menu area.
        // Kept very transparent so the original icons remain clear.
        const float panelW = std::min(w - 44.f, 350.f);
        auto center = RoundedGlassPanel::create(panelW, 184.f, 28.f,
            {0.13f, 0.075f, 0.23f, 0.38f}, {0.88f, 0.75f, 1.0f, 0.17f});
        if (center) {
            center->setPosition({(w - panelW) * 0.5f, h * 0.5f - 91.f});
            center->setID("purpleui-main-glass-surface");
            this->addChild(center, 2);
        }

        // Compact floating bottom capsule, like an iOS status/dock element.
        const float footerW = std::min(w - 54.f, 224.f);
        auto footer = RoundedGlassPanel::create(footerW, 34.f, 17.f,
            {0.15f, 0.09f, 0.27f, 0.76f}, {0.92f, 0.82f, 1.0f, 0.34f});
        if (footer) {
            footer->setPosition({(w - footerW) * 0.5f, 16.f});
            footer->setID("purpleui-footer-glass");
            this->addChild(footer, 30);
            addLabel(footer, "A NEW WAY TO PLAY", "bigFont.fnt", 0.22f,
                     {footerW * 0.5f, 17.f}, {224, 211, 246});
        }
        return true;
    }
};
