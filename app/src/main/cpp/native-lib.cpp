#include <jni.h>
#include <string>
#include "dlfcn.h"
#include "iostream"
#include "unistd.h"
#include "UnityResolve.hpp"
#include "thread"
#include "classmap.h"
#include "KittyMemory/KittyInclude.hpp"

static bool didOnce = false;
static void main() {
    // inside would be your code, example:
    classmap map;
    LOGI("method pointer: %p", map["assembly.dll"]["Namespace"]["Class"]["Method"].funcptr());
    LOGI("method class name: %s", map["assembly.dll"]["Namespace"]["Class"]["Method"].get()->klass->name.c_str());
}

__attribute__ ((constructor))
void lib_main() {
    if (didOnce) return;
    didOnce = true;

    LOGI("Loading %s", libFile);
    std::thread([] {
        ProcMap map;
        do {
            map = KittyMemory::getElfBaseMap("libil2cpp.so");
            sleep(1);
        } while (!map.isValidELF() || !map.isValid());
        UnityResolve::Init(dlopen("libil2cpp.so", RTLD_NOW), UnityResolve::Mode::Il2Cpp);
	main();
    }).detach();

    return;
}