#include <algorithm>
#include <clocale>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

inline std::string lower_string(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

struct holder
{
    holder(std::string n) : name{lower_string(n)}
    {
    }

    std::string name;
};

inline bool operator<(const holder & left, const holder & right) noexcept
{
    return left.name < right.name;
}

class ballot_box
{

public:
    using vote_result = std::pair<holder, int>;

public:
    bool vote(const holder & voter, const holder & vote)
    {
        const bool valid_vote = _voters.insert(voter).second;

        if (valid_vote)
        {
            _votes[vote] += 1;
        }

        return valid_vote;
    }

    auto count(void) const noexcept
    {
        return _voters.size();
    }

    std::vector<vote_result> results(void) const
    {
        std::vector<vote_result> v;

        v.reserve(count());

        std::copy(_votes.cbegin(), _votes.cend(), std::back_inserter(v));

        std::sort(v.begin(), v.end(), [](const vote_result & left, const vote_result & right) { return left.second > right.second; });

        return v;
    }

private:
    std::set<holder> _voters;
    std::map<holder, int> _votes;
};

template <typename T>
std::string query_str(T && prompt)
{
    std::cout << std::forward<T>(prompt) << std::flush;

    std::string input;
    std::getline(std::cin, input);

    return input;
}

template <typename Integer, typename T>
Integer query_int(T && prompt)
{
    const std::string read_data = query_str(std::forward<T>(prompt));

    std::istringstream converter(read_data);

    Integer res;

    converter >> res;

    return res;
}

int main(int, char **)
{
    try
    {
        std::setlocale(LC_ALL, "en_US.UTF-8");

        ballot_box the_box;

        using count_type = decltype(the_box.count());

        count_type voters_count = query_int<count_type>("How many to vote? ");

        while (the_box.count() < voters_count)
        {
            std::cout << std::endl << "Vote " << the_box.count() + 1 << " out of " << voters_count << std::endl;

            holder voter{query_str("Voter name: ")};
            holder vote_cast{query_str("Vote: ")};

            if (the_box.vote(voter, vote_cast))
            {
                std::cout << "Vote of " << voter.name << " acknowledged." << std::endl;
            }
            else
            {
                std::cout << voter.name << " already voted!" << std::endl;
            }
        }

        const auto results = the_box.results();

        int order = 1;

        std::cout << std::endl << "Vote results: " << std::endl;

        for (const auto & vote : results)
        {
            std::cout << order++ << " - " << vote.first.name << " : " << vote.second << " votes" << std::endl;
        }
    }
    catch (const std::exception & e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}