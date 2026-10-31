#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr ccColor3B PURPLE = { 174, 112, 255 };
    constexpr ccColor3B TEXT = { 239, 232, 255 };
    constexpr ccColor3B MUTED = { 166, 151, 190 };
}

class PurpleOverlay final : public CCLayerColor {
protected:
    CCLabelBMFont* m_pageTitle = nullptr;
    CCLabelBMFont* m_pageDescription = nullptr;
    CCLayerColor* m_panel = nullptr;
    bool m_closing = false;

    CCMenuItemLabel* makeButton(
        const char* title,
        SEL_MenuHandler callback,
        CCNode* target
    ) {
        auto label = CCLabelBMFont::create(title, "bigFont.fnt");
        label->setScale(0.42f);
        label->setColor(TEXT);

        auto item = CCMenuItemLabel::create(label, target, callback);
        item->setScale(0.92f);
        item->runAction(CCRepeatForever::create(CCSequence::create(
            CCScaleTo::create(0.85f, 0.96f),
            CCScaleTo::create(0.85f, 0.92f),
            nullptr
        )));
        return item;
    }

    void setPage(const char* title, const char* description) {
        if (!m_pageTitle || !m_pageDescription) return;

        m_pageTitle->runAction(CCSequence::create(
            CCFadeOut::create(0.10f),
            CCCallFunc::create(this, callfunc_selector(PurpleOverlay::refreshPageText)),
            nullptr
        ));

        m_pageTitle->setString(title);
        m_pageDescription->setString(description);
        m_pageTitle->setOpacity(0);
        m_pageDescription->setOpacity(0);
        m_pageTitle->runAction(CCFadeIn::create(0.22f));
        m_pageDescription->runAction(CCFadeIn::create(0.28f));
    }

    void refreshPageText() {}

public:
    static PurpleOverlay* create() {
        auto layer = new PurpleOverlay();
        if (layer && layer->initWithColor(ccc4(8, 6, 15, 238))) {
            layer->autorelease();
            return layer;
        }
        CC_SAFE_DELETE(layer);
        return nullptr;
    }

    bool initWithColor(const ccColor4B& color) override {
        if (!CCLayerColor::initWithColor(color)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        this->setOpacity(0);

        // Main card, sized relative to the game window.
        const float panelWidth = winSize.width * 0.82f;
        const float panelHeight = winSize.height * 0.76f;

        m_panel = CCLayerColor::create(
            ccc4(25, 19, 39, 250),
            panelWidth,
            panelHeight
        );
        m_panel->setPosition(
            (winSize.width - panelWidth) / 2.0f,
            (winSize.height - panelHeight) / 2.0f
        );
        m_panel->setOpacity(0);
        m_panel->setScale(0.96f);
        this->addChild(m_panel, 1);

        // Accent strip.
        auto accent = CCLayerColor::create(
            ccc4(PURPLE.r, PURPLE.g, PURPLE.b, 255),
            panelWidth,
            3.0f
        );
        accent->setPosition(0, panelHeight - 3.0f);
        m_panel->addChild(accent, 2);

        auto title = CCLabelBMFont::create("PURPLE UI", "bigFont.fnt");
        title->setColor(PURPLE);
        title->setScale(0.78f);
        title->setPosition(
            winSize.width / 2.0f,
            winSize.height * 0.78f
        );
        this->addChild(title, 3);

        m_pageTitle = CCLabelBMFont::create("HOME", "bigFont.fnt");
        m_pageTitle->setColor(TEXT);
        m_pageTitle->setScale(0.58f);
        m_pageTitle->setPosition(
            winSize.width / 2.0f,
            winSize.height * 0.61f
        );
        this->addChild(m_pageTitle, 3);

        m_pageDescription = CCLabelBMFont::create(
            "Welcome to your new Geometry Dash interface.",
            "goldFont.fnt"
        );
        m_pageDescription->setColor(MUTED);
        m_pageDescription->setScale(0.42f);
        m_pageDescription->setPosition(
            winSize.width / 2.0f,
            winSize.height * 0.51f
        );
        this->addChild(m_pageDescription, 3);

        auto home = makeButton(
            "HOME", menu_selector(PurpleOverlay::onHome), this
        );
        auto visual = makeButton(
            "VISUAL", menu_selector(PurpleOverlay::onVisual), this
        );
        auto about = makeButton(
            "ABOUT", menu_selector(PurpleOverlay::onAbout), this
        );

        auto tabs = CCMenu::create(home, visual, about, nullptr);
        tabs->setPosition(CCPointZero);
        home->setPosition(winSize.width * 0.37f, winSize.height * 0.37f);
        visual->setPosition(winSize.width * 0.50f, winSize.height * 0.37f);
        about->setPosition(winSize.width * 0.63f, winSize.height * 0.37f);
        this->addChild(tabs, 4);

        auto closeLabel = CCLabelBMFont::create("X", "bigFont.fnt");
        closeLabel->setColor(PURPLE);
        closeLabel->setScale(0.55f);
        auto closeItem = CCMenuItemLabel::create(
            closeLabel, this, menu_selector(PurpleOverlay::onClose)
        );
        auto closeMenu = CCMenu::create(closeItem, nullptr);
        closeMenu->setPosition(CCPointZero);
        closeItem->setPosition(
            winSize.width * 0.87f,
            winSize.height * 0.78f
        );
        this->addChild(closeMenu, 5);

        this->runAction(CCFadeTo::create(0.22f, 255));
        m_panel->runAction(CCSpawn::create(
            CCFadeTo::create(0.25f, 255),
            CCScaleTo::create(0.25f, 1.0f),
            nullptr
        ));

        return true;
    }

    void onHome(CCObject*) {
        setPage("HOME", "Welcome to your new Geometry Dash interface.");
    }

    void onVisual(CCObject*) {
        setPage("VISUAL", "Visual options will be added in the next update.");
    }

    void onAbout(CCObject*) {
        setPage("ABOUT", "PurpleUI v1.0.0  |  Made with Geode.");
    }

    void onClose(CCObject*) {
        if (m_closing) return;
        m_closing = true;

        this->runAction(CCSequence::create(
            CCFadeOut::create(0.18f),
            CCCallFunc::create(this, callfunc_selector(PurpleOverlay::removeOverlay)),
            nullptr
        ));
        m_panel->runAction(CCScaleTo::create(0.18f, 0.96f));
    }

    void removeOverlay() {
        this->removeFromParentAndCleanup(true);
    }
};

class $modify(PurpleMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto label = CCLabelBMFont::create("PURPLE", "bigFont.fnt");
        label->setColor(PURPLE);
        label->setScale(0.42f);

        auto item = CCMenuItemLabel::create(
            label, this, menu_selector(PurpleMenuLayer::openPurpleUI)
        );

        auto menu = CCMenu::create(item, nullptr);
        menu->setPosition(CCPointZero);
        item->setPosition(winSize.width - 58.0f, winSize.height - 28.0f);
        this->addChild(menu, 1000);

        return true;
    }

    void openPurpleUI(CCObject*) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;

        if (scene->getChildByID("purpleui-overlay")) return;

        auto overlay = PurpleOverlay::create();
        if (overlay) {
            overlay->setID("purpleui-overlay");
            scene->addChild(overlay, 9999);
        }
    }
};
