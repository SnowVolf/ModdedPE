/*
 * Copyright (C) 2018-2019 Тимашков Иван
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <jni.h>
#include <android/native_activity.h>
#include <dlfcn.h>

void (*mOnCreateFunc)(ANativeActivity*,void*,size_t) = 0;
void (*mFinishFunc)(ANativeActivity*) = 0;
void (*mMainFunc)(struct android_app*) = 0;

extern "C" void ANativeActivity_onCreate(ANativeActivity* activity,void* savedState, size_t savedStateSize)
{
	mOnCreateFunc(activity,savedState,savedStateSize);
}

extern "C" void ANativeActivity_finish(ANativeActivity* activity)
{
	mFinishFunc(activity);
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM*,void*)
{
	return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL Java_com_mcal_pesdk_nativeapi_LibraryLoader_nativeOnLauncherLoaded(JNIEnv*env,jobject thiz,jstring libPath)
{
	const char * mNativeLibPath = env->GetStringUTFChars(libPath, 0);
	void* imageMCPE=(void*) dlopen(mNativeLibPath,RTLD_LAZY);
	mOnCreateFunc = (void(*)(ANativeActivity*,void*,size_t)) dlsym(imageMCPE,"ANativeActivity_onCreate");
	mFinishFunc = (void(*)(ANativeActivity*)) dlsym(imageMCPE,"ANativeActivity_finish");
	mMainFunc =(void(*)(struct android_app*)) dlsym(imageMCPE,"android_main");
	dlclose(imageMCPE);
	env->ReleaseStringUTFChars(libPath,mNativeLibPath);
}

extern "C" void android_main(struct android_app* state)
{
	mMainFunc(state);
}
