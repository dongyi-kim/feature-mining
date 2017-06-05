#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include<time.h>
#include<opencv2\opencv.hpp>
using namespace cv;

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
int main()
{
	srand( (unsigned)time(NULL) );

    std::ifstream       file("test.csv");

	std::vector<cifar10Data> CifarData;


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

		CifarData.push_back(dummy);

    }
	//////////////////////////////////////////////////////////////////


	Mat img, res, points, labels, centers;

    int width, height, x, y, n, nPoints, cIndex, iTemp;

    const int k = 10;

     

    // 이미지 정보 파악

    
    
    nPoints = CifarData.size();

 

    // 초기화

    points.create(nPoints, 64 , CV_32FC1);        // 입력 데이터

    centers.create(k, 64, points.type());        // k개의 mean 결과값들

    
 

    // kmeans 함수에 맞게 데이터 변환



	for(int n=0; n<CifarData.size(); n++)

    {
		for(int i=0; i<64; i++){
			points.at<float>(n,i) = CifarData[n].vector[i];
			
		}
	}

 

    // k-means clustering

	kmeans(points, k, labels, TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 100, 1.1), 

           1, KMEANS_PP_CENTERS, centers);

 

	int col=0;

	for(int i=0; i<CifarData.size(); i++)
	{
		if(labels.at<int>(i)==(CifarData[i].label-1))
			{
				//printf("Correct! \n");;
				col++;
			}
		else
		{
			//std::cout<<i<<":"<<labels.at<int>(i)<<std::endl;
		}
	}

	printf("\n\nCorrect Num : %d",col);


   




	return 0;
}
