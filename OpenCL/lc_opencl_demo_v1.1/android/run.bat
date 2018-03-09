@echo off

adb push libs/armeabi-v7a /data/local/tmp/
adb push ./../data /data/local/tmp/
adb shell chmod 777 /data/local/tmp/fir_float
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/fir_float
adb pull /data/local/tmp/output/  ./../output/