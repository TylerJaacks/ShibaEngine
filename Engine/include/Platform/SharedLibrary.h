#ifndef SHIBAENGINE_SHAREDLIBRARY_H
#define SHIBAENGINE_SHAREDLIBRARY_H

#include <string>

// Loads a .dll / .so / .dylib at runtime and resolves symbols from it.
class SharedLibrary {
public:
    explicit SharedLibrary(const std::string& path);
    ~SharedLibrary();

    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;

    [[nodiscard]] bool IsLoaded() const { return m_handle != nullptr; }

    [[nodiscard]] void* GetSymbol(const char* name) const;

    template<typename T>
    [[nodiscard]] T GetSymbol(const char* name) const {
        return reinterpret_cast<T>(GetSymbol(name));
    }

    [[nodiscard]] static std::string GetLastError();

private:
    void* m_handle;
};

#endif //SHIBAENGINE_SHAREDLIBRARY_H
