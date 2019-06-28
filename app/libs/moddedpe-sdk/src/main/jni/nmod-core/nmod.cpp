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
//-------------------------------------------------------------
// Includes
//-------------------------------------------------------------
 
#include <jni.h>
#include <string>
#include <cxxabi.h>
#include "Substrate.h"

//-------------------------------------------------------------
// Variants
//-------------------------------------------------------------

bool mGameStarted = false;
JavaVM* mJvm = NULL;
std::string* mAndroidAppDataPath = NULL;
std::string mMinecraftNativeLibPath;
 
//-------------------------------------------------------------
// Methods Definition
//-------------------------------------------------------------
 
std::string toString(JNIEnv* env, jstring j_str)
{
	const char * c_str = env->GetStringUTFChars(j_str, 0);
	std::string cpp_str = c_str;
	env->ReleaseStringUTFChars(j_str,c_str);
	return cpp_str;
}
 
//-------------------------------------------------------------
// Hook Methods
//-------------------------------------------------------------
 
void (*setStartMenuScreen_)(void*);
void setStartMenuScreen(void*self)
{
	setStartMenuScreen_(self);
	mGameStarted=true;
}

//-------------------------------------------------------------
// Native Methods
//-------------------------------------------------------------
 
namespace NModAPI
{
	void nativeSetDataDirectory(JNIEnv*env,jobject thiz,jstring directory)
	{
		*mAndroidAppDataPath = toString(env,directory);
	}
	jboolean nativeIsGameStarted(JNIEnv*env,jobject thiz)
	{
		return mGameStarted;
	}
	jboolean nativeCallOnActivityFinish(JNIEnv*env,jobject thiz,jstring libname,jobject mainActivity)
	{
		void* image=dlopen(toString(env,libname).c_str(),RTLD_LAZY);
		void (*NMod_onActivityFinish)(JNIEnv*env,jobject thiz)=
		(void (*)(JNIEnv*,jobject)) dlsym(image,"NMod_OnActivityFinish");
		if(NMod_onActivityFinish)
		{
			NMod_onActivityFinish(env,mainActivity);
		}
		dlclose(image);
	}
	jboolean nativeCallOnLoad(JNIEnv*env,jobject thiz,jstring libname,jstring mcVer,jstring apiVersion)
	{
		void* image=dlopen(toString(env,libname).c_str(),RTLD_LAZY);
		void (*NMod_onLoad)(JavaVM*,JNIEnv*,std::string const&,std::string const&,std::string const&)=
		(void (*)(JavaVM*,JNIEnv*,std::string const&,std::string const&,std::string const&)) dlsym(image,"NMod_OnLoad");
		if(NMod_onLoad)
		{
			NMod_onLoad(mJvm,env,toString(env,mcVer),toString(env,apiVersion),mMinecraftNativeLibPath);
		}
		dlclose(image);
	}
	jboolean nativeCallOnActivityCreate(JNIEnv*env,jobject thiz,jstring libname,jobject mainActivity,jobject bundle)
	{
		void* image=dlopen(toString(env,libname).c_str(),RTLD_LAZY);
		void (*NMod_onActivityCreate)(JNIEnv*env,jobject thiz,jobject savedInstanceState)=
		(void (*)(JNIEnv*,jobject,jobject)) dlsym(image,"NMod_OnActivityCreate");
		if(NMod_onActivityCreate)
		{
			NMod_onActivityCreate(env,mainActivity,bundle);
		}
		dlclose(image);
	}
	jstring nativeDemangle(JNIEnv*env,jobject thiz,jstring str)
	{
		char const* symbol_name = toString(env,str).c_str();
		if(symbol_name)
		{
			char const* ret = abi::__cxa_demangle(symbol_name,0,0,0);
			return env->NewStringUTF(ret);
		}
		return env->NewStringUTF("");
	}
}
 
//-------------------------------------------------------------
// Register Natives
//-------------------------------------------------------------
 
extern "C" JNIEXPORT jboolean JNICALL Java_com_mcal_pesdk_nmod_NModLib_nativeRegisterNatives(JNIEnv*env,jobject thiz,jclass cls)
{
	JNINativeMethod methods[] =
	{
		{"nativeCallOnActivityFinish", "(Ljava/lang/String;Lcom/mojang/minecraftpe/MainActivity;)Z", (void *)&NModAPI::nativeCallOnActivityFinish},
		{"nativeCallOnLoad", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void *)&NModAPI::nativeCallOnLoad},
		{"nativeCallOnActivityCreate", "(Ljava/lang/String;Lcom/mojang/minecraftpe/MainActivity;Landroid/os/Bundle;)Z", (void *)&NModAPI::nativeCallOnActivityCreate}
	};
	
	if (env->RegisterNatives(cls,methods,sizeof(methods)/sizeof(JNINativeMethod)) < 0)
		return JNI_FALSE;
	return JNI_TRUE;
}
 
extern "C" JNIEXPORT jboolean JNICALL Java_com_mcal_pesdk_nativeapi_NativeUtils_nativeRegisterNatives(JNIEnv*env,jobject thiz,jclass cls)
{
	JNINativeMethod methods[] =
	{
		{"nativeIsGameStarted", "()Z", (void *)&NModAPI::nativeIsGameStarted},
		{"nativeSetDataDirectory", "(Ljava/lang/String;)V", (void *)&NModAPI::nativeSetDataDirectory},
		{"nativeDemangle", "(Ljava/lang/String;)Ljava/lang/String;", (void *)&NModAPI::nativeDemangle}
	};
	
	if (env->RegisterNatives(cls,methods,sizeof(methods)/sizeof(JNINativeMethod)) < 0)
		return JNI_FALSE;
	return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL Java_com_mcal_pesdk_nativeapi_LibraryLoader_nativeOnNModAPILoaded(JNIEnv*env,jobject thiz,jstring libPath)
{
	mMinecraftNativeLibPath = toString(env,libPath);
    void* imageMCPE = dlopen(mMinecraftNativeLibPath.c_str(),RTLD_LAZY);
	mAndroidAppDataPath = ((std::string*)dlsym(imageMCPE,"_ZN19AppPlatform_android20ANDROID_APPDATA_PATHE"));
	void* ptr_setStartMenuScreen = (void*)dlsym(imageMCPE,"_ZN13ScreenChooser18setStartMenuScreenEv");
	MSHookFunction(ptr_setStartMenuScreen,(void*)&setStartMenuScreen,(void**)&setStartMenuScreen_);
	dlclose(imageMCPE);
}
 
//-------------------------------------------------------------
// On Load
//-------------------------------------------------------------
 
JNIEXPORT jint JNI_OnLoad(JavaVM*vm,void*)
{
	mJvm = vm;
	return JNI_VERSION_1_6;
}
 
 
 
