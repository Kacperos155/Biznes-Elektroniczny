#pragma once
#include "Movie.h"
#include <string_view>
#include <string>
#include <array>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/color.h>

class Converter
{
	std::vector<Movie> movies;
	const std::string images_directory = "http://localhost/Images/";

public:
	Converter(std::vector<Movie>& movies);
	Converter(std::vector<Movie>&& movies);

	bool generateColumnsCSV(std::filesystem::path filename);
	bool generateProductsCSV(std::filesystem::path filename);
	bool generateCombinationsCSV(std::filesystem::path filename, Movie::Time smallest_time);
};

