#include "Converter.h"

Converter::Converter(std::vector<Movie>& movies)
	:movies(movies) {}

Converter::Converter(std::vector<Movie>&& movies)
	: movies(std::move(movies)) {}

bool Converter::generateColumnsCSV(std::filesystem::path filename)
{
	std::ofstream csv(filename);
	if (!csv.is_open())
		return false;

	fmt::print(csv, "ID;Aktywny(0 lub 1);Nazwa*;Kategoria nadrzędna;Główna kategoria(0 / 1)\n");
	const auto format_parent = std::string_view{ "{};1;{};;1\n" };
	const auto format = std::string_view{ "{};1;{};Repertuar;0\n" };
	fmt::print(csv, format_parent, 3, "Repertuar");
	fmt::print(csv, format, 4, "Dzisiaj");
	fmt::print(csv, format, 5, "Familijny");
	fmt::print("Exported columns to {}\n", filename.string());
;	return true;
}

bool Converter::generateProductsCSV(std::filesystem::path filename)
{
	if (!movies.size())
		return false;

	std::ofstream csv(filename);
	if (!csv.is_open())
	{
		fmt::print(fg(fmt::color::red), "ERROR: Can't write to file {}", filename.string());
	}

	fmt::print(csv, "ID;Aktywny(0 lub 1);Nazwa*;Kategorie(x, y, z...);" \
		"Cena zawiera podatek. (brutto); ID reguły podatku; W sprzedaży(0 lub 1);" \
		"Procent rabatu; Rabat od dnia(rrrr - mm - dd); Rabat do dnia(rrrr - mm - dd);" \
		"Cecha(Nazwa:Wartość:Pozycja:Indywidualne); " \
		"Adresy URL zdjęcia(x, y, z...); Opis\n");

	const auto format_no_sale = std::string_view{ "{};1;{};{};{:.2f};1;1;;;;{};{};{}\n" };
	const auto format_christmas_sale = std::string_view{ "{};1;{};{};{:.2f};1;1;20;2021-11-26;2021-12-28;{};{};{}\n" };

	const auto current_day = movies[0].times[0].day;

	for (const auto& movie : movies)
	{
		auto format = format_no_sale;
		if (movie.name.find("- Seans mikołajkowy") != std::string::npos)
			format = format_christmas_sale;

		auto price = 26.90f;
		auto categories = std::string{"Repertuar, "};
		if (movie.times[0].day == current_day)
		{
			categories += "Strona Główna, ";
			categories += "Dzisiaj, ";
		}
		if (movie.family_friendly)
		{
			price = 23.90f;
			categories += "Familijny, ";
		}
		categories = categories.substr(0, categories.size() - 2);

		auto name = movie.name;
		if (movie.type != Movie::Type::POLISH)
		{
			if (movie.type == Movie::Type::DUBBING)
				name += " / DUBBING";
			else if (movie.type == Movie::Type::SUBTITLES)
				name += " / NAPISY";
		}

		auto image = images_directory + movie.image_hash + ".jpeg ";

		auto attributes = fmt::format("Czas trwania:{} minut:0:0", movie.duration);
		attributes += fmt::format(", Od lat:{}:1:0", movie.from_age);
		if(movie.dolby)
			attributes += fmt::format(", Dolby Atmos:TAK:2:0");
		attributes += fmt::format(", Data:00-00:3:0");
		attributes += fmt::format(", Godzina:00-00:4:0");

		auto description = '\"' + movie.description + '\"';

		fmt::print(csv, format, movie.ID, name, categories, price, attributes, image, description);
	}
	fmt::print("Exported movies to {}\n", filename.string());
	return true;
		
}

bool Converter::generateCombinationsCSV(std::filesystem::path filename, Movie::Time smallest_time)
{
	if (!movies.size())
		return false;

	std::ofstream csv(filename);
	if (!csv.is_open())
	{
		fmt::print(fg(fmt::color::red), "ERROR: Can't write to file {}", filename.string());
	}

	fmt::print(csv, "Indeks produktu; Atrybut(Nazwa:Typ:Pozycja)*; " \
		"Wartość(Wartość:Pozycja)*; Wpływ na cenę; Ilość \n");
	const auto format = std::string_view{ "{};Data:select:3, Godzina:select:4;{};{};250 \n" };

	for (const auto& movie : movies)
	{
		for (const auto& time : movie.times)
		{
			auto orginal_price = 26.90f;
			auto price = orginal_price;
			if (movie.family_friendly)
				price = 23.90f;
			auto smallest_price = 14.90f;;

			int day_passed{};
			if (time.month == smallest_time.month)
			{
				day_passed = time.day - smallest_time.day;
			}
			else
			{
				auto older = time.day_of_week;
				auto younger = time.day_of_week;
				if (older >= younger)
					day_passed = older - younger;
				else
					day_passed = 7 + older - younger;
			}

			auto date_time_atr = fmt::format("{}-{}:3, {}-{}:4", time.month, time.day, time.hour, time.minutes);
			if ((time.day_of_week == 1) || (day_passed > 2))
			{
				price = smallest_price;
			}
			else if(day_passed == 1)
			{
				price = 22.90f;
				if (movie.family_friendly)
					price -= 2.00f;
			}
			else
			{
				price = 18.90f;
				if (movie.family_friendly)
					price -= 1.00f;
			}
			fmt::print(csv, format, movie.ID, date_time_atr, price - orginal_price);
		}
	}

	fmt::print("Exported variants to {}\n", filename.string());
	return true;
}
