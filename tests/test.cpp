#include "request.h"
#include "dashboard.h"
#include "parser.h"
#include "backend.h"
#include "application.h"

#include <gtest/gtest.h>
#include <list>
#include <memory>
#include <thread>

using namespace std;

// Test Request Logic
TEST ( TestRequestStruct, Request1)
{
    request req("127.0.0.1","/api/rest","GET","apache",100,404, 1000);
    EXPECT_EQ("127.0.0.1 apache GET /api/rest 100", req.toString());
    EXPECT_EQ("/api", req.getSection());

    request req2("127.0.0.1","/website","GET","apache",100,404, 1000);
    EXPECT_EQ("127.0.0.1 apache GET /website 100",req2.toString());
    EXPECT_EQ("/website",req2.getSection());
}



int main(int argc,char *argv[]) {
 ::testing::InitGoogleTest(&argc,argv);
 return RUN_ALL_TESTS();
}
