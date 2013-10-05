#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-cyc");
    BOOST_CHECK(GetBoolArg("-cyc"));
    BOOST_CHECK(GetBoolArg("-cyc", false));
    BOOST_CHECK(GetBoolArg("-cyc", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-cyco"));
    BOOST_CHECK(!GetBoolArg("-cyco", false));
    BOOST_CHECK(GetBoolArg("-cyco", true));

    ResetArgs("-cyc=0");
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", false));
    BOOST_CHECK(!GetBoolArg("-cyc", true));

    ResetArgs("-cyc=1");
    BOOST_CHECK(GetBoolArg("-cyc"));
    BOOST_CHECK(GetBoolArg("-cyc", false));
    BOOST_CHECK(GetBoolArg("-cyc", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nocyc");
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", false));
    BOOST_CHECK(!GetBoolArg("-cyc", true));

    ResetArgs("-nocyc=1");
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", false));
    BOOST_CHECK(!GetBoolArg("-cyc", true));

    ResetArgs("-cyc -nocyc");  // -cyc should win
    BOOST_CHECK(GetBoolArg("-cyc"));
    BOOST_CHECK(GetBoolArg("-cyc", false));
    BOOST_CHECK(GetBoolArg("-cyc", true));

    ResetArgs("-cyc=1 -nocyc=1");  // -cyc should win
    BOOST_CHECK(GetBoolArg("-cyc"));
    BOOST_CHECK(GetBoolArg("-cyc", false));
    BOOST_CHECK(GetBoolArg("-cyc", true));

    ResetArgs("-cyc=0 -nocyc=0");  // -cyc should win
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", false));
    BOOST_CHECK(!GetBoolArg("-cyc", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--cyc=1");
    BOOST_CHECK(GetBoolArg("-cyc"));
    BOOST_CHECK(GetBoolArg("-cyc", false));
    BOOST_CHECK(GetBoolArg("-cyc", true));

    ResetArgs("--nocyc=1");
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", false));
    BOOST_CHECK(!GetBoolArg("-cyc", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-cyc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-cyc", "eleven"), "eleven");

    ResetArgs("-cyc -bar");
    BOOST_CHECK_EQUAL(GetArg("-cyc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-cyc", "eleven"), "");

    ResetArgs("-cyc=");
    BOOST_CHECK_EQUAL(GetArg("-cyc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-cyc", "eleven"), "");

    ResetArgs("-cyc=11");
    BOOST_CHECK_EQUAL(GetArg("-cyc", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-cyc", "eleven"), "11");

    ResetArgs("-cyc=eleven");
    BOOST_CHECK_EQUAL(GetArg("-cyc", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-cyc", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-cyc", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-cyc", 0), 0);

    ResetArgs("-cyc -bar");
    BOOST_CHECK_EQUAL(GetArg("-cyc", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-cyc=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-cyc", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-cyc=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-cyc", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--cyc");
    BOOST_CHECK_EQUAL(GetBoolArg("-cyc"), true);

    ResetArgs("--cyc=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-cyc", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-nocyc");
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", true));
    BOOST_CHECK(!GetBoolArg("-cyc", false));

    ResetArgs("-nocyc=1");
    BOOST_CHECK(!GetBoolArg("-cyc"));
    BOOST_CHECK(!GetBoolArg("-cyc", true));
    BOOST_CHECK(!GetBoolArg("-cyc", false));

    ResetArgs("-nocyc=0");
    BOOST_CHECK(GetBoolArg("-cyc"));
    BOOST_CHECK(GetBoolArg("-cyc", true));
    BOOST_CHECK(GetBoolArg("-cyc", false));

    ResetArgs("-cyc --nocyc");
    BOOST_CHECK(GetBoolArg("-cyc"));

    ResetArgs("-nocyc -cyc"); // cyc always wins:
    BOOST_CHECK(GetBoolArg("-cyc"));
}

BOOST_AUTO_TEST_SUITE_END()
