#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr double kSecondsPerMinute = 60.0;
}

class $modify(AutoCommentMenuLayer, MenuLayer) {
    double m_autoCommentElapsed = 0.0;

    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        // The scheduler is attached to this menu layer, so it is automatically
        // stopped when the layer is destroyed.
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

        auto levelID = mod->getSettingValue<std::string>("level-id");
        auto comment = mod->getSettingValue<std::string>("comment-text");
        if (levelID.empty() || comment.empty()) {
            m_autoCommentElapsed = 0.0;
            return;
        }

        auto interval = mod->getSettingValue<int64_t>("interval-minutes");
        if (interval < 5) {
            interval = 5;
        }

        m_autoCommentElapsed += dt;
        auto requiredSeconds = static_cast<double>(interval) * kSecondsPerMinute;
        if (m_autoCommentElapsed >= requiredSeconds) {
            // Reset before opening the alert so the reminder cannot fire every frame.
            m_autoCommentElapsed = 0.0;
            this->showPreparedComment();
        }
    }

    void onAutoCommentButton(CCObject*) {
        auto mod = Mod::get();
        auto levelID = mod->getSettingValue<std::string>("level-id");
        auto comment = mod->getSettingValue<std::string>("comment-text");
        auto enabled = mod->getSettingValue<bool>("enabled");
        auto interval = mod->getSettingValue<int64_t>("interval-minutes");

        if (levelID.empty() || comment.empty()) {
            FLAlertLayer::create(
                "AutoComment",
                "Open the AutoComment mod settings and enter a Level ID and comment text first.",
                "OK"
            )->show();
            return;
        }

        if (interval < 5) {
            interval = 5;
        }

        auto status = enabled ? "Reminders are enabled." : "Reminders are currently paused.";
        FLAlertLayer::create(
            "AutoComment",
            fmt::format(
                "{}\n\nLevel ID: {}\nInterval: {} minutes\n\nComment:\n{}",
                status,
                levelID,
                interval,
                comment
            ),
            "Close"
        )->show();
    }

    void showPreparedComment() {
        auto mod = Mod::get();
        auto levelID = mod->getSettingValue<std::string>("level-id");
        auto comment = mod->getSettingValue<std::string>("comment-text");

        FLAlertLayer::create(
            "AutoComment reminder",
            fmt::format(
                "Time for your comment reminder.\n\nLevel ID: {}\n\nPrepared comment:\n{}\n\nOpen the level and submit it yourself if you still want to.",
                levelID,
                comment
            ),
            "OK"
        )->show();
    }
};

$on_mod(Loaded) {
    log::info("AutoComment loaded. Reminder interval has a 5-minute minimum.");
}
