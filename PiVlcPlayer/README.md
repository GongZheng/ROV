手机控制端是一个Android studio 项目

导入项目后，可以自己修改对应的build.gradle版本号
如果你修改了版本号，一定要全部版本号都进行修改
在 dependencies {
        classpath 'com.android.tools.build:gradle:2.2.1'

        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
把2.2.1改成你自己版本号，这样导入工程会快很多。

然后就是修改每个项目中的sdk版本号，和sdktool版本号
 compileSdkVersion 24
    buildToolsVersion '24.0.3'

如果你不想下载我的这个版本号，就要改成你们sdk有的版本号，然后同步一下，项目就会导入成功，成功之后就可以直接运行。
