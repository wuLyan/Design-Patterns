#include <string>
#include <iostream>
#include <vector>
using std::string;
using std::vector;

class IProgress{
public:
	virtual void DoProgress(float value) = 0;
	virtual ~IProgress(){}
};


class FileSplitter
{
	string m_filePath;
	int m_fileNumber;

	// ProgressBar* m_progressBar; // 具体通知控件，耦合性高
	vector<IProgress*>  m_iprogressList; // 抽象通知机制，支持多个观察者
	
public:
	FileSplitter(const string& filePath, int fileNumber) :
		m_filePath(filePath), 
		m_fileNumber(fileNumber){

	}


	void split(){

		//1.读取大文件

		//2.分批次向小文件中写入
		for (int i = 0; i < m_fileNumber; i++){
			//...

			float progressValue = m_fileNumber;
			progressValue = (i + 1) / progressValue;
			onProgress(progressValue); //发送通知
		}

	}


	void addIProgress(IProgress* iprogress){
		m_iprogressList.push_back(iprogress);
	}

	void removeIProgress(IProgress* iprogress){
		// remove，类似于这行语句
		// m_iprogressList.remove(iprogress); // 没有remove()成员函数
	}

protected:
	virtual void onProgress(float value){
		
		vector<IProgress*>::iterator itor = m_iprogressList.begin();

		// 依次通知每一个观察者
		while (itor != m_iprogressList.end() ){
			(*itor)->DoProgress(value); // 每一个观察者自更新
			itor++;
		}
	}
};