#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "crypt32.lib")
#endif

#include "Scraper.h"
#include "Converter.h"

#include <set>
#include <filesystem>
#include <fstream>
#include <cpr/cpr.h>
#include <fmt/format.h>

const std::filesystem::path data_directory = "Data/";
const std::filesystem::path json_filename = "Data/Movies.json";
const std::filesystem::path images_directory = "Images/";

std::vector<Movie> generate(Scraper &s)
{
	static bool invoked = false;
	if (invoked)
		return {};

	if (!std::filesystem::exists(data_directory))
		std::filesystem::create_directory(data_directory);

	s.scrap();
	auto movies = s.getMovies();

	std::ofstream out(json_filename);
	nlohmann::json Json{};
	Json["smallest_date"] = s.getSmallestTime();
	for (const auto& movie : movies)
	{
		Json["movies"].push_back(movie);
	}
	out << Json.dump(2) << '\n';

	fmt::print(fg(fmt::color::lime), "Helios (Gdańsk-Metropolia) movie repertoire is scraped to file {}", json_filename.string());
	invoked = true;
	return movies;
}

bool download_images(std::vector<Movie>& movies)
{
	static bool invoked = false;
	if (invoked)
		return false;

	if (!std::filesystem::exists(images_directory))
		std::filesystem::create_directory(images_directory);

	std::set<std::string_view> hashes;
	for (const auto& movie : movies)
	{
		if (hashes.count(movie.image_hash))
			continue;

		auto& url = movie.image_url;
		auto filename = images_directory;
		filename += movie.image_hash + ".jpeg";

		auto response = cpr::Get(cpr::Url{ movie.image_url });
		if (!cpr::status::is_success(response.status_code))
		{
			fmt::print(fg(fmt::color::dark_red), "ERROR: Can't download image from {}", movie.image_url);
			fmt::print("\n");
			return false;
		}
		std::ofstream image(filename, std::ios::binary);
		image.write(response.text.data(), response.text.size());
		fmt::print("Downloaded image for movie: ");
		fmt::print(fg(fmt::color::red), "{}\n", movie.name);
	}
	invoked = true;
	return true;
}

bool convert(std::vector<Movie>& movies, Movie::Time smallest_time)
{
	static bool invoked = false;
	if (invoked)
		return false;

	Converter conv{ movies };
	auto categories_csv = data_directory;
	categories_csv += "categories.csv";
	auto products_csv = data_directory;
	products_csv += "products.csv";
	auto combinations_csv = data_directory;
	combinations_csv += "combinations.csv";

	conv.generateColumnsCSV(categories_csv);
	conv.generateProductsCSV(products_csv);
	conv.generateCombinationsCSV(combinations_csv, smallest_time);

	invoked = true;
	return true;
}


int main(int argc,  char** argv)
{
	std::string_view directory{ argv[0] };
	std::vector<std::string_view> flags{ argv + 1, argv + argc };
	Scraper s;
	auto movies = s.getMovies();
	auto smallest_date = s.getSmallestTime();

	auto flag_generate = std::find(flags.begin(), flags.end(), "-G");
	if (flag_generate != flags.end())
	{
		movies = generate(s);
	}
	
	if (!movies.size())
	{
		if (!std::filesystem::exists(json_filename))
		{
			fmt::print("File {} was not found!\n", json_filename.string());
			fmt::print(R"(Try execute '{} -D' to create {})", "Scraper.exe", json_filename.string());
			fmt::print("\n");
			return 404;
		}

		nlohmann::json JSON{};
		std::ifstream in(json_filename);
		in >> JSON;
		smallest_date = JSON["smallest_date"];
		JSON["movies"].get_to(movies);
	}

	auto flag_images = std::find(flags.begin(), flags.end(), "-I");
	if (flag_images != flags.end())
		download_images(movies);
	
	auto flag_convert = std::find(flags.begin(), flags.end(), "-C");
	if (flag_convert != flags.end())
		convert(movies, smallest_date);

	return 0;
}