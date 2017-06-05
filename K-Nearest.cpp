#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include<time.h>
#include<opencv2\opencv.hpp>
using namespace cv;
// accuracy
float evaluate(cv::Mat& predicted, cv::Mat& actual) {
    assert(predicted.rows == actual.rows);
    int t = 0;
    int f = 0;
    for(int i = 0; i < actual.rows; i++) {
        float p = predicted.at<float>(i,0);
        float a = actual.at<float>(i,0);
        if(p==a) {
            t++;
        } else {
            
        }
    }
    return (float)(t ) / (float)(actual.rows);
}

class cifar10Data
{
public:
	int label;
	float vector[64];
};
class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            // This checks for a trailing comma with no data after it.
            if (!lineStream && cell.empty())
            {
                // If there was a trailing comma then add an empty element.
                m_data.push_back("");
            }
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
} 

using namespace std;

void knn2(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses, int K) {

    CvKNearest knn(trainingData, trainingClasses, cv::Mat(), false, K);


    cv::Mat predicted(testClasses.rows, 1, CV_32F);



	cout<<testData.rows <<"개 Data 비교 \n";
    for(int i = 0; i < testData.rows; i++) {
            const cv::Mat sample = testData.row(i);
            predicted.at<float>(i,0) = knn.find_nearest(sample, K);
    }

    cout << "Accuracy_{KNN} = " << evaluate(predicted, testClasses) << endl;
 //   plot_binary(testData, predicted, "Predictions KNN");
}


int Setting_KNN_CSV(string fileName, Mat &trainData, Mat &trainClasses,int sampleSize)
{
	srand( (unsigned)time(NULL) );

    std::ifstream       file(fileName);

	std::vector<cifar10Data> CifarData;
	vector<cifar10Data> dividedCifar[10];
	


    CSVRow              row;
    while(file >> row)
    {
		cifar10Data dummy;
		//std::cout << "4th Element(" << row[64] << ")\n";
		dummy.label=  atoi(row[0].c_str());
		for(int q=0; q<64; q++)
		{
			dummy.vector[q] = atof(row[q+1].c_str());
		}
			
			dividedCifar[dummy.label-1].push_back(dummy);
			
		//CifarData.push_back(dummy);

    }
	//////////////////////////////////////////////////////////////////
	



	 

    int train_sample_count = sampleSize;
	int K =train_sample_count/100;

  
 
    

	int startNum=0;
	for(int n=0 , toNum=0,i=0; i<10; i++)
	{
		for(int w=0; w<dividedCifar[i].size(); w++,n++)
		{
			for(int q=0; q<64; q++ )
			{

				trainData.at<float>(n,q)=dividedCifar[i][w].vector[q];

			}
			trainClasses.at<float>(n,0)=(float)dividedCifar[i][w].label;
			

		}
		
		
		
	}





   
  
    return 0;


}

int main()
{
	int train_sample_count=50000;
	Mat trainData = cvCreateMat( train_sample_count, 64, CV_32FC1 );
    Mat trainClasses = cvCreateMat( train_sample_count, 1, CV_32FC1 );
	Setting_KNN_CSV("train.csv",trainData,trainClasses,train_sample_count);

	cout<<trainData.at<float>(0,0);

	train_sample_count=10000;
	Mat testData = cvCreateMat( train_sample_count, 64, CV_32FC1 );
    Mat testClasses = cvCreateMat( train_sample_count, 1, CV_32FC1 );
	Setting_KNN_CSV("test.csv",testData,testClasses,train_sample_count);


	for(int i=0; i<10000; i+=70)
	{
		cout<<"label"<<endl;
		cout<<testClasses.at<float>(i,0)<<endl;
	}
	cout<<testData.at<float>(0,0);
	

	knn2(trainData,trainClasses,testData,testClasses,2);
	

}












