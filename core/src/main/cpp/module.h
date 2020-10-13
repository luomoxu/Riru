#pragma once

#include <jni.h>
#include <map>
#include <vector>
#include "api.h"

#define MODULE_NAME_CORE "core"

struct RiruModule {

public:
    const char *name;
    int apiVersion;
    uint32_t token;

    void *handle{};
    std::map<const char *, void *> *funcs;

    int supportHide;
    int version;
    const char *versionName;

private:
    void *_onModuleLoaded;
    void *_shouldSkipUid;
    void *_forkAndSpecializePre;
    void *_forkAndSpecializePost;
    void *_forkSystemServerPre;
    void *_forkSystemServerPost;
    void *_specializeAppProcessPre;
    void *_specializeAppProcessPost;

public:
    explicit RiruModule(const char *name, uint32_t token = 0) : name(name), token(token ? token : (uintptr_t) name) {
        funcs = new std::map<const char *, void *>();
    }

    void info(RiruModuleInfoV9 *info) {
        supportHide = info->supportHide;
        version = info->version;
        versionName = strdup(info->versionName ? info->versionName : "(null)");
        _onModuleLoaded = (void *) info->onModuleLoaded;
        _shouldSkipUid = (void *) info->shouldSkipUid;
        _forkAndSpecializePre = (void *) info->forkAndSpecializePre;
        _forkAndSpecializePost = (void *) info->forkAndSpecializePost;
        _forkSystemServerPre = (void *) info->forkSystemServerPre;
        _forkSystemServerPost = (void *) info->forkSystemServerPost;
        _specializeAppProcessPre = (void *) info->specializeAppProcessPre;
        _specializeAppProcessPost = (void *) info->specializeAppProcessPost;
    }

    bool hasOnModuleLoaded() {
        return _onModuleLoaded != nullptr;
    }

    bool hasShouldSkipUid() {
        return _shouldSkipUid != nullptr;
    }

    bool hasForkAndSpecializePre() {
        return _forkAndSpecializePre != nullptr;
    }

    bool hasForkAndSpecializePost() {
        return _forkAndSpecializePost != nullptr;
    }

    bool hasForkSystemServerPre() {
        return _forkSystemServerPre != nullptr;
    }

    bool hasForkSystemServerPost() {
        return _forkSystemServerPost != nullptr;
    }

    bool hasSpecializeAppProcessPre() {
        return _specializeAppProcessPre != nullptr;
    }

    bool hasSpecializeAppProcessPost() {
        return _specializeAppProcessPost != nullptr;
    }

    void onModuleLoaded() {
        if (apiVersion == 9) {
            ((onModuleLoaded_v9 *) _onModuleLoaded)();
        }
    }

    bool shouldSkipUid(int uid) {
        if (apiVersion == 9) {
            return ((shouldSkipUid_v9 *) _shouldSkipUid)(uid);
        }
        return false;
    }

    void forkAndSpecializePre(
            JNIEnv *env, jclass cls, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
            jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
            jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
            jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp, jobjectArray *pkgDataInfoList,
            jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {

        if (apiVersion == 9) {
            ((nativeForkAndSpecializePre_v9 *) _forkAndSpecializePre)(
                    env, cls, uid, gid, gids, runtimeFlags, rlimits, mountExternal,
                    seInfo, niceName, fdsToClose, fdsToIgnore, is_child_zygote,
                    instructionSet, appDataDir, isTopApp, pkgDataInfoList, whitelistedDataInfoList,
                    bindMountAppDataDirs, bindMountAppStorageDirs);
        }
    }

    void forkAndSpecializePost(JNIEnv *env, jclass cls, jint res) {
        if (apiVersion == 9) {
            ((nativeForkAndSpecializePost_v9 *) _forkAndSpecializePost)(
                    env, cls, res);
        }
    }

    void forkSystemServerPre(
            JNIEnv *env, jclass cls, uid_t *uid, gid_t *gid, jintArray *gids, jint *runtimeFlags,
            jobjectArray *rlimits, jlong *permittedCapabilities, jlong *effectiveCapabilities) {

        if (apiVersion == 9) {
            ((nativeForkSystemServerPre_v9 *) _forkSystemServerPre)(
                    env, cls, uid, gid, gids, runtimeFlags, rlimits, permittedCapabilities,
                    effectiveCapabilities);
        }
    }

    void forkSystemServerPost(JNIEnv *env, jclass cls, jint res) {
        if (apiVersion == 9) {
            ((nativeForkSystemServerPost_v9 *) _forkSystemServerPost)(
                    env, cls, res);
        }
    }

    void specializeAppProcessPre(
            JNIEnv *env, jclass cls, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
            jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
            jboolean *startChildZygote, jstring *instructionSet, jstring *appDataDir,
            jboolean *isTopApp, jobjectArray *pkgDataInfoList, jobjectArray *whitelistedDataInfoList,
            jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {

        if (apiVersion == 9) {
            ((nativeSpecializeAppProcessPre_v9 *) _specializeAppProcessPre)(
                    env, cls, uid, gid, gids, runtimeFlags, rlimits, mountExternal, seInfo,
                    niceName, startChildZygote, instructionSet, appDataDir, isTopApp,
                    pkgDataInfoList, whitelistedDataInfoList, bindMountAppDataDirs, bindMountAppStorageDirs);
        }
    }

    void specializeAppProcessPost(JNIEnv *env, jclass cls) {
        if (apiVersion == 9) {
            ((nativeSpecializeAppProcessPost_v9 *) _specializeAppProcessPost)(
                    env, cls);
        }
    }
};

std::vector<RiruModule *> *get_modules();

void load_modules();