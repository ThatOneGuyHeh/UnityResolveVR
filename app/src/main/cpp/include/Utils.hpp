#pragma once

#include <codecvt>
#include <jni.h>
#include <locale>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <dobby.h>
#include <string>

typedef unsigned long DWORD;

class Utils {
public:
    static DWORD findLibrary(const char* library) {
        FILE* fp;
        unsigned long addr = 0;
        char* pch;
        char filename[32];
        char buffer[1024];

        snprintf(filename, sizeof(filename), "/proc/self/maps");

        fp = fopen(filename, "r");

        if (fp != NULL) {
            while (fgets(buffer, sizeof(buffer), fp)) {
                if (strstr(buffer, library)) {
                    addr = (DWORD)strtoul(buffer, NULL, 16);
                    if (addr == 0x8000)
                        addr = 0;
                    break;
                }
            }
            fclose(fp);
        }
        return addr;
    }

    static DWORD getAbsoluteAddress(const char* libraryName, DWORD relativeAddr) {
        DWORD libBase;
        if (libBase == 0)
            libBase = findLibrary(libraryName);
        if (libBase != 0)
            return (reinterpret_cast<DWORD>(libBase + relativeAddr));
        else
            return 0;
    }

    static bool isLibraryLoaded(const char* libraryName) {
        char line[512] = { 0 };
        FILE* fp = fopen("/proc/self/maps", "rt");
        if (fp != NULL) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, libraryName))
                    return true;
            }
            fclose(fp);
        }
        return false;
    }

    template<typename PTR_T, typename NEW_T, typename T_OLD>
    static inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &oldBytes) {
        if ((void *) ptr != nullptr) DobbyHook((void *)ptr, (dobby_dummy_func_t)newMethod, (dobby_dummy_func_t*)&oldBytes);
        return (void *) ptr;
    }

    template<typename PTR_T, typename NEW_T, typename T_OLD>
    static inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &&oldBytes) {
        if ((void *) ptr != nullptr) DobbyHook((void *)ptr, (dobby_dummy_func_t)newMethod, (dobby_dummy_func_t*)&oldBytes);
        return (void *) ptr;
    }

    template<typename PTR_T>
    static inline void Unhook(PTR_T ptr) {
        if ((void *) ptr != nullptr) DobbyDestroy((void *)ptr);
    }
    [[nodiscard]] static inline auto ToString(Il2CppString* str) -> std::string {
       std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
       std::u16string u16str(reinterpret_cast<const char16_t *>(str->chars), str->length);
       return converter.to_bytes(u16str);
    }
};