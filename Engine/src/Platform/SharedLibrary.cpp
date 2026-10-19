#include <Platform/SharedLibrary.h>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

SharedLibrary::SharedLibrary(const std::string& path) {
#if defined(_WIN32)
    m_handle = reinterpret_cast<void*>(LoadLibraryA(path.c_str()));
#else
    m_handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
}

SharedLibrary::~SharedLibrary() {
    if (m_handle == nullptr) {
        return;
    }

#if defined(_WIN32)
    FreeLibrary(reinterpret_cast<HMODULE>(m_handle));
#else
    dlclose(m_handle);
#endif

    m_handle = nullptr;
}

void* SharedLibrary::GetSymbol(const char* name) const {
    if (m_handle == nullptr) {
        return nullptr;
    }

#if defined(_WIN32)
    return reinterpret_cast<void*>(GetProcAddress(reinterpret_cast<HMODULE>(m_handle), name));
#else
    return dlsym(m_handle, name);
#endif
}

std::string SharedLibrary::GetLastError() {
#if defined(_WIN32)
    return "Win32 error " + std::to_string(::GetLastError());
#else
    const char* error = dlerror();

    return error != nullptr ? error : "";
#endif
}
