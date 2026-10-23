#include "app/app.h"

#include "common/options.h"

namespace game {
void InstallCaptureSignal();
void PadHook(int port, input::Pad& p);
void ScaleDrawDistance();

void Init() { InstallCaptureSignal(); }
void Ui(double) {}
}  // namespace game

// what the runtime asks of the game; strings must be set, hooks may stay null
const app::GameInfo app::kGame = {
    .title = L"Dynasty Warriors 5",         // window and splash
    .short_title = L"Dynasty Warriors 5",   // message boxes
    .name = L"DW5",                         // window classes, crash dumps, handover event
    .log = "dw5",                           // log file prefix
    .settings = "settings.toml",            // settings file beside the exe
    .splash = L"splash",                    // optional splash.png or splash.bmp beside the exe
    .missing = L"Put the game in the assets folder: its disc image (.iso), or default.xbe with its folders beside it.",
    .images = {{"default.xbe", "DW5"}},     // each recompiled xbe and its exe, for title launches
    .options = {{"draw_distance", nullptr, &game::Opt().draw_distance}},  // game settings kept in the settings file
    .load = nullptr,                        // reads more from the settings file's toml::table
    .save = nullptr,                        // writes more into it
    .init = game::Init,                     // after the settings load, before the window and boot
    .loaded = game::ScaleDrawDistance,      // image in memory, before its entry point runs
    .window = nullptr,                      // the game window is up
    .message = nullptr,                     // window messages first; true takes one
    .ui = game::Ui,                         // ImGui drawing each frame
    .pad = game::PadHook,                   // changes a polled pad
    .rumble = nullptr,                      // changes a rumble before it is set
};
