#include <Geode/Geode.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <algorithm>
#include <charconv>
#include <cctype>
#include <string>
#include <system_error>

using namespace geode::prelude;

namespace {
    constexpr double kSecondsPerMinute = 60.0;
    constexpr double kRequestTimeoutSeconds = 90.0;

    // Kept outside MenuLayer so the scheduler survives scene changes.
    double g_elapsedSeconds = 0.0;
    double g_pendingSeconds = 0.0;
    bool g_requestPending = false;
    bool g_firstAttempt = true;
    bool g_wasEnabled = false;
    int g_lastLevelID = 0;
    int64_t g_lastIntervalMinutes = 5;
    std::string g_lastComment;

    bool parseLevelID(std::string const& raw, int& out) {
        if (raw.empty()) return false;

        int parsed = 0;
        auto const* begin = raw.data();
        auto const* end = begin + raw.size();
        auto result = std::from_chars(begin, end, parsed);

        if (result.ec != std::errc{} || result.ptr != end || parsed <= 0) {
            return false;
        }

        out = parsed;
        return true;
    }

    std::string trim(std::string value) {
        auto notSpace = [](unsigned char c) { return !std::isspace(c); };
        auto first = std::find_if(value.begin(), value.end(), notSpace);
        auto last = std::find_if(value.rbegin(), value.rend(), notSpace).base();

        if (first >= last) return {};
        return std::string(first, last);
    }

    void resetTimer() {
        g_elapsedSeconds = 0.0;
    }
}

// GameLevelManager is a long-lived game singleton, unlike MenuLayer.
// Scheduling on it keeps the timer active while changing menus/scenes.
class $modify(AutoCommentGameLevelManager, GameLevelManager) {
    void update(float dt) {
        GameLevelManager::update(dt);

        auto mod = Mod::get();
        auto enabled = mod->getSettingValue<bool>("enabled");
        if (!enabled) {
            // Do not discard an in-flight request when the user disables the mod.
            // It may still complete on the game server.
            if (g_wasEnabled) {
                resetTimer();
                g_firstAttempt = true;
            }
            g_wasEnabled = false;
            return;
        }
        if (!g_wasEnabled) {
            resetTimer();
            g_firstAttempt = true;
        }
        g_wasEnabled = true;

        auto rawID = mod->getSettingValue<std::string>("level-id");
        auto comment = trim(mod->getSettingValue<std::string>("comment-text"));

        int levelID = 0;
        if (!parseLevelID(rawID, levelID) || comment.empty()) {
            resetTimer();
            return;
        }

        // A changed target/comment starts a fresh interval.
        auto interval = std::clamp<int64_t>(
            mod->getSettingValue<int64_t>("interval-minutes"), 5, 1440
        );

        if (levelID != g_lastLevelID || comment != g_lastComment ||
            interval != g_lastIntervalMinutes) {
            resetTimer();
            g_firstAttempt = true;
            g_lastLevelID = levelID;
            g_lastComment = comment;
            g_lastIntervalMinutes = interval;
        }

        dt = std::max(0.0f, dt);

        // Keep the request lock until a conservative timeout. This prevents
        // an immediate duplicate call while the game's network request runs.
        if (g_requestPending) {
            g_pendingSeconds += dt;
            if (g_pendingSeconds < kRequestTimeoutSeconds) {
                return;
            }
            log::warn("AutoComment request timed out without a server callback; will retry after the configured interval.");
            g_requestPending = false;
            g_pendingSeconds = 0.0;
            g_elapsedSeconds = 0.0;
        }

        g_elapsedSeconds += dt;
        auto requiredDelay = g_firstAttempt
            ? 1.0
            : static_cast<double>(interval) * kSecondsPerMinute;
        if (g_elapsedSeconds < requiredDelay) {
            return;
        }

        // Lock and reset before invoking the game's networking method.
        g_requestPending = true;
        g_pendingSeconds = 0.0;
        g_elapsedSeconds = 0.0;
        g_firstAttempt = false;

        GameLevelManager::sharedState()->uploadComment(
            comment,
            CommentType::Level,
            levelID,
            0
        );

        log::info("AutoComment submitted a level-comment upload request for level {}", levelID);
    }

    // Geometry Dash calls this when the comment request finishes. Releasing
    // the lock here allows the next scheduled comment instead of getting stuck.
    void onUploadCommentCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onUploadCommentCompleted(response, tag);

        g_requestPending = false;
        g_pendingSeconds = 0.0;
        g_elapsedSeconds = 0.0;

        if (response == "1") {
            log::info("AutoComment: Geometry Dash accepted the comment request.");
        }
        else {
            log::warn("AutoComment: comment request returned response: {}", response);
        }
    }
};

class $modify(AutoCommentMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

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

    void onAutoCommentButton(CCObject*) {
        auto mod = Mod::get();
        auto enabled = mod->getSettingValue<bool>("enabled");
        auto rawID = mod->getSettingValue<std::string>("level-id");
        auto comment = trim(mod->getSettingValue<std::string>("comment-text"));
        auto interval = std::clamp<int64_t>(
            mod->getSettingValue<int64_t>("interval-minutes"),
            5,
            1440
        );

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
    auto manager = GameLevelManager::sharedState();
    if (manager) {
        manager->scheduleUpdate();
    }
    log::info("AutoComment loaded. Timer minimum: 5 minutes.");
}
