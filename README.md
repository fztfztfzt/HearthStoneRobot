# HearthStoneRobot
炉石传说机器人
######准备写个炉石传说自动战斗AI,准备采用图像识别技术进行模拟
    一些问题：
    1.图像的采集
    2.图像的查找比较
    3.控制鼠标操作
	4.识别卡牌费用
test 1 图像的采集<br>
思路：<br>
1. 获取炉石传说程序的句柄<br>
2.通过句柄获取窗口<br>
3.获取窗口像素<br>
查找资料并实验得到有两种方法：<br>
**方法一：获取窗口后，使用bitblt函数进行窗口函数拷贝，缺点是窗口不能有遮挡**<br>
**方法二：使用printWindows 函数获取窗口图像，缺点是该函数是xp下专用，win10上对于有些窗口会无法获取图像，得到黑屏**<br>
经测试，炉石传说可以使用第二张方法，代码见`test/getWindowsImage.cpp`<br>

test 2 图像的查找比较
思路：<br>
1.获取当前画面<br>
2.读取待比较bmp<br>
3.比较<br>
已知方法：<br>
**方法一：逐像素，判断rgb差值，缺点慢，优点准确 代码见** `test/findSubImage.cpp`<br>
**方法二:采用相似图像比较分方法，比如pHash等，未测试**<br>

test 3 控制鼠标操作<br>
思路：<br>
1.获取目标窗口句柄<br>
2.给目标窗口发送消息<br>
3.获取鼠标在窗口的位置<br>
已知方法：<br>
**方法一：使用`sendMessage` 或者`postMessage`函数向炉石程序发送消息**<br>
**方法二：使用`mouse_event`模拟鼠标行动**<br>
经测试，炉石传说对方法一的消息不响应，即使设置窗口为`SetForegroundWindow`<br>
故采用第二种方式，第二种方式要求窗口在最前面，且固定位置，因此将窗口移到左上角，固定分辨率。代码见`test/controlMouse.cpp`<br>
test 4 识别卡牌费用<br>
有2种方法，代码见`test/HStest.cpp`：   
方法一：使用图像查找比较的方法。经测试，有如下问题：    
        1.图片大小不好统一   
        2.图片背景因为有粒子的变化，变动较大       
        3.费用在某些条件下会更改并改变颜色     
