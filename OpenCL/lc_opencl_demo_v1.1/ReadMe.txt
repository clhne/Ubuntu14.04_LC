编译:
cd ./android
执行ndk-build(linux环境下可执行~/UR_NDK_PATH/ndk.build; windows环境下可将NDK根目录加入系统环境变量中然后ndk-build)
./android/libs/armeabi-v7a 目录下会生成test cases for android.

自动测试:
保证adb可用
windows下直接执行run.bat测试脚本;
linux下直接执行run.sh;
测试结果会被adb pull到./../output目录下
./../data/golden_output/为预期得到的输出结果，可用于校检

如需修改，可参考脚本文件自行测试
