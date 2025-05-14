#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string> getHumanData()
{
	std::vector<std::string> data;
	
	std::ifstream in;
	in.open("humans.txt");

	for(int i = 0; i < 6; i++)
	{
		std::string n;
		in >> n;
		data.push_back(n);
	}

	return data;
}

void appendDataHumans(int scoreP1, int scoreP2)
{
	std::vector<std::string> data = getHumanData();

	std::ofstream out;
	out.open("humans.txt");

	if (!out)
	{
		std::cout << "file humans.txt not opening\n";
		return;
	}

	out << scoreP1 << " " << scoreP2 << std::endl;

	for (int i = 0; i < data.size() - 2; i += 2)
		out << data[i] << " " << data[i + 1] << std::endl;

	out.close();

	return;
}

std::vector<std::string> getAiData()
{
	std::vector<std::string> data;

	std::ifstream in;
	in.open("ai.txt");

	for (int i = 0; i < 6; i++)
	{
		std::string n;
		in >> n;
		data.push_back(n);
	}

	return data;
}

void appendDataAi(int scoreP1, int scoreP2)
{
	std::vector<std::string> data = getAiData();

	std::ofstream out;
	out.open("ai.txt");

	if (!out)
	{
		std::cout << "file ai.txt not opening\n";
		return;
	}

	out << scoreP1 << " " << scoreP2 << std::endl;

	for (int i = 0; i < data.size() - 2; i += 2)
		out << data[i] << " " << data[i + 1] << std::endl;

	out.close();

	return;
}