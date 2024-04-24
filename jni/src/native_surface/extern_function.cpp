#include "extern_function.h"
#include "native_surface/aosp/native_surface_9.h"
#include "native_surface/aosp/native_surface_10.h"
#include "native_surface/aosp/native_surface_11.h"
#include "native_surface/aosp/native_surface_11_1.h"
#include "native_surface/aosp/native_surface_12.h"
#include "native_surface/aosp/native_surface_12_1.h"
#include "native_surface/aosp/native_surface_13.h"

static void *handle;// 动态库方案
static void *handle1;// 动态库方案


ExternFunction::ExternFunction() {
    // printf("当前安卓版本代号:%d ", get_android_api_level());
    if (get_android_api_level() >= 30 && get_android_api_level() <= 32) {
       // printf("支持隐藏窗口，以自动开启\n");
    }
    if (!handle) {
        if (get_android_api_level() == 33) { // 安卓13支持
            exec_native_surface("settings put global block_untrusted_touches 0");
            handle = dlblob(&native_surface_13_64, sizeof(native_surface_13_64)); // 64位
        } else if (get_android_api_level() == /*__ANDROID_API_S__*/ 32) { // 安卓12.1支持
            exec_native_surface("settings put global block_untrusted_touches 0");
            handle = dlblob(&native_surface_12_1_64, sizeof(native_surface_12_64)); // 64位支持
        } else if (get_android_api_level() == /*__ANDROID_API_S__*/ 31) { // 安卓12支持
            exec_native_surface("settings put global block_untrusted_touches 0");
            handle = dlblob(&native_surface_12_64, sizeof(native_surface_12_64)); // 64位支持
        } else if (get_android_api_level() == /*__ANDROID_API_R__*/ 30) { // 安卓11支持
            handle = dlblob(&native_surface_11_64, sizeof(native_surface_11_64)); // 64位支持
            handle1 = dlblob(&native_surface_11_1_64, sizeof(native_surface_11_1_64)); // 64位支持
        } else if (get_android_api_level() == 29) { // 安卓10支持
            handle = dlblob(&native_surface_10_64, sizeof(native_surface_10_64)); // 64位支持
        } else if (get_android_api_level() == __ANDROID_API_P__) { // 安卓9支持
            handle = dlblob(&native_surface_9_64, sizeof(native_surface_9_64)); // 64位支持
        } else {
            printf("Sorry, level:%d Don't Support~", get_android_api_level());
            exit(0);
        }
    }
    if (get_android_api_level() >= /*__ANDROID_API_R__*/ 31) {
    funcPointer.func_more_createNativeWindow = dlsym(handle, "_Z18createNativeWindowPKcjjjjb");
    }else{
    funcPointer.func_createNativeWindow = dlsym(handle, "_Z18createNativeWindowPKcjjb");
    }
    if (get_android_api_level() == /*__ANDROID_API_R__*/ 30) {
    funcPointer.func_createNativeWindow = dlsym(handle1, "_Z18createNativeWindowPKcjj");
    }else{
    funcPointer.func_createNativeWindow = dlsym(handle, "_Z18createNativeWindowPKcjjb");
    }
    // 获取屏幕信息
    funcPointer.func_getDisplayInfo = dlsym(handle, "_Z14getDisplayInfov");
        //funcPointer.func_setSurfaceWH = dlsym(handle, "_Z12setSurfaceWHjj");
        funcPointer.func_initRecord = dlsym(handle, "_Z10initRecordPKcfjj");
        funcPointer.func_runRecord = dlsym(handle, "_Z9runRecordPbPFvPhmE");
        funcPointer.func_stopRecord = dlsym(handle, "_Z10stopRecordv");
        funcPointer.func_getRecordNativeWindow = dlsym(handle, "_Z21getRecordNativeWindowv");
    }
   
/**
 * 创建 native surface
 * @param surface_name 创建名称
 * @param screen_width 创建宽度
 * @param screen_height 创建高度
 * @param author 是否打印作者信息
 * @return
 */
ANativeWindow *
ExternFunction::createNativeWindow(const char *surface_name, uint32_t screen_width, uint32_t screen_height,
                                   bool author) {
    return ((ANativeWindow *(*)(
            const char *, uint32_t, uint32_t, bool))
            (funcPointer.func_createNativeWindow))(surface_name, screen_width, screen_height, author);
}

/**
 * (更多可选参数)创建 native surface
 * @param surface_name 创建名称
 * @param screen_width 创建宽度
 * @param screen_height 创建高度
 * @param format format
 * @param flags flags
 * @param author 是否打印作者信息
 * @return
 */
ANativeWindow *
ExternFunction::createNativeWindow(const char *surface_name, uint32_t screen_width, uint32_t screen_height,
                                   uint32_t format, uint32_t flags, bool author) {
    return ((ANativeWindow *(*)(
            const char *, uint32_t, uint32_t, uint32_t, uint32_t, bool))
            (funcPointer.func_more_createNativeWindow))(surface_name, screen_width, screen_height, format, flags,
                                                        author);
}

/**
 * 获取屏幕宽高以及旋转状态
 */
MDisplayInfo ExternFunction::getDisplayInfo() const {
    return ((MDisplayInfo (*)()) (funcPointer.func_getDisplayInfo))();
}
