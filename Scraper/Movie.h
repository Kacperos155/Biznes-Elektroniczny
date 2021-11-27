#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct Movie
{
	Movie() = default;
	unsigned ID{};
	std::string name{};
	std::string description{};
	std::string image_url{};
	std::string image_hash{};
	bool dolby = false;
	bool family_friendly = false;
	uint8_t from_age{};
	uint8_t duration{};

	enum class Type {
		POLISH,
		SUBTITLES,
		DUBBING
	}type{};

	struct Time {
		uint8_t month{};
		uint8_t day{};
		uint8_t hour{};
		uint8_t minutes{};
		uint8_t day_of_week{};
	};
	std::vector<Time> times{};

	bool operator==(const Movie& other)
	{
		if (this->name != other.name)
			return false;
		if (this->type != other.type)
			return false;
		if (this->dolby != other.dolby)
			return false;
		return true;
	}

	bool operator<(const Movie& other)
	{
		return this->ID < other.ID;
	}
};


inline void to_json(nlohmann::json& j, const Movie::Time& t)
{
	j = nlohmann::json
	{
		{"month", t.month}, {"day", t.day},
		{"hour", t.hour}, {"minutes", t.minutes},
		{"day_of_week", t.day_of_week}
	};
}

inline void from_json(const nlohmann::json& j, Movie::Time& t)
{
	j.at("month").get_to(t.month);
	j.at("day").get_to(t.day);
	j.at("hour").get_to(t.hour);
	j.at("minutes").get_to(t.minutes);
	j.at("day_of_week").get_to(t.day_of_week);
}

inline void to_json(nlohmann::json& j, const Movie& m)
{
	j = nlohmann::json
	{
		{"ID", m.ID},
		{"name", m.name}, {"description", m.description},
		{"image_url", m.image_url}, {"image_hash", m.image_hash},
		{"dolby", m.dolby}, {"family_friendly", m.family_friendly},
		{"from_age", m.from_age}, {"duration", m.duration},
		{"type", m.type}, {"time", m.times}
	};
}

inline void from_json(const nlohmann::json& j, Movie& m)
{
	j.at("ID").get_to(m.ID);
	j.at("name").get_to(m.name);
	j.at("description").get_to(m.description);
	j.at("image_url").get_to(m.image_url);
	j.at("image_hash").get_to(m.image_hash);
	j.at("dolby").get_to(m.dolby);
	j.at("family_friendly").get_to(m.family_friendly);
	j.at("from_age").get_to(m.from_age);
	j.at("duration").get_to(m.duration);
	j.at("type").get_to(m.type);
	j.at("time").get_to(m.times);
}