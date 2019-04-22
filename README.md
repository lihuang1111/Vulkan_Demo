## vulkan test
目前vulkan demo 都暂时只支持Android Studio编译。  
参考了 [google sample][] 

[google sample]: https://github.com/googlesamples/android-vulkan-tutorials
 

移植apk 修改Android.mk 编译生成的binary方式，方便学习或者在Android framework 中使用。  

可以放置在<sdk_path>/frameworks/native/vulkan 目录下进行编译，生成binary 可执行文件，直接push到设备即可执行。  

目前暂未移植glsl 转换 spv 部分的功能，请直接使用转换后的.spv 文件，放在shader 目录下，需要  
`adb push shader/*.spv /data/`  

* tip: glsl转换为spv 可以考虑shaderc 后续有空研究加上

