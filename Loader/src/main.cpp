#include <App.h>
#include <Game.h>
#include <Platform/SharedLibrary.h>

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    // Game module sits next to the executable unless a path is passed on the command line.
    const std::filesystem::path executableDirectory = std::filesystem::absolute(argv[0]).parent_path();
    const std::filesystem::path gameModulePath = argc > 1 ? std::filesystem::path(argv[1]) : executableDirectory / SHIBA_GAME_MODULE;

    const SharedLibrary gameModule{gameModulePath.string()};

    if (!gameModule.IsLoaded()) {
        std::cerr << "Failed to load game module " << gameModulePath << ": " << SharedLibrary::GetLastError() << '\n';
        return 1;
    }

    const auto createGame = gameModule.GetSymbol<CreateGameFn>(CreateGameSymbol);

    if (createGame == nullptr) {
        std::cerr << "Game module " << gameModulePath << " does not export " << CreateGameSymbol << '\n';
        return 1;
    }

    // Scoped so the game is destroyed before its module is unloaded.
    {
        const App app{createGame()};

        app.Run();
    }

    return 0;
}
