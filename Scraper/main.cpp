#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

#include "Scraper.h"
#include <iostream>
#include <fstream>

bool generate(Scraper &s)
{
	static bool invoked = false;
	if (invoked)
		return false;

	s.scrap();
	auto& m = s.getMovies();

	std::string_view filename = "Movies.json";
	std::ofstream out(filename.data());
	nlohmann::json Json;
	for (const auto& [key, value] : m)
	{
		Json = value;
		out << Json.dump(2) << '\n';
	}

	fmt::print(fg(fmt::color::lime), "Helios (Gdańsk-Metropolia) movie repertoire is scraped to file {}", filename);
	invoked = true;
	return true;
}

bool convert(Scraper& s)
{
	static bool invoked = false;
	if (invoked)
		return false;


	invoked = true;
	return true;
}


int main(int argc,  char** argv)
{
	std::string_view directory{ argv[0] };
	std::vector<std::string_view> flags{ argv + 1, argv + argc };
	Scraper s;

	for (const auto& v : flags)
	{
		if (v == "-D")
			generate(s);
		if (v == "-C")
			convert(s);
	}
	return 0;
}