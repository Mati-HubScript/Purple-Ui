#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    // Liquid-glass palette. Alpha is intentionally low so the game remains visible.
    constexpr ccColor4B GLASS_FILL   = { 47, 22, 83, 118 };
    constexpr ccColor4B GLASS_EDGE   = { 220, 190, 255, 150 };
    constexpr ccColor4B GLASS_GLOW   = { 155, 90, 255, 55 };

    class GlassPanel : public CCLayerColor {
    public:
        static GlassPanel* create(CCSize size) {
            auto panel = new GlassPanel();
            if (panel && panel->initWithColor(GLASS_FILL, size.width, size.height)) {
                panel->autorelease();
                panel->setOpacity(220);
                panel->setCascadeOpacityEnabled(true);

                // Soft violet underglow and a thin glass-like outline.
                auto glow = CCLayerColor::create(GLASS_GLOW, size.width + 10.f, size.height + 10.f);
                glow->setPosition({ -5.f, -5.f });
                glow->setZOrder(-1);
                panel->addChild(glow);

                auto outline = CCLayerColor::create(GLASS_EDGE, size.width, 1.f);
                outline->setAnchorPoint({ 0.f, 1.f });
                outline->setPosition({ 0.f, size.height });
                panel->addChild(outline, 2);

                auto bottom = CCLayerColor::create({ 255, 255, 255, 65 }, size.width, 1.f);
                bottom->setAnchorPoint({ 0.f, 0.f });
                panel->addChild(bottom, 2);

                return panel;
            }
            CC_SAFE_DELETE(panel);
            return nullptr;
        }
    };
}

class $modify(PurpleUIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::get()->getWinSize();

        // Main-menu glass dock: visual theme foundation; original GD buttons remain functional.
        auto dock = GlassPanel::create({ 270.f, 58.f });
        if (dock) {
            dock->setAnchorPoint({ 0.5f, 0.f });
            dock->setPosition({ winSize.width / 2.f - 135.f, 10.f });
            dock->setID("purpleui-main-glass-dock");
            this->addChild(dock, 20);
        }

        auto title = CCLabelBMFont::create("PURPLE UI", "goldFont.fnt");
        if (title) {
            title->setScale(0.34f);
            title->setOpacity(220);
            title->setPosition({ winSize.width / 2.f, 39.f });
            title->setID("purpleui-title");
            this->addChild(title, 21);
        }

        return true;
    }
};
