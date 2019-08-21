# handWritten-digit-recognition-based-on-OpenCV
我的第一个项目，原本是软件工程课程设计
还有许多不足需要改进orz

基于OpenCV3.1的简单的手写数字（MNIST库）识别系统

目前版本使用的算法为SVM，特征为8 * 8全部像素与28 * 28（原图）直方图

训练错误率和测试错误率如下：
![image](https://github.com/wkroach/handWritten-digit-recognition-based-on-OpenCV/blob/master/resources/errorRateImage.jpg)

太弱了，这么渣怎么出去见人555

实现功能1：
    在控制台画板（opencv实现）上用鼠标进行书写并识别（支持多数字识别）
    ![image](https://github.com/wkroach/handWritten-digit-recognition-based-on-OpenCV/blob/master/resources/showImage.jpg)

实现功能2：
    图片中的手写数字识别（OCR），对于有污点的图像识别效果仍然不好，OCR什么的好难。。。
    原图：
     ![image](https://github.com/wkroach/handWritten-digit-recognition-based-on-OpenCV/blob/master/resources/sorceImage.jpg)
     处理后：
      ![image](https://github.com/wkroach/handWritten-digit-recognition-based-on-OpenCV/blob/master/resources/proccessedImage.jpg)
      不管怎么滤波都去不了噪点。。。蒙版选大了又会对原图造成影响。。。。orz
      ![image](https://github.com/wkroach/handWritten-digit-recognition-based-on-OpenCV/blob/master/resources/resultImage.jpg)
      ![image](https://github.com/wkroach/handWritten-digit-recognition-based-on-OpenCV/blob/master/resources/answer.jpg)
      识别时还有一定误差。。。。

还需要加油啊！！！

