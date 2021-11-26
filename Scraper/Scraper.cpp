#include "Scraper.h"

std::string_view Scraper::getBodyOfTag(GumboNode* node, std::string_view buffer)
{
	auto start = node->v.element.start_pos.offset;
	start = buffer.find(">", start) + 1;
	auto offset = node->v.element.end_pos.offset - start;
	if (offset <= 0)
		return "";

	auto body = buffer.substr(start, offset);
	auto body_end = body.rfind("<");
	if (body_end == std::string::npos)
		return body;
	body = body.substr(0, body_end);
	return body;
}

std::string_view Scraper::extractCleanString(std::string_view str)
{
	auto whitespaces = [](char i)
	{
		return ((0 < i) && (i <= ' '));
	};

	auto start = std::find_if_not(str.begin(), str.end(), whitespaces);
	if (start == str.end())
		start = str.begin();
	str.remove_prefix(std::distance(str.begin(), start));

	auto r_end = std::find_if_not(str.rbegin(), str.rend(), whitespaces);
	if (r_end == str.rend())
		r_end = str.rbegin();
	str.remove_suffix(std::distance(str.rbegin(), r_end));

	return str;
}

const uint8_t Scraper::convDayOfWeek(std::string_view day_of_week)
{
	if (day_of_week == "Pn")
		return 0;
	if (day_of_week == "Wt")
		return 1;
	if (day_of_week == u8"Śr")
		return 2;
	if (day_of_week == "Cz")
		return 3;
	if (day_of_week == "Pt")
		return 4;
	if (day_of_week == "So")
		return 5;
	if (day_of_week == "Nd")
		return 6;
	return -1;
}

const uint8_t Scraper::convMonth(std::string_view month)
{
	if (month == "sty")
		return 0;
	if (month == "lut")
		return 1;
	if (month == "mar")
		return 2;
	if (month == "kwi")
		return 3;
	if (month == "maj")
		return 4;
	if (month == "cze")
		return 5;
	if (month == "lip")
		return 6;
	if (month == "sie")
		return 7;
	if (month == "wrz")
		return 8;
	if (month == u8"paź")
		return 9;
	if (month == "lis")
		return 10;
	if (month == "gru")
		return 11;
	return -1;
}

bool Scraper::scrapMovie(unsigned ID, std::string&& buffer)
{
	Movie movie;
	movie.ID = ID;

	auto html_page = gumbo_parse(buffer.data());
	GumboNode* html_content{};

	auto cleanAfterError = [&]() 
	{
		movies.erase(ID);
		return false;
	};

	auto restrictToContent = [&](GumboNode* node)
	{
		html_content = node;
		auto body = getBodyOfTag(node, buffer);
		if (body.find("dolby atmos") != std::string::npos)
			movie.dolby = true;
		auto start = body.find("Opis filmu");
		if (start != std::string::npos)
		{
			start = body.find("<p>", start) + 3;
			auto end = body.find("</p>", start);
			movie.description = extractCleanString(body.substr(start, end - start));
		}
	};
	if (!searchForClass(html_page->root, "content-row set-3columns", buffer, restrictToContent))
		return cleanAfterError();

	if (!scrapMovieDetails(html_content, buffer, movie))
		return cleanAfterError();
	if (!scrapMovieTimes(html_content, buffer, movie))
		return cleanAfterError();

	gumbo_destroy_output(&kGumboDefaultOptions, html_page);
	{
		std::lock_guard guard(movie_write);
		movies.at(ID) = movie;
		fmt::print("Movie: ");
		fmt::print(fmt::emphasis::italic | fg(fmt::color::red),"{}", movie.name);
		fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),"({})", movie.ID);
		fmt::print(" sucesfuly scraped\n");
	}
	return true;
}

bool Scraper::scrapMovieDetails(GumboNode* node, std::string_view buffer, Movie& movie)
{
	auto getTitle = [&](GumboNode* node)
	{
		auto title = getBodyOfTag(node, buffer);
		auto title_end1 = title.find("/");
		auto title_end2 = title.find("<");
		auto title_end = (std::min)(title_end1, title_end2);
		movie.name = title.substr(0, title_end);
		if (title_end != std::string::npos)
		{
			title = title.substr(title_end + 2);
			if (title.find("familijny") != std::string::npos)
				movie.family_friendly = true;
			if (title.find("napisy") != std::string::npos)
				movie.type = Movie::Type::SUBTITLES;
			else if (title.find("dubbing") != std::string::npos)
				movie.type = Movie::Type::DUBBING;
		}
		movie.name = extractCleanString(movie.name);
	};
	if (!searchForClass(node, "movie-title", buffer, getTitle))
		return false;

	auto getImage = [&](GumboNode* node)
	{
		auto body = getBodyOfTag(node, buffer);
		auto img_start = body.find(R"(<img src=")");
		if (img_start != std::string::npos)
		{
			img_start += 10;
			auto img_end = body.find("\"", img_start);
			movie.image_url = body.substr(img_start, img_end - img_start);
		}
		return;
	};
	if (!searchForClass(node, "btn-round btn-play show-trailers", buffer, getImage))
		return false;

	auto getDetails = [&](GumboNode* node)
	{
		auto body = getBodyOfTag(node, buffer);
		auto ct_it = body.find("Czas trwania: ");
		if (ct_it != std::string::npos)
		{
			ct_it += 14;
			auto end = body.find(" ", ct_it);
			auto duration = body.substr(ct_it, end - ct_it);
			std::from_chars(duration.data(), duration.data() + duration.size(), movie.duration);
		}
		auto odl_it = body.find("Od lat: ");
		if (odl_it != std::string::npos)
		{
			odl_it += 8;
			auto end = body.find(" ", odl_it);
			auto from_age = body.substr(odl_it, end - odl_it);
			std::from_chars(from_age.data(), from_age.data() + from_age.size(), movie.from_age);
		}
	};
	if (!searchForClass(node, "details", buffer, getDetails))
		return false;
	return true;
}

