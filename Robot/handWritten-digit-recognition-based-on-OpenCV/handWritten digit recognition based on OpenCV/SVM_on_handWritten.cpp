#include"SVM_on_handWritten.h"


void mySVM::loadSVMData(vector<Mat>& srcImageVec, Mat& dstDataMat, getSVMfeature fun, Size size)
{
	for (int i = 0; i < srcImageVec.size(); ++i)
	{
		Mat tmpImage;
		fun(srcImageVec[i], tmpImage, size);
		dstDataMat.push_back(tmpImage);//注意tmpImage为CV_32FC1
	}
}

void mySVM::loadSVMLabel(const vector<int>& srclabelVec, Mat& dstLabelMat)
{
	Mat(srclabelVec).copyTo(dstLabelMat);
	dstLabelMat.convertTo(dstLabelMat, CV_32SC1);//重点！！！不是32FC1！！！！
}




void mySVM::trainSVM(const string& fileName)
{
	if (svm->isTrained())
	{
		return;
	}
	Ptr<TrainData> traindata = TrainData::create(trainDataMat, ROW_SAMPLE, trainLabelMat);
	svm->setType(SVM::C_SVC);

	//LINER
	//svm->setKernel(SVM::LINEAR);
	//svm->setC(10);	

	//RBF with trainAuto
	//svm->setKernel(SVM::RBF);
	//svm->setC(1);
	//svm->setGamma(1e-3);
	//svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 0.001));

	//svm->trainAuto(traindata, 10,
	//	svm->getDefaultGrid(SVM::ParamTypes::C),
	//	svm->getDefaultGrid(SVM::ParamTypes::GAMMA),
	//	ParamGrid::ParamGrid(1, 1, 0.0), 
	//	ParamGrid::ParamGrid(1, 1, 0.0),
	//	ParamGrid::ParamGrid(1, 1, 0.0),
	//	ParamGrid::ParamGrid(1, 1, 0.0));

	//RBF 
	svm->setKernel(SVM::RBF);
	svm->setC(2.5);
	svm->setGamma(3.3e-2);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 0.001));

	svm->train(traindata);

	svm->save(fileName);
}

void mySVM::getSVM(const string & fileName)
{
	svm = Algorithm::load<SVM>(fileName);
}

float mySVM::testSVM(Mat& testingDataMat, vector<int>& testingLabelVec)
{
	int error = 0;
	int sum = testingLabelVec.size();
	for (int i = 0; i < testingLabelVec.size(); ++i)
	{
		float predictAnswer = svm->predict(testingDataMat.row(i));
		if ((int)predictAnswer != testingLabelVec[i])
		{
			error++;
		}
	}
	return (error / (float)sum);
}

void mySVM::testSVM(const string& fileName)
{
	if (!svm->isTrained())
	{
		cout << 1 << endl;
		getSVM(fileName);
	}
	float trainTestErrorRate = testSVM(trainTestDataMat, trainTestLabelVec);
	float testErrorRate = testSVM(testDataMat, testLabelVec);
	cout << "trainTest error rate:" << trainTestErrorRate << endl;
	cout << "test error rate:" << testErrorRate << endl;
}

void mySVM::trainTestSVM(const string& fileName)
{
	printf("training SVM, please wait");
	trainSVM(fileName);
	printf("\r");
	showSVMInfo();
	testSVM(fileName);
}

mySVM::mySVM(int trainNumber, int testNumber, int trainTestNumber, getSVMfeature fun, Size size)
{
	//读取MNIST数据
	loadMNISTData(trainImageVec, trainLabelVec, trainNumber, testImageVec, testLabelVec, testNumber);
	loadMNISTData(trainTestImageVec, trainTestLabelVec, trainTestNumber, testImageVec, testLabelVec, 0);
	////测试读取是否成功
	//testFileLoad(trainImageVec, trainLabelVec);
	//getchar();


	//SVM数据转换
	loadSVMData(trainImageVec, trainDataMat, fun, size);
	loadSVMLabel(trainLabelVec, trainLabelMat);

	loadSVMData(testImageVec, testDataMat, fun, size);
	loadSVMLabel(testLabelVec, testLabelMat);

	loadSVMData(trainTestImageVec, trainTestDataMat, fun, size);
	loadSVMLabel(trainTestLabelVec, trainTestLabelMat);

	svm = SVM::create();
}


void mySVM::showSVMInfo()
{
	cout << "number of supporting vectors: " << svm->getSupportVectors().rows << endl;
	cout << "param C: " << svm->getC() << endl;
	cout << "param gama: " << svm->getGamma() << endl;
}
