adb push libs/armeabi-v7a /data/local/tmp/
adb push ./../data /data/local/tmp/
adb shell chmod 777 /data/local/tmp/fir_float
adb shell chmod 777 /data/local/tmp/image_scaling
adb shell chmod 777 /data/local/tmp/mandelbrot
adb shell chmod 777 /data/local/tmp/sgemm
adb shell chmod 777 /data/local/tmp/sobel
adb shell chmod 777 /data/local/tmp/sobel_no_vectors
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/fir_float
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/image_scaling
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/mandelbrot
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/sgemm
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/sobel
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp:/system/lib/egl /data/local/tmp/sobel_no_vectors
adb pull /data/local/tmp/output/  ./../output/