方法二：使用数字识别的方法。     
有2个场景需要识别费用：      
场景一：发牌换牌阶段     
此阶段，背景为黑色，便于分离。流程：灰度化，阈值成二值图像，轮廓提取，测试得到卡牌的轮廓范围，近似矩形，得到卡牌数（得到先手还是后手），根据轮廓得到每个卡牌，对每个卡牌的左上角进行灰度处理，阈值化，轮廓提取，得到数字图像。使用数字识别的方法进行判断。     
结果：![提取卡牌](https://github.com/fztfztfzt/HearthStoneRobot/blob/master/image/scene1_getRect.bmp)   
![提取数字](https://github.com/fztfztfzt/HearthStoneRobot/blob/master/image/scene1_getNum_2.bmp)     
场景二：出牌场景     
此阶段，背景色彩多样，不能采用场景一的方式，经观察，发现数字部分为白色，且形状接近矩形（宽小于高），测试流程：     
遍历图像，白色部分不变，其他变为黑色，轮廓提取，去除过小的，近似矩形，去除宽大于等于高的，剩下部分均为数字。     
结果：![提取数字](https://github.com/fztfztfzt/HearthStoneRobot/blob/master/image/scene2_getNum.bmp)   

难点解决完毕
项目环境：VS2013+opencv,opencv主要是使用提取轮廓函数，操作方便
日志：   
2016-5-5:   
建立项目，配置opencv环境   
开始游戏操作编写，完成发牌阶段卡牌个数与费用识别   
发现问题：
 *后手时，有4张牌，彼此之间距离较小，由于卡牌边缘有绿色粒子效果导致原先方案会出现4张卡牌连在一起的问题，解决方法：
     1.将连在一起的图像横向分成4分
	 2.使用阈值将绿色的粒子去除，并将设置中的画面效果调成最低降低粒子效果（目前使用此种方法）
 *数字识别会出现一些错误，比如5识别成3等，解决方法：
     1.增加每个数字模型个数，将识别错误的样例加入模型

2016-5-6:
增加出牌
发现问题：
 *卡牌在手中的数字是歪的，会产生错误

2016-5-9:
玩了2天，愧疚。。。
改用特征点识别卡牌，组了一副T7猎，将卡组中的卡牌保存下来作为识别的模版
![T7猎卡组](https://github.com/fztfztfzt/HearthStoneRobot/blob/master/image/T7猎卡组.png)     
特征识别测试代码见`test/compareImageByORB.cpp`    
更改：
换牌阶段：使用特征识别，将得到的卡牌图片与每张模版图片比较，选出最接近的图片，查找该模版图片的信息并记录。
自己出牌阶段：根据定位出的数字位置（不采用数字识别的方法），将鼠标移动到数字位置，卡牌会被放大，根据位置关系获取卡牌图片，运用特征识别比较识别。
ControlMouse改为单例模式[自己出牌阶段也需要移动鼠标，对鼠标进行控制]
移动鼠标后并不能立即获取图片，需要Sleep一会等鼠标已过去，太快获取的是旧图片
 
 2016-5-10:
 更新：   
  1.参数的调整：阈值参数，每个鼠标动作之间的延时时间，图片处理中的参数
  2.AI的更新：
   1.换牌阶段：用数组保存可留下的卡牌名称，其他卡牌直接换
   2.出牌阶段-出牌：卡牌行为为下场的，只要费用够就直接下
   3.出牌阶段-战斗：重新识别自己场上随从，识别对方场上嘲讽怪，有嘲讽怪攻击嘲讽怪，否则打脸
 3.每个阶段结束后移开鼠标并延时，防止鼠标在卡牌上影响下个阶段的判断
目前问题：
 1.卡池太大，不方便全部收集
 2.对对方随从的识别，暂时只识别对方场上嘲讽怪的生命和攻击力，不好识别，尤其是对方随从加buff后
 3.英雄技能和英雄本身攻击未处理
 4.buff类随从，法术未处理

 2016-5-16:
 炉石安装目录的Logs文件夹下有游戏相关数据，能得到当前游戏的各种数据。![https://github.com/HearthSim/Hearthstone-Deck-Tracker]https://github.com/HearthSim/Hearthstone-Deck-Tracker 这个开源项目就是分析这些日志得到的信息    
 打算继续使用图像处理的方法    
更新：
 1.处理英雄技能和攻击，简单的判断了下，如果武器位置与无武器时的背景相差不大就判断有武器，把英雄加入攻击随从；最后点击下英雄技能。     
 需要注意的是要处理的图像的位置和鼠标点击的位置有偏差，偏差的原因是程序边框。    
 2.获取当前场上随从时，发现游戏桌面的颜色会有变化，这样就不能采用原先截出来的背景图片。    
 改为使用在未出牌时截取图片作为背景    
 3.目前可以识别手牌，打出无buff随从，识别对面嘲讽随从，先攻击嘲讽随从，再打脸，使用英雄技能    
 
 2016-5-17:
 目标：战胜普通难度电脑     
 更新：    
 1.调整开始时点击按钮的情况（针对练习模式）    
 2.换牌动画结束的判断不再按照时间，而是判断是否出现确认按钮    
 3.AI调整	
 bug:在识别场上随从时会越界，原因不明      

 2016-5-18：	
 更新:		
 1.bug:出一张牌后，手牌的位置会改变，随从进行攻击死亡后，位置也会改变		
 解决方法：每次出牌后，重新扫描手牌和随从
 2.改变场上随从获取方式。当2个嘲讽随从在一起时，会难以分开，使用轮廓的方法无法分开。
 目前方案：获取二值图像后，从左到右扫描，获取最左边和最右边白色位置，这中间部分即为随从部分，均分即可获得各随从，经测试，该方法准确率高于旧方案。
 3.将一些固定的位置改为宏，放入define文件
 4.修改判断是否是嘲讽怪参数，因为2的改变，场上每个随从的区域固定，直接判断灰色区域是否超过一定值。

 2016-5-19:
 更新：
 1.随从攻击嘲讽随从死亡后，需要重新判断场上随从的位置		
 2.出牌顺序问题（AI）		
 3.判断是否为嘲讽不是非常准确		
 4.增加直伤牌的出法		
 目前可以战胜普通难度

 2016-5-20:		
 更新：		
 1.初始费用为1，费用增长放在回合结束时。（因为在出牌时会return，重新进入该阶段，避免不正确的增加水晶）		
 2.增加一些算法的耗时显示		
 3.图片越界判断，有越界图片时直接跳过		
 4.增加判断是否需要重新扫描		
 5.增加鼠标移动过程，以前是从A点直接移动到B点。		
 待完成：		
 游戏结束的判断

 2016-5-23:		
 更新：		
 1.对游戏结束的判断，暂时没有想到好的方法，采用图像相减的方法，测试代码：			
 ```cpp
	Mat p1 = imread("1.png");
	Mat p2 = imread("2.png");
	cout << processImage->compareImageBySub(p1, p2) << endl;
	cout << processImage->compareImage(p1, p2) << endl;
```			
测试后发现，游戏结束时，compareImageBySub结果会有较大改变		
2.增加自己回合开始时等待时间			
3.当有武器时，不再安装武器			
问题：奥秘，护甲等的判断问题;buf类随从策略

2016-5-24：		
更新：		
1.游戏结束后，多次点击屏幕并自动开始下一局			
2.将每局的图片转为视频保存			
结束判断还有bug			

2016-5-24:			
更新：   
修补bug：    
1.识别是否到自己出牌，原先是在对方回合识别是否到自己出牌，是就sleep，然后直接调用己方函数处理，这样处理的就是旧图片，一定概率会出现漏识别，识别错误     
2.在图像处理函数中：			
```cpp
process()
{
	switch(state)
	{
	case selfPlay:
		a();
		break;
	case otherPlay:
		b();
		isGameover();
		break;
	}
}
b()
{
	a();
}
```
遇到一个bug：当x==a时，程序执行a(),然后到f()的最后，再到b()的最后，再执行c();如果把b函数中调用a()去掉，程序就正常运行。		
bug造成手牌识别后直接判断游戏是否结束，然后手牌挡住结束判断的图片，造成判断失败。不清楚原因，修正方法是在isGameOver总移走鼠标。
