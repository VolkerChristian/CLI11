#ifdef CLI_SINGLE_FILE
#include "CLI11.hpp"
#else
#include "CLI/CLI.hpp"
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <fstream>

using ::testing::HasSubstr;
using ::testing::Not;

TEST(THelp, Basic) {
    CLI::App app{"My prog"};

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("My prog"));
    EXPECT_THAT(help, HasSubstr("-h,--help"));
    EXPECT_THAT(help, HasSubstr("Options:"));
    EXPECT_THAT(help, HasSubstr("Usage:"));

}

TEST(THelp, OptionalPositional) {
    CLI::App app{"My prog"};

    std::string x;
    app.add_option("something", x, "My option here");

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("My prog"));
    EXPECT_THAT(help, HasSubstr("-h,--help"));
    EXPECT_THAT(help, HasSubstr("Options:"));
    EXPECT_THAT(help, HasSubstr("Positionals:"));
    EXPECT_THAT(help, HasSubstr("something TEXT"));
    EXPECT_THAT(help, HasSubstr("My option here"));
    EXPECT_THAT(help, HasSubstr("Usage: program [OPTIONS] [something]"));
}

TEST(THelp, Hidden) {
    CLI::App app{"My prog"};

    std::string x;
    app.add_option("something", x, "My option here")
        ->group("Hidden");
    std::string y;
    app.add_option("--another", y)
        ->group("Hidden");

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("My prog"));
    EXPECT_THAT(help, HasSubstr("-h,--help"));
    EXPECT_THAT(help, HasSubstr("Options:"));
    EXPECT_THAT(help, HasSubstr("[something]"));
    EXPECT_THAT(help, Not(HasSubstr("something ")));
    EXPECT_THAT(help, Not(HasSubstr("another")));
}

TEST(THelp, OptionalPositionalAndOptions) {
    CLI::App app{"My prog"};
    app.add_flag("-q,--quick");

    std::string x;
    app.add_option("something", x, "My option here");

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("My prog"));
    EXPECT_THAT(help, HasSubstr("-h,--help"));
    EXPECT_THAT(help, HasSubstr("Options:"));
    EXPECT_THAT(help, HasSubstr("Usage: program [OPTIONS] [something]"));

}

TEST(THelp, RequiredPositionalAndOptions) {
    CLI::App app{"My prog"};
    app.add_flag("-q,--quick");

    std::string x;
    app.add_option("something", x, "My option here")
        ->required();

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("My prog"));
    EXPECT_THAT(help, HasSubstr("-h,--help"));
    EXPECT_THAT(help, HasSubstr("Options:"));
    EXPECT_THAT(help, HasSubstr("Positionals:"));
    EXPECT_THAT(help, HasSubstr("Usage: program [OPTIONS] something"));
}

TEST(THelp, EnvName) {
    CLI::App app{"My prog"};
    std::string input;
    app.add_option("--something", input)->envname("SOME_ENV");

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("SOME_ENV"));
}

TEST(THelp, Requires) {
    CLI::App app{"My prog"};

    CLI::Option* op1 = app.add_flag("--op1");
    app.add_flag("--op2")->requires(op1);

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("Requires: --op1"));
}

TEST(THelp, Excludes) {
    CLI::App app{"My prog"};

    CLI::Option* op1 = app.add_flag("--op1");
    app.add_flag("--op2")->excludes(op1);

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("Excludes: --op1"));
}

TEST(THelp, Subcom) {
    CLI::App app{"My prog"};

    app.add_subcommand("sub1");
    app.add_subcommand("sub2");

    std::string help = app.help();
    EXPECT_THAT(help, HasSubstr("Usage: program [OPTIONS] [SUBCOMMAND]"));

    app.require_subcommand();

    help = app.help();
    EXPECT_THAT(help, HasSubstr("Usage: program [OPTIONS] SUBCOMMAND"));

}

TEST(THelp, IntDefaults) {
    CLI::App app{"My prog"};

    int one{1}, two{2};
    app.add_option("--one", one, "Help for one", true); 
    app.add_set("--set", two, {2,3,4}, "Help for set", true);

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("--one"));
    EXPECT_THAT(help, HasSubstr("--set"));
    EXPECT_THAT(help, HasSubstr("1"));
    EXPECT_THAT(help, HasSubstr("=2"));
    EXPECT_THAT(help, HasSubstr("2,3,4"));

}

TEST(THelp, SetLower) {
    CLI::App app{"My prog"};

    std::string def{"One"};
    app.add_set_ignore_case("--set",def, {"oNe", "twO", "THREE"}, "Help for set", true);

    std::string help = app.help();

    EXPECT_THAT(help, HasSubstr("--set"));
    EXPECT_THAT(help, HasSubstr("=One"));
    EXPECT_THAT(help, HasSubstr("oNe"));
    EXPECT_THAT(help, HasSubstr("twO"));
    EXPECT_THAT(help, HasSubstr("THREE"));

}
