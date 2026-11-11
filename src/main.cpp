#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr double kSecondsPerMinute = 60.0;
}

class $modify(AutoCommentMenuLayer, MenuLayer) {
    double m_autoCommentElapsed = 0.0;
    bool m_autoCommentBusy = false;

    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        this->scheduleUpdate();

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            if (sprite) {
                auto button = CCMenuItemSpriteExtra::create(
                    sprite,
                    this,
                    menu_selector(AutoCommentMenuLayer::onAutoCommentButton)
                );
                if (button) {
                    button->setID("autocomment-open"_spr);
                    menu->addChild(button);
                    menu->updateLayout();
                }
            }
        }
        return true;
    }

    void update(float dt) {
        MenuLayer::update(dt);

        auto mod = Mod::get();
        if (!mod->getSettingValue<bool>("enabled")) {
            m_autoCommentElapsed = 0.0;
            return;
        }

        auto rawID = mod->getSettingValue<std::string>("level-id");
        auto comment = mod->getSettingValue<std::string>("comment-text");
        if (rawID.empty() || comment.empty() || m_autoCommentBusy) {
            return;
        }

        int levelID = 0;
        try {
            levelID = std::stoi(rawID);
        } catch (...) {
            return;
        }
        if (levelID <= 0) {
            return;
        }

        auto interval = mod->getSettingValue<int64_t>("interval-minutes");
        if (interval < 5) interval = 5;

        m_autoCommentElapsed += dt;
        if (m_autoCommentElapsed < static_cast<double>(interval) * kSecondsPerMinute) {
            return;
        }

        // Reset the timer before sending to prevent frame-by-frame retries.
        m_autoCommentElapsed = 0.0;
        m_autoCommentBusy = true;
        GameLevelManager::sharedState()->uploadComment(
            comment,
            CommentType::Level,
            levelID,
            0
        );
        m_autoCommentBusy = false;
        log::info("AutoComment submitted a comment for level {}", levelID);
    }

    void onAutoCommentButton(CCObject*) {
        auto mod = Mod::get();
        auto enabled = mod->getSettingValue<bool>("enabled");
        auto rawID = mod->getSettingValue<std::string>("level-id");
        auto comment = mod->getSettingValue<std::string>("comment-text");
        auto interval = mod->getSettingValue<int64_t>("interval-minutes");
        if (interval < 5) interval = 5;

        FLAlertLayer::create(
            "AutoComment",
            fmt::format(
                "Automatic posting: {}\nLevel ID: {}\nInterval: {} minutes\n\nComment:\n{}",
                enabled ? "ON" : "OFF",
                rawID,
                interval,
                comment
            ),
            "Close"
        )->show();
    }
};

$on_mod(Loaded) {
    log::info("AutoComment loaded. Automatic comment interval minimum: 5 minutes.");
}
