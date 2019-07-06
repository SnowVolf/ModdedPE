-keepattributes SourceFile, LineNumberTable
-renamesourcefileattribute SourceFile
-repackageclasses

#-obfuscationdictionary proguard-dictionary.txt
#-packageobfuscationdictionary proguard-dictionary.txt
#-classobfuscationdictionary proguard-dictionary.txt

-keep class com.mcal.pesdk.nativeapi.** { *; }

-keep class com.mojang.** { *; }
-keep class org.fmod.** { *; }
-keep class Microsoft.** { *; }
-keep class net.hockeyapp.** { *; }
-keep class MS.** { *; }
-keep class com.microsoft.** { *; }
-keep class com.google.** { *; }
-keep class com.googleplay.** { *; }
-keep class com.facebook.** { *; }
-keep class com.appboy.** { *; }
-keep class com.appsflyer.** { *; }
-keep class com.amazon.** { *; }
-keep class com.android.** { *; }
-keep class bo.** { *; }
-keep class bolts.** { *; }
-keep class org.apache.** { *; }
-keep class com.simpleframework.** { *; }
-keep class android.net.** { *; }