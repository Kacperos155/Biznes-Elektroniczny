#pragma once

#include "Movie.h"
#include <string_view>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <charconv>

#include <fmt/format.h>
#include <fmt/color.h>
#include <cpr/cpr.h>
#include <gumbo.h>

class Scraper
{
	const std::string_view url_repertoire = "https://www.helios.pl/49,Gdansk/Repertuar/index/dzien/{}/kino/49";
	const std::string_view url_movie = "https://www.helios.pl/49,Gdansk/BazaFilmow/Szczegoly/film/{}";

	std::map<unsigned, Movie> movies{};
	std::vector<std::thread> threads;
	std::mutex movie_write;
	Movie::Time smallest_date{ 100 };

	bool scrapMovie(unsigned ID, std::string&& buffer);
	bool scrapMovieDetails(GumboNode* node, std::string_view buffer, Movie& movie);
	bool scrapMovieTimes(GumboNode* node, std::string_view buffer, Movie& movie);
	bool scrapMoviesID(std::string_view buffer);

	void checkSmallestDate(Movie::Time time);

	template<typename Function>
	static bool searchForClass(GumboNode* node, std::string_view class_name, std::string_view buffer, Function&& function);
	[[nodiscard]] static std::string_view getBodyOfTag(GumboNode* node, std::string_view buffer);
	static std::string_view extractCleanString(std::string_view str);
	static const uint8_t convDayOfWeek(std::string_view day_of_week);
	static const uint8_t convMonth(std::string_view month);

public:
	void scrap();
	std::vector<Movie> getMovies();
	const Movie::Time getSmallestTime() const;
};



template<typename Function>
inline bool Scraper::searchForClass(GumboNode* node, std::string_view class_name, std::string_view buffer, Function&& function)
{
	if (node == nullptr)
	{
		fmt::print(bg(fmt::color::dark_red), "ERROR: Class {} was not found", class_name);
		fmt::print("\n");
		return false;
	}
	if (node->type != GUMBO_NODE_ELEMENT) {
		return false;
	}

	GumboAttribute* class_attr;
	if (class_attr = gumbo_get_attribute(&node->v.element.attributes, "class")) {
		if (class_attr->value == class_name)
			function(node);
	}

	GumboVector* children = &node->v.element.children;
	for (unsigned int i = 0; i < children->length; ++i) {
		searchForClass(static_cast<GumboNode*>(children->data[i]), class_name, buffer, function);
	}
	return true;
}
