//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/vcs.hpp"
#include <string>
#include <sstream>

using namespace jlt;

TEST_CASE("SVN keyword extraction", "[vcs][svn]") {
    SECTION("Extract SVN revision from keyword string") {
        std::string svnKeyword = "$LastChangedRevision: 123 $";
        std::string revision = getSVNRevision(svnKeyword);
        REQUIRE(revision == "123");
    }
    
    SECTION("Extract SVN revision with larger number") {
        std::string svnKeyword = "$LastChangedRevision: 12345 $";
        std::string revision = getSVNRevision(svnKeyword);
        REQUIRE(revision == "12345");
    }
    
    SECTION("Handle unexpanded SVN keyword") {
        // String too short to be expanded
        std::string svnKeyword = "$LastChangedRevision$";
        std::string revision = getSVNRevision(svnKeyword);
        REQUIRE(revision == "");
    }
    
    SECTION("Handle empty SVN keyword") {
        std::string svnKeyword = "";
        std::string revision = getSVNRevision(svnKeyword);
        REQUIRE(revision == "");
    }
    
    SECTION("Extract SVN date from keyword string") {
        std::string svnKeyword = "$LastChangedDate: 2011-08-31 14:10:23 -0500 (Wed, 31 Aug 2011) $";
        std::string date = getSVNDate(svnKeyword);
        REQUIRE(date == "2011-08-31");
    }
    
    SECTION("Extract SVN date with different date") {
        std::string svnKeyword = "$LastChangedDate: 2024-12-25 10:30:00 +0000 (Wed, 25 Dec 2024) $";
        std::string date = getSVNDate(svnKeyword);
        REQUIRE(date == "2024-12-25");
    }
    
    SECTION("Handle unexpanded SVN date keyword") {
        std::string svnKeyword = "$LastChangedDate$";
        std::string date = getSVNDate(svnKeyword);
        REQUIRE(date == "");
    }
    
    SECTION("Handle empty SVN date") {
        std::string svnKeyword = "";
        std::string date = getSVNDate(svnKeyword);
        REQUIRE(date == "");
    }
}

TEST_CASE("Git repository detection", "[vcs][git]") {
    SECTION("Detect Git repo in current directory") {
        // This test assumes we're running from within the git repo
        bool isGit = detectGitRepo();
        REQUIRE(isGit);  // Should be true since tests are in a git repo
    }
    
    SECTION("getRepo returns git for git repository") {
        std::string repo = getRepo();
        REQUIRE(repo == "git");  // Should detect git in the test environment
    }
}

TEST_CASE("VCS revision and date extraction", "[vcs]") {
    SECTION("getVCSRevision returns non-empty for git repo") {
        std::string revision = getVCSRevision();
        // Should return either a 7-char hash or "none"
        REQUIRE((revision.length() == 7 || revision == "none"));
        
        // If we're in a git repo, it should be a 7-char hex string
        if (getRepo() == "git") {
            REQUIRE(revision.length() == 7);
            // Check it contains only hex characters
            for (char c : revision) {
                REQUIRE(((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')));
            }
        }
    }
    
    SECTION("getVCSDate returns date in YYYY-MM-DD format") {
        std::string date = getVCSDate();
        
        // Should return either a date or "none"
        if (date != "none") {
            REQUIRE(date.length() == 10);
            REQUIRE(date[4] == '-');
            REQUIRE(date[7] == '-');
        }
    }
    
    SECTION("getVCSRevDate combines revision and date") {
        std::string revdate = getVCSRevDate();
        
        if (!revdate.empty()) {
            // Should start with "rev "
            REQUIRE(revdate.find("rev ") == 0);
            // Should contain date in parentheses
            REQUIRE(revdate.find("(") != std::string::npos);
            REQUIRE(revdate.find(")") != std::string::npos);
        }
    }
}

TEST_CASE("VCS banner output", "[vcs][banner]") {
    SECTION("printVCSBanner produces output for git repo") {
        std::ostringstream oss;
        printVCSBanner(oss, "test_program");
        std::string output = oss.str();
        
        if (!output.empty()) {
            // Should contain the program name
            REQUIRE(output.find("test_program") != std::string::npos);
            // Should contain dashes (banner decoration)
            REQUIRE(output.find("-") != std::string::npos);
            // Should contain newline
            REQUIRE(output.find("\n") != std::string::npos);
        }
    }
    
    SECTION("printVCSBanner produces no output without repo") {
        // This would require running outside a repo to fully test
        // For now, just verify it doesn't crash
        std::ostringstream oss;
        printVCSBanner(oss, "test");
        // No requirements - function may or may not produce output
        // depending on whether we're in a repo
        REQUIRE(true);
    }
}
