#include"mycore.h"

int main()
{
	//训练SVM
	//mySVM svmOnHand(60000, 10000, 60000, getMixedFeature, Size(28, 28));
	//svmOnHand.testSVM(g_SVM);
	//getchar();

	cout << "手写数字识别 版本1.0" << endl;
	int flag = 0;
	while (cout << "请选择所需要的功能: 1 手写数字并识别 2 从图片中识别数字, 0 退出" << endl, scanf("%d", &flag), flag)
	{
		if (flag == 1)
		{
			handWritingRecognize();
		}
		else if (flag == 2)
		{
			cout << "图片尽量清晰，除手写数字外不要有其余图案，手写数字尽量连笔，详情可参考测试图片" << endl;
			imageRecognize();
		}
	}


	return 0;
}