bool Scraper::scrapMovieTimes(GumboNode* node, std::string_view buffer, Movie& movie)
{
	Movie::Time day;
	auto getDays = [&](GumboNode* node)
	{
		if (movie.ID == 21748)
			fmt::print("");
		auto body = getBodyOfTag(node, buffer);
		auto day_it = body.find("day-name\">");
		if (day_it != std::string::npos)
		{
			day_it += 10;
			day.day_of_week = convDayOfWeek(body.substr(day_it, body.find("<", day_it) - day_it));
		}
		day_it = body.find("day-number\">", day_it);
		if (day_it != std::string::npos)
		{
			day_it += 12;
			auto d_day = body.substr(day_it, body.find("<", day_it) - day_it);
			std::from_chars(d_day.data(), d_day.data() + d_day.size(), day.day);
		}
		day_it = body.find("month-name\">", day_it);
		if (day_it != std::string::npos)
		{
			day_it += 12;
			day.month = convMonth(body.substr(day_it, body.find("<", day_it) - day_it));
		}

		auto times_it = body.find("</a>", day_it);
		while (times_it != std::string::npos)
		{
			Movie::Time time = day;
			body.remove_prefix(times_it - 5);
			if (body[0] == '>')
				body.remove_prefix(1);
			times_it = body.find('<');
			auto time_text = std::string_view{ body.data(), times_it };

			auto separator = time_text.find(":");
			if (separator != std::string::npos)
			{
				auto hour_sv = time_text.substr(0, separator);
				std::from_chars(hour_sv.data(), hour_sv.data() + hour_sv.size(), time.hour);
				auto minutes = time_text.substr(separator + 1);
				std::from_chars(minutes.data(), minutes.data() + minutes.size(), time.minutes);
				movie.times.push_back(time);
			}

			body.remove_prefix(times_it + 1);
			times_it = body.find("</a>", times_it);
		}

	};
	if (!searchForClass(node, "day", buffer, getDays))
		return false;
	return true;
}

bool Scraper::scrapMoviesID(std::string_view buffer)
{
	std::string movie_buffor{};
	auto html_page = gumbo_parse(buffer.data());

	std::vector<GumboNode*> html_movies{};
	auto search_movies_div = [&](GumboNode* node)
	{
		html_movies.push_back(node);
	};
	if (!searchForClass(html_page->root, "movie", buffer, search_movies_div))
		return false;

	auto search_movie_id = [&](GumboNode* node)
	{
		auto body = getBodyOfTag(node, buffer);
		auto it = body.find("/film/");
		it += 6;
		auto it_end = body.find("/", it);
		body = body.substr(it, it_end - it);

		auto ID = unsigned{};
		std::from_chars(body.data(), body.data() + body.size(), ID);
		if (movies.count(ID) == 0)
		{
			movies.emplace(std::make_pair(ID, Movie{}));

			auto url = cpr::Url(fmt::format(url_movie, ID));
			auto response = cpr::Get(url);
			movie_buffor = response.text;
			if (!cpr::status::is_success(response.status_code))
				fmt::print("ERROR at scraping {}\n", url.c_str());
				
			else
			{
				fmt::print("\tScraping: {}\n", url.c_str());
				if (threads.size() >= 4) {
					for (auto& thread : threads)
						thread.join();
					threads.clear();
				}
				threads.emplace_back(&Scraper::scrapMovie, this, ID, std::move(movie_buffor));
			}
		}

	};
	for (const auto& movie : html_movies)
	{
		if (!searchForClass(movie, "movie-media", buffer, search_movie_id))
			return false;
	}
	gumbo_destroy_output(&kGumboDefaultOptions, html_page);
	return true;
}

void Scraper::scrap()
{

	for (int i = 0; i < 12; ++i)
	{
		auto url = cpr::Url(fmt::format(url_repertoire, i));
		auto response = cpr::Get(url);
		if (cpr::status::is_success(response.status_code))
		{
			fmt::print("\tScraping: {}\n", url.c_str());
			scrapMoviesID(response.text);
			fmt::print("Page ");
			fmt::print(fg(fmt::color::yellow), "{}", i);
			fmt::print(" of repertoire is sucesfuly scraped\n", i);
		}
		else
			fmt::print("ERROR at scraping: {}\n", url.c_str());
	}
	for (auto& thread : threads)
		thread.join();
}

std::map<unsigned, Movie>& Scraper::getMovies()
{
	return movies;
}
