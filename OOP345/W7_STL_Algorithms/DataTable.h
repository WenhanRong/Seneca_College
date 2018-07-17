#ifndef W8_DATATABLE_H
#define W8_DATATABLE_H

namespace w8 {
	template<typename T>
	class DataTable {
		int width;
		int precision;
		int n;
		std::vector<std::vector<T>> data;
	public:
		DataTable(std::ifstream& is, int width, int ND) {
			this->width = width;
			this->precision = ND;
			size_t pos = 0u;
			n = getFileLine(is);
			is.clear();
			is.seekg(0, std::ios::beg);
			std::string line;
			data.resize(n, std::vector<T>(2, 0));
			for (int i = 0; i < n; i++)
			{
				std::getline(is, line);
				pos = line.find(' ');
				data[i][0] = (T)std::stod(line.substr(0, pos));
				line.erase(0, pos + 1);
				data[i][1] = (T)std::stod(line);
			}
		}
		//returns the mean value of the dependent coordinate
		T mean() const {
			T mean = (T)0;
			for (int i = 0; i < n; i++)
			{
				mean += data[i][1];
			}
			return mean / n;
		}
		//returns the standard deviation of the dependent coordinates
		T sigma() const {
			T sigma = (T)0;
			for (int i = 0; i < n; i++)
			{
				sigma += (T)pow(data[i][1]-mean(),2);
			}
			return (T)sqrt((sigma / (T)(n - 1)));
		}
		//returns the median value of the dependent coordinate
		T median() const {
			T* temp=new T[n];
			T median;
			for (int i = 0; i < n; i++)
			{
				temp[i] = data[i][1];
			}
			std::sort(temp, temp + n);
			median = temp[n / 2];
			delete[] temp;
			return median;
		}
		//returns the slope and intercept for the data set
		void regression(T& slope, T& y_intercept) const {
			T xSum = (T)0;
			T x2Sum = (T)0;
			T xySum = (T)0;
			T ySum = mean()*n;
			for (int i = 0; i < n; i++)
			{
				xSum += data[i][0];
				x2Sum += (T)pow(data[i][0],2);
				xySum += data[i][0] * data[i][1];
			}
			slope = (n*xySum - xSum * ySum) / (n*x2Sum - (T)pow(xSum, 2));
			y_intercept = (ySum - slope * xSum) / n;
		}
		//displays the data pairs as shown below
		void display(std::ostream& os) const {
			os << std::setw(width) << std::fixed << "x";
			os << std::setw(width) << std::fixed << "y";
			os << std::endl;
			os.precision(precision);
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < 2; j++) {
					os << std::setw(width) << std::fixed << data[i][j];
				}
				os << std::endl;
			}
		}
		//inserts the data generated by display() into the output stream
		friend std::ostream& operator<<(std::ostream& os, const DataTable& data) {
			data.display(os);
			return os;
		}
		//this function return the number of line of the parameter file.
		int getFileLine(std::ifstream& fin) {
			std::string temp;
			int numberOfLines = 0;
			while (std::getline(fin, temp))
			{
				numberOfLines++;
			}
			return numberOfLines;

		}
	};
}
#endif // !W8_DATATABLE_H