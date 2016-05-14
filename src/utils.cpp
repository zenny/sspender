#include "utils.h"

void getAllDisksAndPartitions(vector<string> *disks, vector<string> *partitions)
{
	vector<string> fileContent;
	string fileName = "/proc/partitions";

	parseFile(fileName, &fileContent);

	for(size_t i = 1, size = fileContent.size(); i < size; ++i)
	{
		string line = fileContent[i];
		vector<string> splitLine;

		splitByEmptySpace(line, &splitLine);

		if(splitLine.size() == 4)
		{
			int deviceMinor = atoi(splitLine[1].c_str());

			if(deviceMinor == 0)
			{
				disks->push_back(splitLine[3]);
			}
			else
			if(deviceMinor > 0)
			{
				partitions->push_back(splitLine[3]);
			}
		}
	}
}

bool convertTimeToMinutes(string time, double *totalMinutes)
{
	replace(time.begin(), time.end(), ':', ' ');

	vector<string> splitTime;
	splitByEmptySpace(time, &splitTime);

	if(splitTime.size() < 1)
	{
		cout << "Failed to convert time to minutes: " << time << endl;
		return false;
	}

	*totalMinutes += atoi(splitTime[0].c_str()) * 60;
	*totalMinutes += atoi(splitTime[1].c_str());

	return true;
}

bool getCurremtTimeInMinutes(double *totalMinutes)
{
	vector<string> dateOutput;
	runSystemCommand("date +%R", &dateOutput);

	bool gotTimeInMinutes = convertTimeToMinutes(dateOutput[0], totalMinutes);

	return gotTimeInMinutes;
}

void printHeaderMessage(const string &message, bool logTime)
{
	if(logTime)
	{
		cout << "=====|| " << message << " | " << getCurrentDateTime() << endl;
	}
	else
	{
		cout << "\n==========|| " << message << endl;
	}
}

void splitByEmptySpace(const string &input, vector<string> *output)
{
	istringstream iss(input);

	copy(istream_iterator<string>(iss),
		 istream_iterator<string>(),
		 back_inserter(*output));
}

string trimString(string input)
{
	string::const_iterator bItr = input.begin();

	while(bItr != input.end() && isspace(*bItr))
	{
		++bItr;
	}

	string::const_reverse_iterator eItr = input.rbegin();

	while(eItr.base() != bItr && isspace(*eItr))
	{
		eItr++;
	}

	string trimedString = string(bItr, eItr.base());

	return trimedString;
}

bool runSystemCommand(const string &command, vector<string> *output /* = null*/)
{
	FILE *in;
	char buff[512];

	if(!(in = popen(command.c_str(), "r")))
	{
		return false;
	}

	if(output)
	{
		while(fgets(buff, sizeof(buff), in) != NULL)
		{
			output->push_back(buff);
		}
	}

	pclose(in);
	return true;
}

double getDoubleFromSystemCall(const string &command)
{
	FILE *in;
	char buff[512];

	if(!(in = popen(command.c_str(), "r")))
	{
		return -1;
	}

	double value;

	while(fgets(buff, sizeof(buff), in) != NULL)
	{
		value = atof(string(buff).c_str());
	}

	pclose(in);
	return value;
}

void splitStringByDelimiter(vector<string> *output,
		                    const string &input,
		                    const string &delimiter)
{
	size_t last = 0;
	size_t next = 0;

	while ((next = input.find(delimiter, last)) != string::npos)
	{
		output->push_back(input.substr(last, next-last));
		last = next + 1;
	}

	output->push_back(input.substr(last));
}

string getCurrentDateTime()
{
	vector<string> dateOutput;

	runSystemCommand("date", &dateOutput);

	dateOutput[0].erase(std::remove(dateOutput[0].begin(), dateOutput[0].end(), '\n'), dateOutput[0].end());

	return dateOutput[0];
}

void parseFile(const string &fileName, vector<string>* fileOutput)
{
	string line;

	ifstream file (fileName.c_str());

	if (file.is_open())
	{
		while ( getline(file,line) )
		{
			fileOutput->push_back(line);
		}

		file.close();
	}
	else
	{
		cout << "Could not open " << fileName << endl;
	}
}

void parseFile(ifstream &file, vector<string>* fileOutput)
{
	string line;

	if (file.is_open())
	{
		file.clear();
		file.seekg(0, ios::beg);

		while ( getline(file,line) )
		{
			fileOutput->push_back(line);
		}
	}
	else
	{
		cout << "ifstream is not open!!" << endl;
	}
}